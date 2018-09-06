/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CRTM.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CRTM::CRTM()
{
	memset(this, 0, sizeof(CRTM));
	m_p_a_frame = new VertexVec();
	m_f_version = C_CRTM_VERSION;
	m_p_a_anim = new AnimVec();
}


CRTM::~CRTM()
{
	//release time
	DeleteAnimations();
	FreeAll();

}

void CRTM::DeleteAnimations()
{
	m_p_a_anim->clear();
	m_i_animations = 0;
	SAFE_DELETE(m_p_a_anim);

}

void CRTM::FreeAll()
{
	SAFE_FREE(m_p_a_faces);
	SAFE_FREE(m_p_a_tex);
	SAFE_FREE(m_p_a_normals);
	//delete any vertexes
	if (m_p_a_frame)
	{
		
		for (unsigned int i=0; i < m_p_a_frame->size(); i++)
		{
			SAFE_DELETE((*m_p_a_frame)[i]);
		}
	}
	
	SAFE_DELETE(m_p_a_frame); //kill the stl array of vertex arrays

	m_i_total_frames = 0;
	m_l_tex_verts = 0;
	m_l_faces = 0;
	m_l_normal_count = 0;
}

//this must be done before adding any verts
void CRTM::SetFrameCount(int i_frames)
{
	if (m_i_total_frames != 0)
	{
		log_error("Total frame count cannot be set twice.");
		return;
	}
	m_i_total_frames = i_frames;
	
	int i_index;
	//init each frame
	for (int i=0; i < m_i_total_frames; i++)
    {
	  m_p_a_frame->push_back(NULL);
	  i_index = m_p_a_frame->size()-1;
	  (*m_p_a_frame)[i_index] = new CRTMFrame();
	}

}


//if the vec_offset isn't null, we'll move all verts by this amount as we add them.
bool CRTM::AddVerts(int i_frame, D3DXVECTOR3 *p_verts, long i_count, D3DXVECTOR3 *p_vec_offset)
{
    if (this->m_i_total_frames == 0)
	{
		log_error("Cannot add verts until SetFrameCount is called.");
		return false;
	} else
	{
		if (i_frame > m_i_total_frames)
		{
			log_error("Can't add verts to frame %d. Out of range.", i_frame);
			return false;
		}
	}
	
	CRTMFrame *p_anim = (*m_p_a_frame)[i_frame]; //to make this easier on the eyes 
	
	long i_old_vert_count = p_anim->i_vert_count;
	
	p_anim->i_vert_count += i_count;

//	log_msg("Adding %d verts to anim %d. That anim now has %d verts.", i_count, i_frame,
//		p_anim->i_vert_count);
	
	//now let's really add the verts
	p_anim->p_verts = (D3DXVECTOR3*) realloc(p_anim->p_verts, p_anim->i_vert_count*sizeof(D3DXVECTOR3));

	if (!p_anim)
	{
		log_error("Error allocating the verts in frame %d.", i_frame);
		return false;
	}

	//memory allocated.  Now do straight copy to the correct place.
	memcpy(&p_anim->p_verts[i_old_vert_count], p_verts,i_count*sizeof(D3DXVECTOR3));

	//now modify them?

	if (p_vec_offset)
	{
		for (int i=i_old_vert_count; i < p_anim->i_vert_count; i++)
		{
		  p_anim->p_verts[i] += *p_vec_offset;
		 
		}
	}
	
	return true; //no error
}


bool CRTM::AddTexCoords(D3DXVECTOR2 *p_tex_verts, long i_count)
{
   	long l_old_verts = m_l_tex_verts;
	m_l_tex_verts += i_count;

	//now let's really add the verts
	m_p_a_tex = (D3DXVECTOR2*) realloc(m_p_a_tex , m_l_tex_verts*sizeof(D3DXVECTOR2));

	if (!m_p_a_tex)
	{
		log_error("Error allocating tex_verts");
		return false;
	}

	//memory allocated.  Now do straight copy to the correct place.
	memcpy(&m_p_a_tex[l_old_verts], p_tex_verts,i_count*sizeof(D3DXVECTOR2));
	return true; //no error
}

bool CRTM::AddNormals(D3DXVECTOR3 *p_normals, long i_count)
{
   	long l_old_normals = m_l_normal_count;
	m_l_normal_count += i_count;

	//now let's really add the verts
	m_p_a_normals = (D3DXVECTOR3*) realloc(m_p_a_normals , m_l_normal_count*sizeof(D3DXVECTOR3));

	if (!m_p_a_normals)
	{
		log_error("Error allocating tex_verts");
		return false;
	}

	//memory allocated.  Now do straight copy to the correct place.
	memcpy(&m_p_a_normals[l_old_normals], p_normals,i_count*sizeof(D3DXVECTOR3));
	return true; //no error
}




