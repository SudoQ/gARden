/***********************************************************************
SurfaceAddWaterColor - Shader fragment to modify the base color depening
of the vegetation texture.
***********************************************************************/
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect vegetationSampler;

void addVegetationColor(in vec2 fragCoord, inout vec4 baseColor)
	{
	// Get value from sampler and change baseColor.
	float value = texture2DRect(vegetationSampler, fragCoord.xy).r;
	baseColor = vec4(0.0, float, 0.0, 0.0);
	}
