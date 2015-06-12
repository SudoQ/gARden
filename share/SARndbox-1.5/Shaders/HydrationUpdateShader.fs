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

uniform sampler2DRect derivativeSampler;

void main()
	{
	//vec3 qt=texture2DRect(derivativeSampler,gl_FragCoord.xy).rgb;
	
	float n = 0.0;
	float hydration = 0.0;
	float range = 0.1;
	float start = -1*(range/2.0);
	float end = range/2.0;
	float steps = 10.0;
	float step = range/steps;

	// Search the surronding pixels for water
	for(float i=start; i<end; i+=step){
		for(float j=start; j<end; j+=step){
			n++;
			float deriv0 = texture2DRect(derivativeSampler, gl_FragCoord.xy).r;
			float water = 0.0;
				
			// Water detection
			if (abs(deriv0) > 0.01){
				water = 1.0;
			}
			
			hydration += water;
		}
	}
	hydration = hydration/n; // The average water coverage
	//float currentHydration = gl_FragColor.r;
	//float velocity = 0.1;
	//hydration = velocity*hydration + (1.0-velocity)*currentHydration;
	//gl_FragColor=vec4(hydration,0.0,0.0,1.0);
	gl_FragColor=vec4(hydration,0.0,0.0,0.0);
	}
