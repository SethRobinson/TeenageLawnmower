/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "Material.h"


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/******************************************************************************
	Construction/Destruction
 *****************************************************************************/

Material::Material()
{
	SetDiffuse(1.0f, 1.0f, 1.0f,1.0f);
	SetAmbient(0.5f, 0.5f, 0.5f,1.0f);
	SetSpecular(0.0f,0.0f,0.0f,1.0f);
	SetEmissive(0.0f,0.0f,0.0f,1.0f);
	SetPower(1.0f);
}

Material::~Material()
{

}

/******************************************************************************
	Name:   SetDiffuse
	Desc:   Sets the Diffuse colour
	Param1:	float red   - The Red channel.
	Param2: float green - The Green channel.
	Param3: float blue  - The Blue channel.
	Param4: float alpha - The Alpha channel.
 ******************************************************************************/
HRESULT Material::SetDiffuse(float red, float green, float blue, float alpha)
{
	m_mtrlObject.Diffuse.r = red;
	m_mtrlObject.Diffuse.g = green;
	m_mtrlObject.Diffuse.b = blue;
	m_mtrlObject.Diffuse.a = alpha;

	return S_OK;
}

/******************************************************************************
	Name:   SetAmbient
	Desc:   Sets the Ambience of the material.
	Param1:	float red   - The Red channel.
	Param2: float green - The Green channel.
	Param3: float blue  - The Blue channel.
	Param4: float alpha - The Alpha channel.
 ******************************************************************************/
HRESULT Material::SetAmbient(float red, float green, float blue, float alpha)
{
	m_mtrlObject.Ambient.r = red;
	m_mtrlObject.Ambient.g = green;
	m_mtrlObject.Ambient.b = blue;
	m_mtrlObject.Ambient.a = alpha;

	return S_OK;
}

/******************************************************************************
	Name:   SetSpecular
	Desc:   Sets the specularity (shinyness) of the material.
	Param1:	float red   - The Red channel.
	Param2: float green - The Green channel.
	Param3: float blue  - The Blue channel.
	Param4: float alpha - The Alpha channel.
 ******************************************************************************/
HRESULT Material::SetSpecular(float red, float green, float blue, float alpha)
{
	m_mtrlObject.Specular.r = red;
	m_mtrlObject.Specular.g = green;
	m_mtrlObject.Specular.b = blue;
	m_mtrlObject.Specular.a = alpha;

	return S_OK;
}

/******************************************************************************
	Name:   SetEmissive
	Desc:   Sets the Emissive level of the material.
	Param1:	float red   - The Red channel.
	Param2: float green - The Green channel.
	Param3: float blue  - The Blue channel.
	Param4: float alpha - The Alpha channel.
 ******************************************************************************/
HRESULT Material::SetEmissive(float red, float green, float blue, float alpha)
{
	m_mtrlObject.Emissive.r = red;
	m_mtrlObject.Emissive.g = green;
	m_mtrlObject.Emissive.b = blue;
	m_mtrlObject.Emissive.a = alpha;

	return S_OK;
}

/******************************************************************************
	Name:   SetPower
	Desc:   Sets the Brightness of the colour
	Param1:	float power - Brightness
 ******************************************************************************/
HRESULT Material::SetPower( float power )
{
	m_mtrlObject.Power = power;
	return S_OK;
}

/******************************************************************************
	Name:   Use
	Desc:   Assigns the Material to an object
	Param1:	LPDIRECT3DDEVICE8 device - 
 ******************************************************************************/
HRESULT Material::Use( LPDIRECT3DDEVICE8 pD3Ddevice )
{
	pD3Ddevice->SetMaterial(&m_mtrlObject);

	return S_OK;
}
