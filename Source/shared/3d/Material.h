/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#ifndef MATERIAL_H
#define MATERIAL_H

#include "D3Dx8.h"

class Material  
{
public:
	Material();
	virtual ~Material();

	HRESULT Use(LPDIRECT3DDEVICE8 device);
	HRESULT SetPower(float power);
	HRESULT SetEmissive(float red, float green, float blue, float alpha);
	HRESULT SetSpecular(float red, float green, float blue, float alpha);
	HRESULT SetAmbient(float red, float green, float blue, float alpha);
	HRESULT SetDiffuse(float red, float green, float blue, float alpha);

//protected:
	D3DMATERIAL8 m_mtrlObject;
};

#endif // MATERIAL_H