bool CRTM::AddFaces(WORD *p_faces, long i_count, bool b_auto_offset)
{

   	long l_old_faces = m_l_faces;
	m_l_faces += i_count;

	//now let's really add the verts
	m_p_a_faces = (WORD*) realloc(m_p_a_faces , m_l_faces*sizeof(WORD));

	if (!m_p_a_faces)
	{
		log_error("Error allocating faces");
		return false;
	}

	//memory allocated.  Now do straight copy to the correct place.
	memcpy(&m_p_a_faces[l_old_faces], p_faces,i_count*sizeof(WORD));

	if (b_auto_offset)
	{
		//renumber everything we just got...
		for (int i=l_old_faces; i < m_l_faces; i++)
		{
		  m_p_a_faces[i] += WORD((*m_p_a_frame)[0]->i_vert_count);
		 //log_msg("Added %d to vert, its now %d.", (*m_p_a_frame)[0]->i_vert_count, m_p_a_faces[i]);
		}

	}
	
	
	return true; //no error
}

bool CRTM::SaveRTM(char *fname)
{

	//open the file
	FILE *fp = fopen(fname, "wb");

	if (!fp)
	{
		log_error("Error saving RTM to %s.", fname);
		return false;
	}
	
	int i_size_of_us = sizeof(CRTM);

   	//first write our data size
	fwrite(&i_size_of_us, sizeof(int), 1, fp);

	//now write the real data
	fwrite(this, i_size_of_us, 1, fp);
	
	//now write out faces index
	fwrite(m_p_a_faces, m_l_faces*sizeof(WORD), 1, fp);

	//write tex coords
	fwrite(m_p_a_tex, m_l_tex_verts*sizeof(D3DXVECTOR2), 1, fp);

	//write normal info
	fwrite(m_p_a_normals, m_l_normal_count*sizeof(D3DXVECTOR3), 1, fp);

	//if we have animation data, let's write that.

	for (UINT k=0; k < m_p_a_anim->size(); k++)
	{
	 	fwrite(&(*m_p_a_anim)[k],  sizeof(CRTMAnim), 1, fp);
	   //log_msg("Saved anim %d. %s with %d and %d for ending.",k, (*m_p_a_anim)[k].st_name, 
	//	 (*m_p_a_anim)[k].i_start, (*m_p_a_anim)[k].i_end);
	}


	//cycle through and write info for each frame
	int i_vert_count = 0;
	for (UINT i=0; i < m_p_a_frame->size(); i++)
	{
	   //write how many verts are in it
	   i_vert_count = (*m_p_a_frame)[i]->i_vert_count;
	   fwrite(&i_vert_count, sizeof(int), 1, fp);

	   //write real vert data
	  fwrite((*m_p_a_frame)[i]->p_verts, i_vert_count*sizeof(D3DXVECTOR3), 1, fp);
	}

	fclose(fp);
   return true;
}

bool CRTM::AddAnimation(char *st_name, int i_start, int i_end)
{
	//push a new anim into our vector
	CRTMAnim anim;
	if (strlen(st_name) > 11)
	{
		log_error("Animation name %s is too long");
	}

	strcpy(anim.st_name, st_name);
	//convert it to uppercase
	strupr((char*)&anim.st_name);

	anim.i_start = i_start;
	anim.i_end = i_end;
	anim.i_length = i_end - i_start;

	m_p_a_anim->push_back(anim);
	m_i_animations = m_p_a_anim->size();
	return true;
}

//will return 0 if no anim is found, to stop a crash.
int CRTM::GetAnimIndexByName(char *st_name)
{
   for (UINT i=0; i < m_p_a_anim->size();i++)
   {
	   if (strcmp( (*m_p_a_anim)[i].st_name, st_name) == 0)
	   {
		   //found it
		   return i;
	   }
   }
  //if we got here we didn't find jack.
   log_msg("Unable to find anim %s.", st_name);
   return 0;
}

