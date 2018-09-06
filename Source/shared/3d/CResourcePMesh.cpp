/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




#include "CResourcePMesh.h"
#include "d3dapp.h"
#include "CThing_util.h"
#include <DXErr8.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CResourcePMesh::CResourcePMesh()
{
 	//defaults
    
    m_pPMeshes           = NULL;
    m_cPMeshes           = 0;
    m_pPMeshFull         = NULL;
    m_dwNumMaterials     = 0L;
    m_mtrlMeshMaterials  = NULL;
	m_fObjectRadius = 0;
	m_vObjectCenter = D3DXVECTOR3(0,0,0);
		
    vec_bb_min = D3DXVECTOR3(0,0,0); //bounding box
    vec_bb_max = D3DXVECTOR3(0,0,0);
   	
	
 
}

CResourcePMesh::~CResourcePMesh()
{
	dx_invalidate();

	//kill all our texture junk

	//we don't want to lose any custom texture changes, so this not
	//done in the invalidate() function

	for (unsigned int i=0; i < m_vec_texinfo.size(); i++)
	{

		SAFE_DELETE_ARRAY(m_vec_texinfo[i]->m_texture_id_array);
		SAFE_DELETE(m_vec_texinfo[i]);

	}
}
	

void CResourcePMesh::dx_create()
{

    if (get_refs() < 1) return; //nobody needs us, why should we load?

    if (m_pPMeshFull) return; //don't load it again, that's stupid
    if (m_pPMeshFull) dx_invalidate(); //destroy if it's loaded
//	log_msg("Loading mesh %s.", GetName());

    DWORD cVerticesPerMesh;
  
 // Load mesh
    LPD3DXBUFFER pAdjacencyBuffer = NULL;
    LPDIRECT3DVERTEXBUFFER8 pVertexBuffer = NULL;
    LPD3DXMESH   pMesh = NULL;
    LPD3DXPMESH  pPMesh = NULL;
    LPD3DXMESH   pTempMesh = NULL;
    LPD3DXBUFFER pD3DXMtrlBuffer = NULL;
    BYTE*        pVertices = NULL;
    HRESULT      hr;
    DWORD        dw32BitFlag;
    DWORD        cVerticesMin;
    DWORD        cVerticesMax;
    DWORD        iPMesh;
    D3DXWELDEPSILONS Epsilons;
    DWORD        i;
    D3DXMATERIAL* d3dxMaterials;
    LPD3DXBUFFER p_dx_error = NULL;

   TCHAR        strMediaPath[512];
 	strcpy(strMediaPath, GetName());								   
    // Create full path to mesh
	strcpy(str_path, ::get_path_from_string(GetName()));

	int i_tex_index = 0; //we might change this later to not always init with the original
	
	//we always load the default info here
         
    if( !exist(strMediaPath))
    {
        sprintf(strMediaPath, "%s\\%s", GetExeDir(), GetName());
        
        if( !exist(strMediaPath))
        {
            log_error("Could not find model %s.", strMediaPath);
            return;
        }
        
    }

    // Load the mesh from the specified file
    if( FAILED( hr = D3DXLoadMeshFromX( strMediaPath, D3DXMESH_MANAGED, cls_tglo.p_d3dDevice,
                                        &pAdjacencyBuffer, &pD3DXMtrlBuffer,
                                        &m_dwNumMaterials, &pMesh ) ) )
    {
        // hide error so that device changes will not cause exit, shows blank screen instead
        log_error("D3DXLoadMeshFromX failed. (%s)", GetName());
        goto End;
    }
 	if (m_vec_texinfo.size() == 0)
	{
		//add our base info, it hasn't been created yet
		m_vec_texinfo.push_back(NULL);
		m_vec_texinfo[0] = new CTexSetting(m_dwNumMaterials);
		strcpy(m_vec_texinfo[0]->st_name, "ORIGINAL");
	}

	//LogMsg("%s before cleaning adj buff is %d bytes.", GetName(), pAdjacencyBuffer->GetBufferSize());

    dw32BitFlag = (pMesh->GetOptions() & D3DXMESH_32BIT);

	// perform simple cleansing operations on mesh
 
//note, this seems to leak memory internally when processing mole.x. (while doing something called "BreakBowTies")
   //No other mesh does it so I assume this to be a bug in D3DX.  ???  
   
if( FAILED( hr = D3DXCleanMesh( pMesh, (DWORD*)pAdjacencyBuffer->GetBufferPointer(), &pTempMesh, 
                                           (DWORD*)pAdjacencyBuffer->GetBufferPointer(), &p_dx_error ) ) )
    {
        m_dwNumMaterials = 0;
        log_error("Unable to clean mesh %s.", GetName());
        goto End;
    }
    if (p_dx_error)
	{
		LogMsg("Comments while cleaning mesh %s - %s", GetName(), p_dx_error->GetBufferPointer());
		SAFE_RELEASE(p_dx_error);
	}
	if (pTempMesh != pMesh)
	{
	 //it didn't just return the same Mesh, so I guess we should use the one they gave us
	SAFE_RELEASE(pMesh);
    pMesh = pTempMesh;
	//LogMsg("%s cleaned. New adj buff is %d bytes.", GetName(), pAdjacencyBuffer->GetBufferSize());
 
	//this fixed a small memory leak that most people miss
	//pAdjacencyBuffer->Release();  //it now goes back to having 1 reference

} else
	{

	//	LogMsg("%s not changed.", GetName());
		SAFE_RELEASE(pTempMesh); //same pointer as pMesh, we just need to release a reference
	}
	pTempMesh = NULL;



	
    //  Perform a weld to try and remove excess vertices like the model bigship1.x in the DX8.0 SDK (current model is fixed)
    //    Weld the mesh using all epsilons of 0.0f.  A small epsilon like 1e-6 works well too
    memset(&Epsilons, 0, sizeof(D3DXWELDEPSILONS));
    if( FAILED( hr = D3DXWeldVertices( pMesh, &Epsilons, 
                                                (DWORD*)pAdjacencyBuffer->GetBufferPointer(), 
                                                (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL ) ) )
    {
        m_dwNumMaterials = 0;
        goto End;
    }

    // verify validity of mesh for simplification
    if( FAILED( hr = D3DXValidMesh( pMesh, (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL ) ) )
    {
        assert(0 && "This should never happen, right?");
		m_dwNumMaterials = 0;
        goto End;
    }
  
    // Allocate a material/texture arrays
    d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    m_mtrlMeshMaterials = new D3DMATERIAL8[m_dwNumMaterials];
   

	
 // Copy the materials and load the textures
    for( i=0; i<m_dwNumMaterials; i++ )
    {
        m_mtrlMeshMaterials[i] = d3dxMaterials[i].MatD3D;
        m_mtrlMeshMaterials[i].Ambient = m_mtrlMeshMaterials[i].Diffuse;

		// Find the path to the texture and create that texture
       	if (d3dxMaterials[i].pTextureFilename)
		{
			
			//set our default texture
			strcpy(m_vec_texinfo[i_tex_index]->m_texture_id_array[i].st_tex, d3dxMaterials[i].pTextureFilename);
			//init texture, taking into account possible texture overrides
		InitTexture(i_tex_index, i, false); //init everything, but don't actually load it now
		} else 	m_vec_texinfo[i_tex_index]->m_texture_id_array[i].i_tex = ::C_RESOURCE_NONE;
	  
	}
    SAFE_RELEASE(pD3DXMtrlBuffer);
    
    // Lock the vertex buffer, to generate a simple bounding sphere
    hr = pMesh->GetVertexBuffer( &pVertexBuffer );
    if( FAILED(hr) )
	{
		assert(0 && "This should never happen, right?");
		goto End;
	}

    hr = pVertexBuffer->Lock( 0, 0, &pVertices, D3DLOCK_NOSYSLOCK );
    if( FAILED(hr) )
	{ 
    assert(0 && "This should never happen, right?");
        goto End;
	}

    hr = D3DXComputeBoundingSphere( pVertices, pMesh->GetNumVertices(),
                                    pMesh->GetFVF(),
                                    &m_vObjectCenter, &m_fObjectRadius );
    //also let's get the bounding box
	
    hr = D3DXComputeBoundingBox( pVertices, pMesh->GetNumVertices(),
                                    pMesh->GetFVF(),
                                    &this->vec_bb_min, &vec_bb_max);
	
	pVertexBuffer->Unlock();
    SAFE_RELEASE(pVertexBuffer);

    if( FAILED(hr) || m_dwNumMaterials == 0 )
    {
       log_error("Unable to find all materials in %s?", GetName());
       goto End;
    }
    if ( !(pMesh->GetFVF() & D3DFVF_NORMAL) )
    {
        hr = pMesh->CloneMeshFVF( dw32BitFlag|D3DXMESH_MANAGED, pMesh->GetFVF() | D3DFVF_NORMAL, 
                                            cls_tglo.p_d3dDevice, &pTempMesh );
        if (FAILED(hr))
        {
           log_error("Failed to clone mesh %s.", GetName());
		assert(0 && "This should never happen, right?");        
		   goto End;
        }
        D3DXComputeNormals( pTempMesh, NULL );

        SAFE_RELEASE(pMesh);
        pMesh = pTempMesh;
		pTempMesh = NULL;
    }

    hr = D3DXGeneratePMesh( pMesh, (DWORD*)pAdjacencyBuffer->GetBufferPointer(),
                            NULL, NULL, 1, D3DXMESHSIMP_VERTEX, &pPMesh);
    if( FAILED(hr) )
    {
         log_error("Failed to generate PMesh %s.", GetName());

        goto End;
    }

    cVerticesMin = pPMesh->GetMinVertices();
    cVerticesMax = pPMesh->GetMaxVertices();

    //uncomment this to make 10 LOD meshed
	
//	cVerticesPerMesh = (cVerticesMax - cVerticesMin) / 10;

	cVerticesPerMesh = cVerticesMax;

    m_cPMeshes = max(1, (DWORD)ceil((cVerticesMax - cVerticesMin) / (float)cVerticesPerMesh));
    m_pPMeshes = new LPD3DXPMESH[m_cPMeshes];
    if (m_pPMeshes == NULL)
    {    assert(0 && "This should never happen, right?");
        hr = E_OUTOFMEMORY;
        goto End;
    }
    memset(m_pPMeshes, 0, sizeof(LPD3DXPMESH) * m_cPMeshes);

    // clone full size pmesh
    hr = pPMesh->ClonePMeshFVF( D3DXMESH_MANAGED | D3DXMESH_VB_SHARE, pPMesh->GetFVF(), cls_tglo.p_d3dDevice, &m_pPMeshFull );
    if (FAILED(hr))
	{
         assert(0 && "This should never happen, right?");
		 goto End;
	}

    // clone all the separate pmeshes
    for (iPMesh = 0; iPMesh < m_cPMeshes; iPMesh++)
    {
        hr = pPMesh->ClonePMeshFVF( D3DXMESH_MANAGED | D3DXMESH_VB_SHARE, pPMesh->GetFVF(), cls_tglo.p_d3dDevice, &m_pPMeshes[iPMesh] );
        if (FAILED(hr))
		{
             assert(0 && "This should never happen, right?");
			 goto End;
		}

        // trim to appropriate space
        hr = m_pPMeshes[iPMesh]->TrimByVertices(cVerticesMin + cVerticesPerMesh * iPMesh, cVerticesMin + cVerticesPerMesh * (iPMesh+1), NULL, NULL);
        if (FAILED(hr))
		{
             assert(0 && "This should never happen, right?");
			 goto End;
		}

        hr = m_pPMeshes[iPMesh]->OptimizeBaseLOD(D3DXMESHOPT_VERTEXCACHE, NULL);
        if (FAILED(hr))
		{
             assert(0 && "This should never happen, right?");
			 goto End;
		}
    }

    // set current to be maximum number of vertices
    m_iPMeshCur = m_cPMeshes - 1;
    hr = m_pPMeshes[m_iPMeshCur]->SetNumVertices(cVerticesMax);
   
//	log_msg("%s has %d meshes.", GetName(), m_cPMeshes);
	
	if (FAILED(hr))
    {
       log_error("Unable to convert SetNumVertices with %s.", GetName());
        goto End;
    }
    hr = m_pPMeshFull->SetNumVertices(cVerticesMax);
    if (FAILED(hr))
    {
         log_error("Unable to convert SetNumVertices with %s.", GetName());

        goto End;
    }

  b_loaded = true;

End:
    
    SAFE_RELEASE( pAdjacencyBuffer );
    SAFE_RELEASE( pD3DXMtrlBuffer );
    SAFE_RELEASE( pMesh );
    SAFE_RELEASE( pPMesh );

    if (FAILED(hr))
    {
        for (iPMesh = 0; iPMesh < m_cPMeshes; iPMesh++)
        {
            SAFE_RELEASE( m_pPMeshes[iPMesh] );
        }

       	if (m_pPMeshes)
		SAFE_DELETE_ARRAY(m_pPMeshes);
        m_cPMeshes = 0;
        m_pPMeshes = NULL;
        SAFE_RELEASE( m_pPMeshFull )
    }
 
}

//i know, horrible naming but I'm lazy, i_tex_index is the index of the set of textures we want
//to use, index is the actual texture index

void CResourcePMesh::InitTexture(int i_tex_index, int i_index, bool b_load_now)
{
    
    //strip the path from the texture
    char st_filename[256];
    char st_temp[255];
    TCHAR strMediaPath[512];
    
    
    //first, let's just try the texture "as is" just in case it's right.
    if (exist(m_vec_texinfo[i_tex_index]->m_texture_id_array[i_index].st_tex))
    {
        strcpy(strMediaPath, m_vec_texinfo[i_tex_index]->m_texture_id_array[i_index].st_tex);
    }	else
    {
        
        
        strcpy(strMediaPath, GetName());								   
        // Create full path to mesh
        strcpy(str_path, ::get_path_from_string(GetName()));
        strcpy(st_temp, m_vec_texinfo[i_tex_index]->m_texture_id_array[i_index].st_tex);
        
        get_filename_only(st_filename, st_temp);
        //if the same file name with a .dds extension exists, let's use it instead.
        strcpy(st_temp, st_filename);
        change_file_extension(st_temp, "dds");
        sprintf(strMediaPath, "%s\\%s", this->str_path,st_temp); 
        
        
        
        if (!exist(strMediaPath))
        {
            //no .dds version exists, let's use the normal way.
            sprintf(strMediaPath, "%s\\%s", this->str_path,st_filename); 
            
            if (!exist(strMediaPath))
            {
                //if a mod, let's also check the base dir
               	sprintf(strMediaPath, "%s%s\\%s", GetExeDir(), this->str_path,st_temp); 
                
                if (!strMediaPath)
                {
                    //check for default name too
                    sprintf(strMediaPath, "%s%s\\%s", GetExeDir(), this->str_path,st_filename); 
                }
                
            }
            
        }
    }
    
    
	m_vec_texinfo[i_tex_index]->m_texture_id_array[i_index].i_tex = cls_tglo.p_manager->get_dynamic_resource_by_string(::C_RESOURCE_TEXTURE, strMediaPath);
	if (b_load_now)
	{
	
	cls_tglo.p_manager->load_if_needed(m_vec_texinfo[i_tex_index]->m_texture_id_array[i_index].i_tex);
	//let's also load it if needed
	}
}	


int CResourcePMesh::ReplaceTexture(char *st_tex_system, char *st_old, char *st_new)
{
	//	log_msg("Replacing texture %s with texture %s in %s.", st_old, st_new, GetName());
	//first figure out what texture system this is for, if a new one we will 
    int i_tex_system = 0;
	if (m_vec_texinfo.size() == 0)
	{
		log_error("Can't switch textures, not initted yet.");
		return 0;
	}
	for (unsigned int i=1; i < m_vec_texinfo.size(); i++)   //we skip texture 0, can't replace that
	{
		if (stricmp(m_vec_texinfo[i]->st_name, st_tex_system) == 0)
		{
			//found match.
			i_tex_system = i;
		}
		
	}
	
	if (i_tex_system == 0)
	{
		//no match was found.  We'll just have to add this.
		m_vec_texinfo.push_back(NULL);
		i_tex_system = m_vec_texinfo.size()-1;
		m_vec_texinfo[i_tex_system] = new CTexSetting(m_dwNumMaterials);
		//name it so we remember this for next time
		strcpy(m_vec_texinfo[i_tex_system]->st_name, st_tex_system);
		//great, ready for action.
	}
	
	
	if (b_loaded)
	{
		//let's do a realtime switch
		
		//cycle through and match up the tex name
		char st_temp[256];
		for( DWORD i=0; i<m_dwNumMaterials; i++ )
		{
			
			//get filename without . thingie
			get_filename_only( st_temp,m_vec_texinfo[0]->m_texture_id_array[i].st_tex );
			
			//strip file extension
			char *p_check = strchr(st_temp, '.');
			if (p_check)
			{
				//truncate filename at the .
				p_check[0] = 0;
			}
			
			if (stricmp(st_temp, st_old) == 0 )
			{
				  // log_msg("Replacing %s with %s. (text id is %d)", st_temp, st_new, i);
				
				//turn on the alt texture we just got
				strcpy(m_vec_texinfo[i_tex_system]->m_texture_id_array[i].st_tex, st_new);
				
				//kill the texture we had loaded	, if we had one
				
				if (m_vec_texinfo[i_tex_system]->m_texture_id_array[i].i_tex != C_RESOURCE_NONE)
					cls_tglo.p_manager->delete_ref(m_vec_texinfo[i_tex_system]->m_texture_id_array[i].i_tex);
				m_vec_texinfo[i_tex_system]->m_texture_id_array[i].Reset();
				InitTexture(i_tex_system, i, true);
				return i_tex_system;
			}
			
		}
	}
	  else
	{
		//not loaded yet, so let's just assume we're replacing whatever texture 1 is.
		
  	//     strcpy(m_vec_texinfo[i_tex_system]->m_texture_id_array[0].st_tex, st_new);
   	  //   return i_tex_system;
		  log_msg("Texture replace error");
	}


log_error("TextureReplace failed, can't find texture %s to replace with %s.", st_old, st_new);
return i_tex_system;
}

void CResourcePMesh::dx_invalidate()
{
 	  //free everything
     DWORD iPMesh;
	
  /*
  	for (int i_tex_system = 0; i_tex_system < m_vec_texinfo.size(); i_tex_system++)
	{
		
		for( UINT i=0; i<m_dwNumMaterials; i++ )
		{
			//let them know we're done with the texture
			if (m_vec_texinfo[i_tex_system]->m_texture_id_array[i].i_tex != C_RESOURCE_NONE)
				cls_tglo.p_manager->delete_ref(m_vec_texinfo[i_tex_system]->m_texture_id_array[i].i_tex);
		}
	}

    */
     
   
     for (unsigned int i_tex_system = 0; i_tex_system < m_vec_texinfo.size(); i_tex_system++)
	 {
		 
		 for( UINT i=0; i<m_dwNumMaterials; i++ )
		 {
			 //let them know we're done with the texture
			if (m_vec_texinfo[i_tex_system]->m_texture_id_array[i].i_tex != C_RESOURCE_NONE)
            {
       		cls_tglo.p_manager->delete_ref(m_vec_texinfo[i_tex_system]->m_texture_id_array[i].i_tex);
 
			 //reset the texture so it will get loaded again
			 m_vec_texinfo[i_tex_system]->m_texture_id_array[i].ReleaseTextureReference();
            }
		 }
	 }

   SAFE_RELEASE( m_pPMeshFull );
    for (iPMesh = 0; iPMesh < m_cPMeshes; iPMesh++)
    {
        SAFE_RELEASE( m_pPMeshes[iPMesh] );
    }
    	if (m_pPMeshes)
	SAFE_DELETE_ARRAY(m_pPMeshes);
    m_cPMeshes = 0;

    SAFE_DELETE_ARRAY( m_mtrlMeshMaterials );
   // m_dwNumMaterials = 0L;
    b_loaded = false;
	//LogMsg("Releasing the PMESH %s.", GetName());

} 



void CResourcePMesh::dx_restore()
{
 if (m_cPMeshes < 1) dx_create(); //only reload if we have to
}


//send -1 for the texture set
void CResourcePMesh::draw(bool b_lod_active, int i_lod, int i_texture_set, DWORD dw_color, int i_render_extra)
{
 
  //save material, switch back to it at the end
    static D3DMATERIAL8 mat_orig;
    cls_tglo.p_d3dDevice->GetMaterial(&mat_orig);



    int i_temp_set = 0; 
  bool b_tint_active = false;
  bool b_full_alpha = false;
  static DWORD dw_alpha = 255;
  if (dw_color != D3DCOLOR_ARGB(255,255,255,255))
  {
	  //we need to enable color alpha/tinting support
       
 
       if (dw_alpha = GetAlphaFromD3DCOLOR(dw_color) < 130)
       {
           //since we want this whole model faded at less than 50 percent we're going to need to allow
           //more than on/off alpha, even though it does screw up the output a little...
           b_full_alpha = true;
         if (dw_alpha == 0) 
         {
             //um yeah, it's totally invisible.  Don't render it.
             return;
         }
       }
	  	//transparency test
			cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	    	cls_tglo.p_d3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dw_color);
			cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
			cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);


			cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	    
		    cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	
			b_tint_active = true;
  } else
  {

//	  		cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
//				  	cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

  }


  if( m_pPMeshes )
        {
      	
      if (i_render_extra != C_RENDER_EXTRA_SHADOW)
      cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SPECULARENABLE, true );

     // Set and draw each of the materials in the mesh
            for( DWORD i=0; i<m_dwNumMaterials; i++ )
            {
			    if (&m_mtrlMeshMaterials[i])
                cls_tglo.p_d3dDevice->SetMaterial( &m_mtrlMeshMaterials[i] );
				
				//should we use a default texture or a new fancy special one?
				if (i_texture_set != 0)
				{
					//no reason to check if the texture set is normal
					if (m_vec_texinfo[i_texture_set]->m_texture_id_array[i].st_tex[0] != 0)
					{
						//do a quick check to make sure it's actually loaded and ready
						if (m_vec_texinfo[i_texture_set]->m_texture_id_array[i].i_tex == NULL)
						{
							this->InitTexture(i_texture_set, i, true);
						}
						//yes, use this instead.  We made need to do a load check here later.
					    i_temp_set = i_texture_set;
					}	
					   else
					{
						//force default.
						i_temp_set = 0;
					}
	            } 


				if (m_vec_texinfo[i_temp_set]->m_texture_id_array[i].i_tex != C_RESOURCE_NONE)
				{
				if (m_vec_texinfo[i_temp_set]->m_texture_id_array[i].p_tex == NULL)
				{
				 //cache out the info for speed	
					CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(m_vec_texinfo[i_temp_set]->m_texture_id_array[i].i_tex);
                  //cache the info out
				cls_tglo.p_manager->load_if_needed(m_vec_texinfo[i_temp_set]->m_texture_id_array[i].i_tex);
				  m_vec_texinfo[i_temp_set]->m_texture_id_array[i].p_tex = p_tex->get_texture();
				m_vec_texinfo[i_temp_set]->m_texture_id_array[i].b_alpha = p_tex->HasAlpha();
				}

		//	log_msg("Setting %s to tex %s. (Setting %d, id %d)", GetName(), m_vec_texinfo[i_temp_set]->m_texture_id_array[i].st_tex,
		//			i_temp_set, i);
				cls_tglo.p_d3dDevice->SetTexture( 0, m_vec_texinfo[i_temp_set]->m_texture_id_array[i].p_tex);
				
					//ok, if it's a dxt3 format texture let's go ahead and render with alpha channel support.
					if ( (m_vec_texinfo[i_temp_set]->m_texture_id_array[i].b_alpha) || b_tint_active)
					{
						cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
						cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
						cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
						
						
                        //let's force only 2 bit alpha basically, stops the bilinear filtering from
                        //blurring around the edges.
                        if( cls_tglo.p_d3dapp->m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
                        {
                             cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
                            cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER );
                            if ( (i_render_extra == C_RENDER_EXTRA_SHADOW) || b_full_alpha)
                            {
                                cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
                           
                               if (i_render_extra == C_RENDER_EXTRA_SHADOW)
                                {
                                
                                cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
                                }
                             } 
                            else
                            {
                                //don't render partial transparencies.. looks better
                                cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHAREF,  128); // tolerance
                             }	
                            
                        }	
                        
                    }
                    
				} else
				{
					//no texture, let it use it's own stuff
					cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
					cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);

				}

				if (b_lod_active)
				{
                 	if (i_lod+1 > int(m_cPMeshes)) i_lod = m_cPMeshes-1;
					m_pPMeshes[i_lod]->DrawSubset( i );
				}
                else
                    m_pPMeshFull->DrawSubset( i );
           
			     if (m_vec_texinfo[i_temp_set]->m_texture_id_array[i].i_tex == C_RESOURCE_NONE)
				 {
				   //put the render stats back
	 				cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
				  	cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				 }
				 if ( (m_vec_texinfo[i_temp_set]->m_texture_id_array[i].b_alpha) || b_tint_active)
				 {
					 cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
					 cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
					 //   cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING,    TRUE );
					 
					 cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
					 cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
				 }
				 
			}
     	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SPECULARENABLE, false );
     
  }


  if (b_tint_active)
  {
      //turn off the tinting stuff  
      //	  cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
      //	  cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
      
      
      cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,   D3DTA_TEXTURE );
      cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2,   D3DTA_DIFFUSE );
      
      cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,   D3DTA_TEXTURE );
      cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,   D3DTA_DIFFUSE );
      
      cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
          //	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	    cls_tglo.p_d3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR,D3DCOLOR_ARGB(255,255,255,255));
  }
   //set material back to whatever it was before
    cls_tglo.p_d3dDevice->SetMaterial(&mat_orig);
   //cls_tglo.p_d3dDevice->SetMaterial(NULL);
}
//does this texture set have alpha in any of the textures?
bool CResourcePMesh::HasAlpha(int i_texture_set)
{
	for( DWORD i=0; i<m_dwNumMaterials; i++ )
	{
		bool b_used_alpha =false;
		
		//cls_tglo.p_d3dDevice->SetMaterial( &m_mtrlMeshMaterials[i] );
		if (m_vec_texinfo[i_texture_set]->m_texture_id_array[i].i_tex != C_RESOURCE_NONE)
		{
			CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(m_vec_texinfo[i_texture_set]->m_texture_id_array[i].i_tex);
			cls_tglo.p_d3dDevice->SetTexture( 0, p_tex->get_texture());
			
			//ok, if it's a dxt3 format texture let's go ahead and render with alpha channel support.
			if (p_tex->HasAlpha())
			{
			  return true;
			}
		}
	}

  return false;
}


