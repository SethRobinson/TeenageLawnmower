#include "CResourceCalRender.h"
#include "CCalControl.h"

const C_RESOURCE_CAL_MAX_INDICES = 50000;

DWORD cal_shader[] =
{
    D3DVSD_STREAM( 0 ),
    D3DVSD_REG( 0, D3DVSDT_FLOAT3 ),
	D3DVSD_STREAM( 1 ),
    D3DVSD_REG( D3DVSDE_NORMAL, D3DVSDT_FLOAT3 ),
  	D3DVSD_STREAM( 2 ),
    D3DVSD_REG( D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2 ),
    D3DVSD_END()

};




CResourceCalRender::CResourceCalRender()
{
  this->i_max_verts = 30000;
  this->i_max_indexes = C_RESOURCE_CAL_MAX_INDICES;	
  p_vertex_buffer = NULL;
  p_index_buffer = NULL;
  p_vertex_shader = NULL;
  p_vertex_buffer_normals = NULL;
  p_vertex_buffer_tex = NULL;

}

CResourceCalRender::~CResourceCalRender()
{
 dx_invalidate();
 dx_kill();
}

//#define SKIP_RENDER

void CResourceCalRender::render_cal(CCalControl *p_controller, CalModel *p_cal_model)
{
 	
	// get the renderer of the model
  CalRenderer *pCalRenderer;
  pCalRenderer = p_cal_model->getRenderer();

  // begin the rendering loop
  if(!pCalRenderer->beginRendering())
  {
	  log_error("pCalRenderer is null!");
	  return;
  }

   	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SPECULARENABLE, true );
