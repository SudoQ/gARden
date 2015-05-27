/***********************************************************************
VegetationUpdateShader - Shader to update vegetation
***********************************************************************/

#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect waterSampler;

void updateVegetation(out vec4 fragColor, in vec2 fragCoord)
{
	float waterQ = 0.0;
	float hydration = 0.0;
	for(float i = -10.0; i < 10.0; i++){
			for(float j = -10.0; j < 10.0; j++){
				hydration = hydration + texture2DRect(waterSampler, vec2(uv.x+i, uv.y+j)).r;
			}
	}
	hydration = hydration/400.0; // Normalize
	
	float waterColor = texture2DRect(waterSampler, fragCoord.xy).r;

	if(waterColor < 0.0){
			waterColor = 0.0;
	}
	
	float vegetation = 0.0;

	// Start a linear increase of vegetation from start to top
	// and a linear decrease of vegetation from top to end.
	float start = 0.25;
	float top = 0.5; 
	float end = 0.75;
	
	float k1 = 1.0 / (top-start);
	float k2 = 1.0 / (top-end);
	float m1 = 1.0 - top*k1;
	float m2 = 1.0 - top*k2;
	
	if (hydration > start && hydration <= top){
			vegetation = k1 * hydration + m1;
	} else if (hydration > top && hydration <= end){
			vegetation = k2 * hydration + m2;
	}

	float b = waterColor;
  float g = vegetation;

	fragColor = vec4 ( 0.0, g, b, 0.0);
}
void main()
{
	updateVegetation(gl_FragColor, gl_FragCoord.xy);
}
