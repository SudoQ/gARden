#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect depthSampler; // Sampler for the depth image-space elevation texture
uniform mat4 depthProjection; // Transformation from depth image space to camera space
uniform vec4 basePlane; // Plane equation of the base plane
uniform vec2 heightColorMapTransformation; // Transformation from elevation to height color map texture coordinate";
uniform mat4 waterLevelTextureTransformation; // Transformation from camera space to water level texture coordinate space";
uniform mat4 vegetationTextureTransformation; // Transformation from camera space to vegetation texture coordinate space";
varying float heightColorMapTexCoord; // Texture coordinate for the height color map";
varying vec2 waterLevelTexCoord; // Texture coordinate for water level texture";
varying vec2 vegetationTexCoord; // Texture coordinate for vegetation texture";
void main()
	{
	/* Get the vertex' depth image-space z coordinate from the texture: */
	vec4 vertexDic=gl_Vertex;
	vertexDic.z=texture2DRect(depthSampler,vertexDic.xy).r;
	/* Transform the vertex from depth image space to camera space: */
	vec4 vertexCc=depthProjection*vertexDic;
	/* Plug camera-space vertex into the base plane equation: */
	float elevation=dot(basePlane,vertexCc)/vertexCc.w;
	
	/* Transform elevation to height color map texture coordinate: */
	heightColorMapTexCoord=elevation*heightColorMapTransformation.x+heightColorMapTransformation.y;
	vec4 wltc=waterLevelTextureTransformation*vertexCc;
	waterLevelTexCoord=wltc.xy/wltc.w;
	//Transform the vertex from camera space to vegetation texture coordinate space: 
	vec4 vtc=vegetationTextureTransformation*vertexCc;
	vegetationTexCoord=vtc.xy/vtc.w;
	/* Transform vertex to clip coordinates: */
	gl_Position=gl_ModelViewProjectionMatrix*vertexCc;
}