//	cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP );
 //   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP );



  // get the number of meshes
  int meshCount;
  meshCount = pCalRenderer->getMeshCount();

  // render all meshes of the model
  int meshId;
  for(meshId = 0; meshId < meshCount; meshId++)
  {
    // get the number of submeshes
    int submeshCount;
    submeshCount = pCalRenderer->getSubmeshCount(meshId);

    // render all submeshes of the mesh
    int submeshId;
    for(submeshId = 0; submeshId < submeshCount; submeshId++)
    {
      // select mesh and submesh for further data access
      if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
      {
        unsigned char meshColor[4];
	// mat.SetAmbient(0,0,0,1);
	  //mat.SetEmissive(1,1,1,1);
	 // mat.SetDiffuse(0,1,1,1);
	//  mat.SetSpecular(0.1f,0.1f,0.1f,1);
	 // mat.SetPower(10);
	// set the material ambient color
     	
		
		 CalCoreMaterial *pCoreMaterial;
         pCoreMaterial = p_cal_model->getCoreModel()->getCoreMaterial(0);
		
	//	pCalRenderer->getAmbientColor(&meshColor[0]);
	
	  float f_max_color = 255.0f;
	 // mat.SetAmbient(meshColor[0] / f_max_color,meshColor[1] / f_max_color,meshColor[2] / f_max_color, meshColor[3] / f_max_color);
     // mat.SetAmbien
	   // set the material diffuse color
     //   pCalRenderer->getDiffuseColor(&meshColor[0]);
    //   mat.SetDiffuse(float(meshColor[0]) / f_max_color,float(meshColor[1]) / f_max_color,float(meshColor[2]) / f_max_color, float(meshColor[3]) / f_max_color);

	   if (cls_tglo.p_cls_input->is_key_down(DIK_K))
	   {
		 char st_crap[255];
		 sprintf(st_crap, "Diffuse is %f, %f, %f, %f", mat.m_mtrlObject.Diffuse.r,mat.m_mtrlObject.Diffuse.g,
			 mat.m_mtrlObject.Diffuse.b, mat.m_mtrlObject.Diffuse.a);
		 log_msg(st_crap);

	   }
        // set the material specular color
        
	   pCalRenderer->getSpecularColor(&meshColor[0]);
       mat.SetSpecular(meshColor[0] / f_max_color,meshColor[1] / f_max_color,meshColor[2] / f_max_color, meshColor[3] / f_max_color);

        // set the material shininess factor
      //  float shininess;
        //shininess = 50.0f; //pCalRenderer->getShininess();
       mat.SetPower((pCalRenderer->getShininess()*600));
	//   mat.SetPower(50);
	   mat.Use(cls_tglo.p_d3dDevice);


        // get the transformed vertices of the submesh
        
			// Now populate the vertices with data
	HRESULT hr;
	float *	pVertices = NULL;
	WORD*		pIndices  = NULL;



	// Lock the Vertex Buffer
	int nCounter = 0;
	if (FAILED(hr=p_vertex_buffer->Lock(0, 0, (BYTE**)&pVertices, 0)))
	{
		  	    log_error("CResourceCalRender (%s)", DXGetErrorString8(hr));

	}
	int vertexCount;
  // pCalRenderer->getVertices((float*)&crap);
  	#ifndef SKIP_RENDER //for a speed test I'm doing
  
  vertexCount = pCalRenderer->getVertices(pVertices);
#endif
  p_vertex_buffer->Unlock();


	
  
  if (FAILED(hr=p_index_buffer->Lock(0, 0, (BYTE**)&pIndices, 0)))
	{
		  	    log_error("CResourceCalRender (%s)", DXGetErrorString8(hr));


	}

	int faceCount;
  	#ifndef SKIP_RENDER //for a speed test I'm doing


	
	static int a_index[C_RESOURCE_CAL_MAX_INDICES]; //a temp buffer, we have to convert from 32 bit indices to 16 bit.. argh
	//faceCount = pCalRenderer->getFaces(pIndices);

	faceCount = pCalRenderer->getFaces(a_index);

	//actually copy them over in the correct format
	static int i_face_verts;
	i_face_verts = faceCount*3;
	
	for (int i=0; i < i_face_verts; i++)
	{
	  pIndices[i] =  a_index[i];
	  
	}

      if (cls_tglo.p_cls_input->is_key_down(DIK_K))
	   {
		log_msg("Read %d indices.", faceCount);
	  }

#endif
	p_index_buffer->Unlock();
	
     // get the transformed normals of the submesh
    	
	if (FAILED(hr=p_vertex_buffer_normals->Lock(0, 0, (BYTE**)&pVertices, 0)))
	{
		  	    log_error("CResourceCalRender (%s)", DXGetErrorString8(hr));

	}
		#ifndef SKIP_RENDER //for a speed test I'm doing


        pCalRenderer->getNormals(pVertices);
#endif
	 	p_vertex_buffer_normals->Unlock();

     // get the texture coordinates of the submesh
       // static float meshTextureCoordinates[30000][2];
       /// int textureCoordinateCount;
      //  textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);
	if (FAILED(hr=p_vertex_buffer_tex->Lock(0, 0, (BYTE**)&pVertices, 0)))
	{
		  	    log_error("CResourceCalRender (%s)", DXGetErrorString8(hr));

	}
	 	#ifndef SKIP_RENDER //for a speed test I'm doing

    int textureCoordinateCount;
    textureCoordinateCount =  pCalRenderer->getTextureCoordinates(0, pVertices);
#endif
	 	p_vertex_buffer_tex->Unlock();

	 

        // set the texture coordinate buffer and state if necessary
        
		if(pCalRenderer->getMapCount() > 0) 
        {
           //there is a texture we need to use

			int i_tex_id = (int) pCalRenderer->getMapUserData(0);

			if (i_tex_id)
			{
				CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
                if (!p_tex->b_loaded) p_tex->dx_create(); //load texture
				cls_tglo.p_d3dDevice->SetTexture( 0, p_tex->get_texture());
			}

        } else
		{
		  //no texture
		//	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
		//	cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);


			cls_tglo.p_d3dDevice->SetTexture( 0, NULL);
		}
		  
	   //	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

        // draw the submesh
        //glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &meshFaces[0][0]);
 	 	#ifndef SKIP_RENDER //for a speed test I'm doing


		cls_tglo.p_d3dDevice->SetVertexShader (p_vertex_shader);
		cls_tglo.p_d3dDevice->SetStreamSource (0, p_vertex_buffer, sizeof(D3DXVECTOR3));
		cls_tglo.p_d3dDevice->SetStreamSource (1, p_vertex_buffer_normals, sizeof(D3DXVECTOR3));
		cls_tglo.p_d3dDevice->SetStreamSource (2, p_vertex_buffer_tex, sizeof(float)*2);
		cls_tglo.p_d3dDevice->SetIndices( p_index_buffer, 0 );
		
		// Draw the grid
		cls_tglo.p_d3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
			0, 
			vertexCount,  // number of vertices
			0, 
			faceCount); // number of primitives
	 
#endif	
     }
    }
  }

  // clear vertex array state
 
  // clear light

  // end the rendering
  pCalRenderer->endRendering();
  	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SPECULARENABLE, false );




}


