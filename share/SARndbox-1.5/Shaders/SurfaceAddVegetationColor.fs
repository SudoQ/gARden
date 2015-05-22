/***********************************************************************
SurfaceAddWaterColor - Shader fragment to modify the base color depening
of the vegetation texture.
***********************************************************************/
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect vegetationSampler;

varying vec2 vegetationTexCoord; // Texture coordinate for vegetation texture

void addVegetationColor(in vec2 fragCoord, inout vec4 baseColor)
	{
	// Get value from sampler and change baseColor.
	float value = texture2DRect(vegetationSampler, vegetationTexCoord).r;
	baseColor = vec4(0.0, value, 0.0, 1.0);
	}
