/***********************************************************************
Water2EulerStepShader - Shader to perform an Euler integration step.
Copyright (c) 2012 Oliver Kreylos

This file is part of the Augmented Reality Sandbox (SARndbox).

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

void main()
	{
	float hydration=texture2DRect(hydrationSampler, gl_FragCoord.xy).r;
	// Hydration to vegetation value
	float vegetation = 0.0;
	float growth = 0.1;
	float decay = 0.9;
	//float top = ((decay-growth)/2.0)+growth;
	float top = 0.5;

	float k1 = 1.0/(top-growth);
	float k2 = 1.0/(top-decay);
	float m1 = 1.0 - top*k1;
	float m2 = 1.0 - top*k2;

	if (hydration > 0.99) {
		vegetation = 1.0;
	}

	if (hydration > growth && hydration < top){
		vegetation = k1 * hydration + m1;
	} else if (hydration > top && hydration < decay){
		vegetation = k2 * hydration + m2;
	}
	//if (hydration > 0.1 && hydration < 0.5){
	//	vegetation = 0.5;
	//} else if (hydration > 0.5 && hydration < 0.9){
	//	vegetation = 1.0;
	//}
	
	gl_FragColor=vec4(vegetation,0.0,0.0,1.0);
	}