void CResourceCalRender::dx_create()
{
  	if (b_loaded) return;
	//  if (p_resource) return; //already loaded
	if (get_refs() < 1) return; //nobody needs us, why should we load?
	if (this->b_dynamic == false) if (get_refs() < 2) return; //ignore the first ref, this is
	//just to stop us from being removed when no longer needed

	HRESULT hr;
	//create the vertex buffers
	 	// Create the vertex buffer
	if( FAILED(hr= cls_tglo.p_d3dDevice->CreateVertexBuffer (	this->i_max_verts, 0,
													D3DFVF_CAL3D, 
													D3DPOOL_DEFAULT, 
													&p_vertex_buffer ) ) )
	{
		  	    log_error("CResourceCalRender (%s)", DXGetErrorString8(hr));

		return;
	}

	// Create the index buffer
	if( FAILED(hr= cls_tglo.p_d3dDevice->CreateIndexBuffer( this->i_max_indexes,
	                                             0, D3DFMT_INDEX16,
		                                         D3DPOOL_MANAGED, 
												 &p_index_buffer ) ) )
	{
		  	    log_error("CResourceCalRender (%s)", DXGetErrorString8(hr));

		return;
	}


	//vertex buffer for the NORMALS
	if( FAILED(hr= cls_tglo.p_d3dDevice->CreateVertexBuffer (	this->i_max_verts, 0,
													D3DFVF_CAL3D_NORM, 
													D3DPOOL_DEFAULT, 
													&p_vertex_buffer_normals ) ) )
	{
		DXTRACE_ERR_NOMSGBOX( "CResourceCalRender", hr );
		return;
	}


		//vertex buffer for the TEXTURE coordinates
	if( FAILED(hr= cls_tglo.p_d3dDevice->CreateVertexBuffer (	this->i_max_verts, 0,
													D3DFVF_CAL3D_TEX, 
													D3DPOOL_DEFAULT, 
													&p_vertex_buffer_tex ) ) )
	{
		  	    log_error("CResourceCalRender (%s)", DXGetErrorString8(hr));

		return;
	}






	//now create the vertex shader we're going to use
	if( FAILED(hr=cls_tglo.p_d3dDevice->CreateVertexShader(cal_shader, NULL, &p_vertex_shader, 0)))
	{
	  DXTRACE_ERR_NOMSGBOX( "CResourceCalRender", hr );
		return;


	}


	b_loaded = true;
}


void CResourceCalRender::dx_invalidate()
{
  	if (b_loaded == false) return; //already unloaded
	//	if (!p_resource) return;
	SAFE_RELEASE(p_vertex_buffer);
    SAFE_RELEASE(p_index_buffer);
    
	SAFE_RELEASE(p_vertex_buffer_normals);
	SAFE_RELEASE(p_vertex_buffer_tex);
	
	if( p_vertex_shader != 0 )
    {
        cls_tglo.p_d3dDevice->DeleteVertexShader( p_vertex_shader );
        p_vertex_shader = 0;
    }

   b_loaded = false;
 
}

void CResourceCalRender::dx_kill()
{


}

void CResourceCalRender::dx_restore()
{
 //	if (p_resource) return;
	dx_create();
}