//same as above but check a specific texture

bool CResourcePMesh::HasAlphaSpecific(int i_texture_set, int i_tex)
{
		bool b_used_alpha =false;
		
	//	cls_tglo.p_d3dDevice->SetMaterial( &m_mtrlMeshMaterials[i_tex] );
		if (m_vec_texinfo[i_texture_set]->m_texture_id_array[i_tex].i_tex != C_RESOURCE_NONE)
		{
			CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(m_vec_texinfo[i_texture_set]->m_texture_id_array[i_tex].i_tex);
			cls_tglo.p_d3dDevice->SetTexture( 0, p_tex->get_texture());
			
			//ok, if it's a dxt3 format texture let's go ahead and render with alpha channel support.
			if (p_tex->HasAlpha())
			{
			  return true;
			}
		}

  return false;
}



//if NULL is sent for p_out_hit then we won't compute the exact hit point, saves time

bool CResourcePMesh::AccurateRayCheck(D3DXVECTOR3 *p_vec_origin, D3DXVECTOR3 *p_vec_dir, D3DXVECTOR3 *p_out_hit)
{
	
	BOOL b_hit;
    LPD3DXBUFFER pHitBuffer = NULL;
  	DWORD dw_hits;
	DWORD p_index_to_face_hit;
	float f_U, f_V;
	float f_dist;
	HRESULT hr = D3DXIntersect(m_pPMeshFull, p_vec_origin, p_vec_dir, &b_hit, &p_index_to_face_hit, &f_U, &f_V, &f_dist, &pHitBuffer, &dw_hits);
  
	if (FAILED(hr))
	{
		log_error("PMeshRayCheckError with %s - (%s)", GetName(), DXGetErrorString8(hr));
		return false;
	}

	if (b_hit)
	{
		//contact has been made sir.  
		if (!p_out_hit)
		{
			//we don't need extra data, save time by doing this
			SAFE_RELEASE(pHitBuffer);
			return true;
		} else
		{
		 p_out_hit->x = p_vec_origin->x + p_vec_dir->x * f_dist;	
		 p_out_hit->y = p_vec_origin->y + p_vec_dir->y * f_dist;	
		 p_out_hit->z = p_vec_origin->z + p_vec_dir->z * f_dist;	
	 	SAFE_RELEASE(pHitBuffer);

		 return true;
		}
	
		/*
	
	  	//eventually we'll want to do it the real way, so we can get the tri we hit and check the
		//alpha coords

		log_msg("Hit made on %s. Face index is %d. Buffer contains %d hits.", GetName(), p_index_to_face_hit,
			pHitBuffer->GetBufferSize());
	    
		BYTE*   m_pIndexedVertices;
	  
		WORD *indices;
		if (SUCCEEDED(hr=m_pPMeshFull->LockIndexBuffer(D3DLOCK_READONLY,(BYTE **) &indices)))
		{
			if (SUCCEEDED(hr=m_pPMeshFull->LockVertexBuffer(D3DLOCK_READONLY,(BYTE **)
				&m_pIndexedVertices))) 
			{
			   D3DXVECTOR3 A, B, C;
		 	   DWORD vertex_size = D3DXGetFVFVertexSize(m_pPMeshFull->GetFVF());

			   D3DXVECTOR3 *temp;
				//get the triangle we actually hit
			   temp = (D3DXVECTOR3*)&m_pIndexedVertices[(indices[p_index_to_face_hit]*vertex_size)];
			   A= *temp;
				
			   temp = (D3DXVECTOR3*)&m_pIndexedVertices[((indices[p_index_to_face_hit]+1)*vertex_size)];
			   B= *temp;
			   
			   temp = (D3DXVECTOR3*)&m_pIndexedVertices[((indices[p_index_to_face_hit]+2)*vertex_size)];
			   C= *temp;
			  
			   //to get the exact position let's do the ray through tri function
				D3DXPLANE plane;	
			   	D3DXPlaneFromPoints(&plane, &A, &B, &C);
				D3DXPlaneIntersectLine(p_out_hit,&plane,p_vec_origin,p_vec_dir);
		
			   // get the vertices
				log_msg("A %s, B %s, C %s Hitpoint: %s", print_vector(A), print_vector(B), print_vector(C), print_vector(*p_out_hit));
				m_pPMeshFull->UnlockVertexBuffer();
				m_pPMeshFull->UnlockIndexBuffer();
			}
		} else
		{
			log_error("Failed to lock the index buffer of %s. (%s)", GetName(),DXGetErrorString8(hr)); 
		}
		
	  */


	}
	SAFE_RELEASE(pHitBuffer);

	return b_hit !=0;

}
