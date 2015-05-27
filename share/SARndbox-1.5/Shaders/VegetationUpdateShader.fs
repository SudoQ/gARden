/***********************************************************************
VegetationUpdateShader - Shader to update vegetation
***********************************************************************/

#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect waterSampler;

void updateVegetation(out vec4 fragColor, in vec2 fragCoord)
{
	float hydration = 0.0;

	// Calculate the amount of water in a 20x20 pixel area
	for(float i = -10.0; i < 10.0; i++){
			for(float j = -10.0; j < 10.0; j++){
				hydration = hydration + texture2DRect(waterSampler, vec2(fragCoord.x+i, fragCoord.y+j)).r;
			}
	}
	hydration = hydration/400.0; // Normalize
	
	// Start a linear increase of vegetation from start to top
	// and a linear decrease of vegetation from top to end.
	// TODO Change to two S-curve functions.
	float start = 0.25;
	float top = 0.5; 
	float end = 0.75;
	
	float k1 = 1.0 / (top-start);
	float k2 = 1.0 / (top-end);
	float m1 = 1.0 - top*k1;
	float m2 = 1.0 - top*k2;
	
	float vegetation = 0.0;
	if (hydration > start && hydration <= top){
			vegetation = k1 * hydration + m1;
	} else if (hydration > top && hydration <= end){
			vegetation = k2 * hydration + m2;
	}

	float waterColor = texture2DRect(waterSampler, fragCoord.xy).r;
	float b = waterColor;
  float g = vegetation;

	fragColor = vec4(g);
}
void main()
{
	updateVegetation(gl_FragColor, gl_FragCoord.xy);
}
