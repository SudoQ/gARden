/***********************************************************************
VegetationUpdateShader - Shader to update the current vegetation level
The vegetation value is calculated by applying two linear functions to
simulate vegetation growth and decay.
Copyright (c) 2015 Simon Johansson

This file is part of the Vegetation Augmented Reality Sandbox (gARden).

This is a fork of the Augmented Reality Sandbox (SARndbox)
Copyright (c) 2012 Oliver Kreylos

The Augmented Reality Sandbox is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The Augmented Reality Sandbox is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with the Augmented Reality Sandbox; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
***********************************************************************/

#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect hydrationSampler;
uniform float minHydration;
uniform float maxHydration;

void main()
	{
	float hydration=texture2DRect(hydrationSampler, gl_FragCoord.xy).r;
	float vegetation = 0.0;
	
	float midHydration = ((maxHydration-minHydration)/2.0)+minHydration;

	// Linear function paramters, f(x) = k*x + m
	float k1 = 1.0/(midHydration-minHydration);
	float k2 = 1.0/(midHydration-maxHydration);
	float m1 = 1.0 - midHydration*k1;
	float m2 = 1.0 - midHydration*k2;
	
	if (hydration >= minHydration && hydration <= midHydration){
		vegetation = k1 * hydration + m1;
	} else if (hydration > midHydration && hydration <= maxHydration){
		vegetation = k2 * hydration + m2;
	}
	
	gl_FragColor=vec4(vegetation,0.0,0.0,0.0);
	}
