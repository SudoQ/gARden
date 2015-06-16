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
uniform sampler2DRect prevHydrationSampler;

void main()
	{
	float n = 0.0;
	float maxHydration = 0.0;
	// Search the surronding pixels for water
	int range = 80;
	int start = -1*(range/2);
	int end = range/2;
	int step = 1;
	for(int i=start; i<end; i+=step){
		for(int j=start; j<end; j+=step){
			n++;
			float deriv0 = texture2DRect(derivativeSampler, vec2(gl_FragCoord.x+i, gl_FragCoord.y+j)).r;
			float water = 0.0;
				
			// Water detection
			if (abs(deriv0) > 0.001){
				water = 1.0;
			}
			
			maxHydration += water;
		}
	}
	maxHydration = maxHydration/n; // The average water coverage
	float previousHydration = texture2DRect(prevHydrationSampler, gl_FragCoord).r;	
	//float previousHydration = gl_FragColor.r;
	float velocity = 0.001;
	float newHydration = previousHydration + (maxHydration - previousHydration)*velocity;
	
	gl_FragColor=vec4(newHydration, 0.0, 0.0, 0.0);
	}
