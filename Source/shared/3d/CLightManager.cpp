#include "CLightManager.h"

#include "CThing_globals.h"
#include "CCamera.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLightManager::CLightManager()
{
    cls_tglo.p_light_manager = this;
    //init stuffs
    m_timer_update.set_interval(100); //how often we should update the lights
}

CLightManager::~CLightManager()
{
    cls_tglo.p_light_manager = NULL;

}



int CLightManager::AddLight(D3DXVECTOR3 *p_vec_pos, D3DXVECTOR3 *p_vec_color, float f_power, float f_trigger_distance)
{
    int i_light = -1; //final index
    
    //check for empty light spot
     for (unsigned int i=0; i < m_vec_light.size(); i++)
    {
       if (!m_vec_light[i].b_active)
       {
           //found one not in use
           i_light = i;
           break;
       }
    }

    if (i_light == -1)
    {
        //no one old found to use, let's actually add one to our vector
        m_vec_light.push_back(CLightInfo());
        i_light = m_vec_light.size()-1;
    }

    //now actually fill it with info
    m_vec_light[i_light].b_active = true;
    m_vec_light[i_light].f_power = f_power;
    m_vec_light[i_light].f_trigger_distance = f_trigger_distance;
    m_vec_light[i_light].vec_color = *p_vec_color;
    m_vec_light[i_light].vec_pos = *p_vec_pos;
   // log_msg("added light %d.", i_light);
 //   UpdateLights();
    return i_light;
}

void CLightManager::RemoveLight(int i_light)
{
    if (i_light < 0 || i_light >= int(m_vec_light.size()))
    {
        log_error("Can't remove light %d, doesn't exist.", i_light);
        return;
    }

    m_vec_light[i_light].b_active = false; //will get over written with the next new light
   // UpdateLights();
}



void CLightManager::UpdateD3DLight( CLightInfo & light_info, int i_d3d_light)
{
    if (!cls_tglo.p_d3dDevice)
    {
        return;
    }
    D3DLIGHT8 m_light;
   // log_msg("Updating light %d.",i_d3d_light);

    ZeroMemory( &m_light, sizeof(D3DLIGHT8) );
    m_light.Type        = D3DLIGHT_POINT;
    m_light.Diffuse.r = light_info.vec_color.x;
    m_light.Diffuse.g = light_info.vec_color.y;
    m_light.Diffuse.b = light_info.vec_color.z;
    m_light.Position = light_info.vec_pos;
    m_light.Range        =  light_info.f_power;
    m_light.Attenuation1 = 1;
    m_light.Attenuation0 = 0.0;
    m_light.Attenuation2 = 0.0;
     
    m_light.Specular.a = 1;
    m_light.Specular.r = 1;
    m_light.Specular.g = 1;
    m_light.Specular.b = 1;
   
    
    cls_tglo.p_d3dDevice->SetLight( i_d3d_light, &m_light );
    cls_tglo.p_d3dDevice->LightEnable( i_d3d_light, TRUE ); 

}

void CLightManager::UpdateLights()
{

    int i_cur_d3d_light = C_LIGHT_START;

    if (cls_tglo.p_d3dDevice)
    {
        
        //first turn off all d3d lights
        for (int l=C_LIGHT_START; l < C_LIGHT_END; l++)
        {
            
            cls_tglo.p_d3dDevice->LightEnable( l, FALSE );
            
        }
    }
    
	for (unsigned int i = 0; i < m_vec_light.size(); i++)
	{
		if (m_vec_light[i].b_active)
		{

			//distance check


			static D3DXVECTOR3 vec_temp;

			vec_temp = m_vec_light[i].vec_pos-cls_tglo.p_camera->m_vObjectPosition;

			//how far are we from the camera?
			if (D3DXVec3LengthSq(&vec_temp)
				< pow(m_vec_light[i].f_trigger_distance,2))
			{

				UpdateD3DLight(m_vec_light[i], i_cur_d3d_light);
				i_cur_d3d_light++;
				if (i_cur_d3d_light >= C_LIGHT_END)
				{
					log_msg("All d3d lights used up.");
					break;
				}
			}

		}

	}

}

void CLightManager::think()
{
    if (m_timer_update.interval_reached())
    {
        UpdateLights();
    }
}