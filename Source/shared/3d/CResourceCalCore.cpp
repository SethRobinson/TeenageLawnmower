#include "CResourceCalCore.h"

CResourceCalCore::CResourceCalCore()
{
// p_resource = NULL;
}

CResourceCalCore::~CResourceCalCore()
{
 dx_invalidate();
 dx_kill();
}


//----------------------------------------------------------------------------//
// Parse the configuration file and load the whole model                      //
//----------------------------------------------------------------------------//

void log_cal_error()
{
  //retrieve error message and show it on our console window
  int i_error_core = CalError::getLastErrorCode();
  char st_error[256];


  switch(i_error_core)
  {
    case 0:
      strcpy(st_error, "No error found");
      break;
    case 1:
      strcpy(st_error, "Internal error");
      break;
    case 2:
      strcpy(st_error, "Invalid handle as argument");
      break;
    case 3:
      strcpy(st_error, "Memory allocation failed");
      break;
    case 4:
      strcpy(st_error, "File not found");
      break;
    case 5:
      strcpy(st_error, "Invalid file format");
      break;
    case 6:
      strcpy(st_error, "Parser failed to process file");
      break;
    case 7:
      strcpy(st_error, "Building of the index failed");
      break;
    case 8:
      strcpy(st_error, "There is no document to parse");
      break;
    case 9:
      strcpy(st_error, "The duration of the animation is invalid");
      break;
    case 10:
      strcpy(st_error, "Bone not found");
      break;
    case 11:
      strcpy(st_error, "Invalid attribute value");
      break;
    case 12:
      strcpy(st_error, "Invalid number of keyframes");
      break;
    case 13:
      strcpy(st_error, "Invalid animation type");
      break;
    case 14:
      strcpy(st_error, "Failed to create file");
      break;
    case 15:
      strcpy(st_error, "Failed to write to file");
      break;
    case 16:
      strcpy(st_error, "Incompatible file version");
      break;
    case 17:
      strcpy(st_error, "No mesh attached to the model");
      break;
    default:
      	strcpy(st_error, "Unknown error");
		break;
  }
  char st_crap[256];

  sprintf(st_crap, "%s %s (line %d) in %s.", st_error,
	  CalError::getLastErrorText().c_str(),
	  CalError::getLastErrorLine(),
	  CalError::getLastErrorFile().c_str());
  log_error(st_crap);
}

bool CResourceCalCore::parseModelConfiguration(char st_file[256])
{
  // open the model configuration file
 
  char st_path[256];
  
  strcpy(st_path, ::get_path_from_string(GetName()));
  strcat(st_path, "\\");
	
 
    CTextParse cls_parse;
  // cls_parse.set_path(st_data_path);
   
   if (!cls_parse.open(st_file))
   {
	 log_error("Couldn't open model cfg %s.",st_file);
	 return false;
   }
   
   //Create a core model instance, it actually worked without me doing this but..
    if(!m_calCoreModel.create("dummy"))
  {
  		  log_cal_error();
      return false;
  }


   
   //ready to read actual stuff!
	char *p_buffer;
 	char st_crap[255];
	char st_temp[400];
		while(cls_parse.get_next_line(&p_buffer))
		{
	 	strcpy(st_crap, cls_parse.get_word(1));
	
		if (strcmp(st_crap, "load_skeleton") == 0)
		{
			//clear vars/set defaults
			sprintf(st_temp,"%s%s",st_path, cls_parse.get_word(2));
			log_msg("Loading skeleton %s",st_temp);
			if(!m_calCoreModel.loadCoreSkeleton(st_temp))
			{
				log_cal_error();
				return false;
			}
			
		} else
		if (strcmp(st_crap, "load_mesh") == 0)
		{
			//clear vars/set defaults
			sprintf(st_temp,"%s%s",st_path, cls_parse.get_word(2));
			log_msg("Loading mesh %s",st_temp);
		   if(m_calCoreModel.loadCoreMesh(st_temp) == -1)
			{
				log_cal_error();
				return false;
			}
			
		} else
			if (strcmp(st_crap, "load_animation") == 0)
			{
				//clear vars/set defaults
				sprintf(st_temp,"%s%s",st_path, cls_parse.get_word(2));
				log_msg("Loading animation %s",st_temp);
				if(m_calCoreModel.loadCoreAnimation(st_temp) == -1)
				{
					log_cal_error();
					return false;
				}
				
			} else
		if (strcmp(st_crap, "load_material") == 0)
			{
				//clear vars/set defaults
				sprintf(st_temp,"%s%s",st_path, cls_parse.get_word(2));
				log_msg("Loading material %s",st_temp);
			 if(m_calCoreModel.loadCoreMaterial(st_temp) == -1)
  	{
					log_cal_error();
					return false;
				}
				
			} else
				{
				//nothing we know
			}
			
		}
		
    


   int materialId;
  for(materialId = 0; materialId < m_calCoreModel.getCoreMaterialCount(); materialId++)
  {
    // create the a material thread
    m_calCoreModel.createCoreMaterialThread(materialId);
    // initialize the material thread
    m_calCoreModel.setCoreMaterialId(materialId, 0, materialId);
  	
		// get the core material
		CalCoreMaterial *pCoreMaterial;
		pCoreMaterial = m_calCoreModel.getCoreMaterial(materialId);
		
		log_msg("Processing material %d..",materialId);
		
		// loop through all maps of the core material
		int mapId;
		for(mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
		{
			// get the filename of the texture
			std::string strFilename;
			strFilename = pCoreMaterial->getMapFilename(mapId);
			
			// load the texture from the file
			//  GLuint textureId;
			//textureId = loadTexture(strFilename);
			log_msg("Loading bitmap %s.",strFilename.c_str());
			char st_tex_path[256];
			sprintf(st_tex_path, "%s%s", st_path, strFilename.c_str());
			int i_tex_id = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE,
				st_tex_path);
			
			// store the opengl texture id in the user data of the map
			 pCoreMaterial->setMapUserData(mapId, (void *)i_tex_id);
		}

  }




  return true;
}


void CResourceCalCore::dx_create()
{
  	if (b_loaded) return;
	//  if (p_resource) return; //already loaded
	if (get_refs() < 1) return; //nobody needs us, why should we load?
	if (this->b_dynamic == false) if (get_refs() < 2) return; //ignore the first ref, this is
	//just to stop us from being removed when no longer needed

	// create the model instance from the loaded core model
 
	parseModelConfiguration(this->GetName());


	

     b_loaded = true;
}


void CResourceCalCore::dx_invalidate()
{
   
}

void CResourceCalCore::dx_kill()
{
//	if (!p_resource) return;

   if (!b_loaded) return;
	//return the texture refs
 
	  int materialId;
  for(materialId = 0; materialId < m_calCoreModel.getCoreMaterialCount(); materialId++)
  {
     // initialize the material thread
    m_calCoreModel.setCoreMaterialId(materialId, 0, materialId);
  	
		// get the core material
		CalCoreMaterial *pCoreMaterial;
		pCoreMaterial = m_calCoreModel.getCoreMaterial(materialId);
		// loop through all maps of the core material
		int mapId;
		for(mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
		{
			// store the opengl texture id in the user data of the map
			 cls_tglo.p_manager->delete_ref((int)pCoreMaterial->getMapUserData(mapId));
		}

  }


	
	// destroy core model instance
   m_calCoreModel.destroy();
   b_loaded = false;

   
}

void CResourceCalCore::dx_restore()
{
 //	if (p_resource) return;
	dx_create();
}