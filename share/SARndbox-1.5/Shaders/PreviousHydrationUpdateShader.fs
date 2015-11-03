/***********************************************************************
PreviousHydrationUpdateShader - Shader to update the previous hydration
level.
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

void main()
	{
	gl_FragColor = vec4(texture2DRect(hydrationSampler, gl_FragCoord.xy).r,0.0,0.0,0.0);
	}
