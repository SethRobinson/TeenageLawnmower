#include "CCalControl.h"


CCalControl::CCalControl()
{
  //init everything
  p_vmulti = NULL;
  m_blendTime = 0.3f;
 m_lastTick = 0;
 i_rend_id = 0;
 p_render = NULL;
 
}

CCalControl::~CCalControl()
{

//kill everything

	this->m_calModel.destroy();
 //the .ref to the core will get released in CVisualMulti, we don't have to worry about it
	//remove ref from renderer
	if (cls_tglo.p_manager)
	cls_tglo.p_manager->delete_ref(i_rend_id);

}


bool CCalControl::init_cal_instance(CVisualMulti *p_vmulti_new, char st_cal_name[])
{
	p_vmulti = p_vmulti_new; //remember our owner, only the owner calls anything
	//from this class so it's pretty safe.

	log_msg("Creating instance of %s.", st_cal_name);
	p_vmulti->i_visual_id = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_CAL_CORE,
	st_cal_name);
	
	//also create/find rendered

	i_rend_id = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_CAL_RENDER,
		"CalRender");
	p_render = (CResourceCalRender*) cls_tglo.p_manager->get(i_rend_id);

  return true;
}



void CCalControl::init_and_attach_meshes()
{
  // load all textures and store the opengl texture id in the corresponding map in the material
  //get a pointer to the core model

  //first make SURE the original model is loaded


  CalCoreModel * p_m_calCoreModel = &((CResourceCalCore*)cls_tglo.p_manager->get(this->p_vmulti->i_visual_id))->m_calCoreModel;

  m_calModel.create(p_m_calCoreModel);


  if (p_m_calCoreModel->getCoreMeshCount() == 0)
  {
   log_error("CalCoreMesh object has zero meshes or isn't loaded yet, object %s can't attach to it.",
	   this->p_vmulti->get_thing_name());
   return;
  }	 else
  {
    log_msg("Found %d meshes.",	p_m_calCoreModel->getCoreMeshCount());
  }
  
  // attach all meshes to the model
  int meshId;
  for(meshId = 0; meshId < p_m_calCoreModel->getCoreMeshCount(); meshId++)
  {
    m_calModel.attachMesh(meshId);
  }

  // set the material set of the whole model
  m_calModel.setMaterialSet(0);

  // set initial animation state
  m_currentAnimationId = 0;

  if (p_m_calCoreModel->getCoreAnimationCount() < 1)
  {
 	  //no animations!


  } else
  {

  
  m_leftAnimationTime = p_m_calCoreModel->getCoreAnimation(m_currentAnimationId)->getDuration() - m_blendTime;
 
  
  if(p_m_calCoreModel->getCoreAnimationCount() > 1)
  {
    m_calModel.getMixer()->executeAction(m_currentAnimationId, 0.0f, m_blendTime);
  }
  else
  {
	  m_calModel.getMixer()->blendCycle(m_currentAnimationId, 1.0f, 0.0f);
  }

  } 

  // we're done
  log_msg("Done initting new model.");

  m_calModel.setLodLevel(1.0f);
 
}

void CCalControl::update()
{
  unsigned int tick;
  tick = GetTickCount();

  // calculate the amount of elapsed seconds
  float elapsedSeconds;
  elapsedSeconds = (float)(tick - m_lastTick) / 1000.0f;

   // check if the time has come to blend to the next animation
    if(m_calModel.getCoreModel()->getCoreAnimationCount() > 1)
    {
      m_leftAnimationTime -= elapsedSeconds;
      if(m_leftAnimationTime <= m_blendTime)
      {
        // get the next animation
        m_currentAnimationId = (m_currentAnimationId + 1) % m_calModel.getCoreModel()->getCoreAnimationCount();

        // fade in the new animation
        m_calModel.getMixer()->executeAction(m_currentAnimationId, m_leftAnimationTime, m_blendTime);

        // adjust the animation time left until next animation flip
        m_leftAnimationTime = m_calModel.getCoreModel()->getCoreAnimation(m_currentAnimationId)->getDuration() - m_blendTime;
      }
    
  	
	}

   m_calModel.update(elapsedSeconds);

   m_lastTick = tick;

}

void CCalControl::draw()
{
	//are we ready to render?
	if (this->m_calModel.getSkeleton() == NULL)
	{
	  return; //something is wrong, don't try to draw it

	}
	
	
	//update model info
	update();
	//set any properties for the rendering
	
	//let's draw the thing
 	p_render->render_cal(this, &m_calModel);

}