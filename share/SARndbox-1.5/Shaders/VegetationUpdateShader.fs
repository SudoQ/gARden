/***********************************************************************
VegetationUpdateShader - Shader to update vegetation
***********************************************************************/

#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect waterSampler;

void main()
	{
	/* Scan the surrounding areas for water amount */
	float amount = texture2DRect(waterSampler, gl_FragCoord.xy).r;

	/* Set the vegetation value */
	gl_FragColor=vec4(1 - amount, 0.0, 0.0, 0.0);
	}