bool CRTM::LoadRTM(char *fname)
{
	FILE *fp = fopen(fname, "rb");

	if (!fp)
	{
		log_error("Error finding file %s to load.", fname);
		return false;
	}

	
	//clear us
	this->FreeAll(); //if we had any verts/etc loaded, they are gone
	DeleteAnimations(); //free all doesn't actually free all.  haha bad naming

	ZeroMemory(this, sizeof(CRTM)); //make sure we're in a good default position

	//load it
    int i_our_size = 0;
  
  //first load the size we're supposed to be
 	
	if (!fread(&i_our_size, sizeof(int), 1, fp))
  {
	  log_error("Couldn't read RTM info from %s.", fname);
		fclose(fp);
		return false;
  }

  if (!fread(this, i_our_size, 1, fp))
  {
	  log_error("Couldn't read RTM info from %s.", fname);
		fclose(fp);
		return false;
  }
 
  m_p_a_faces = NULL;
  m_p_a_tex = NULL;
  m_p_a_frame = NULL;
  m_p_a_normals = NULL;
  m_p_a_anim = new AnimVec();
  //ok, yes we now are a perfect copy of the class that was saved.  However, all our pointers currently
  //point to utter crap.  Let's fix them up and load the real info.
  
  //read how many faces
  if (m_l_faces > 0)
  {
	  
	  m_p_a_faces = (WORD*)malloc(m_l_faces*sizeof(WORD));
	  
	  if (!fread(m_p_a_faces, m_l_faces*sizeof(WORD), 1, fp))
	  {
		  log_error("Couldn't read RTM face info from %s.", fname);
		  fclose(fp);
		  return false;
	  }
   	 
//	  log_msg("Loaded faces for %d verts.", m_l_faces);
 /*	for (int k=0; k < m_l_faces; k+=3)
	{
		log_msg("Face %d - %d %d %d", k, this->m_p_a_faces[k],m_p_a_faces[k+1], m_p_a_faces[k+2] );
	}
  */

  }
  if (m_l_tex_verts > 0)
  {
	  m_p_a_tex = (D3DXVECTOR2*)malloc(m_l_tex_verts*sizeof(D3DXVECTOR2));
	  
	  if (!fread(m_p_a_tex, m_l_tex_verts*sizeof(D3DXVECTOR2), 1, fp))
	  {
		  log_error("Couldn't read RTM tex info from %s.", fname);
		  fclose(fp);
		  return false;
	  }
 
  //	  log_msg("Loaded %d texture coords.", m_l_tex_verts);
	  /*
	  for (int k=0; k < m_l_tex_verts; k++)
	{
		log_msg("Tex %d - %.3f %.3f", k, m_p_a_tex[k].x,  m_p_a_tex[k].y);
	}
  		*/
  
  }
  //load normals here later?
  if (m_l_normal_count > 0)
  {
	  m_p_a_normals = (D3DXVECTOR3*)malloc(m_l_normal_count*sizeof(D3DXVECTOR3));
	  
	  if (!fread(m_p_a_normals, m_l_normal_count*sizeof(D3DXVECTOR3), 1, fp))
	  {
		  log_error("Couldn't read RTM normal info from %s.", fname);
		  fclose(fp);
		  return false;
	  }
	  
//	  log_msg("Loaded normals for %d verts.", m_l_normal_count, 1, fp);
		  
  }

  //should we load any animation information? (like framenames, etc)
  if (m_i_animations > 0)
  {
	  CRTMAnim anim; //temp buffer to use
	  
	  for (int k=0; k < m_i_animations; k++)
	  {
		  //load each record
		  if (!fread(&anim, sizeof(CRTMAnim), 1, fp))
		  {
			  log_error("Couldn't read anim record %d in %s. Aborting read.", k, fname);
			  fclose(fp);
			  return false;
		  }
	
		  //actually add it to our vector
		  m_p_a_anim->push_back(anim);
	//	 log_msg("read anim %d. %s with %d and %d for ending.",k, (*m_p_a_anim)[k].st_name, 
	//	 (*m_p_a_anim)[k].i_start, (*m_p_a_anim)[k].i_end);

	  }
	  
  }	 else
  {
	  log_msg("Error!  RTM model has no animations");
  }
  


  //now load real poly data
 
  m_p_a_frame = new VertexVec(); //replace the bad pointer info that was there from  the load
  int i_temp = m_i_total_frames;
  m_i_total_frames = 0; //fake it into letting us reinit the frames
  SetFrameCount(i_temp); //this will set the m_i_total_frames to be correct again

  //now let's fill each frame with vertex data
  for (int i=0; i < m_i_total_frames; i++)
  {
  	CRTMFrame *p_anim = (*m_p_a_frame)[i]; //to make this easier on the eyes 

	//load how many verts are in this frame.  Probably a waste of space, why would the vert # change?
    if (!fread(&p_anim->i_vert_count, sizeof(LONG), 1, fp))
	{
		log_error("Couldn't read vert size for frame %d in %s.", i, fname);
		fclose(fp);
		return false;
	}

	if (p_anim->i_vert_count > 0)
	{
		
		//now let's really add the verts
		p_anim->p_verts = (D3DXVECTOR3*) malloc(p_anim->i_vert_count*sizeof(D3DXVECTOR3));
		
		if (!p_anim)
		{
			log_error("Error allocating the verts in frame %d while loading %s.", i, fname);
			fclose(fp);
			return false;
		}
		
		//memory allocated.  Now load the verts in
		if (!fread(p_anim->p_verts, p_anim->i_vert_count*sizeof(D3DXVECTOR3), 1, fp))
		{
			log_error("Couldn't read verts for frame %d in %s.", i, fname);
			fclose(fp);
			return false;
		}
	}

//	log_msg("loaded frame %d, it had %d verts.", i, p_anim->i_vert_count);

  }



 

  //all done
    fclose(fp);

   return true;

}
