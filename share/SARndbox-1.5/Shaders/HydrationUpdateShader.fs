/***********************************************************************
HydrationUpdateShader - Shader to update the current hydration level.
The hydration for each cell is determined by calcluating the water 
ratio of nearby cells. 
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

uniform sampler2DRect derivativeSampler;
uniform sampler2DRect prevHydrationSampler;
uniform float hydrationRange;
uniform float detectionThreshold;
uniform float hydrationVelocity;
uniform float hydrationStepSize;

void main()
	{
	float n = 0.0;
	float waterCoverage = 0.0;
	float range = hydrationRange;
	float start = -1.0*(range/2.0);
	float end = range/2.0;
	float step = hydrationStepSize;

	// Search nearby cells for water
	for(float i=start; i<end; i+=step){
		for(float j=start; j<end; j+=step){
			n++;
			// Detect water by looking at the water derivative texture
			float deriv0 = texture2DRect(derivativeSampler, vec2(gl_FragCoord.x+i, gl_FragCoord.y+j)).r;
			float water = 0.0;
			if (abs(deriv0) > detectionThreshold){
				water = 1.0;
			}
			
			waterCoverage += water;
		}
	}
	waterCoverage = waterCoverage/n; // The average water coverage

	// Take a share of the previous hydration value to simulate vegetation growth/decay over time
	float previousHydration = texture2DRect(prevHydrationSampler, gl_FragCoord.xy).r;	
	float velocity = hydrationVelocity;
	float newHydration = previousHydration + (waterCoverage - previousHydration)*velocity;
	
	gl_FragColor=vec4(newHydration, 0.0, 0.0, 0.0);
	}
