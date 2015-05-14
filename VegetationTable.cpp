/***********************************************************************
VegetationTable - Class to simulate vegetation.
***********************************************************************/

#include "VegetationTable.h"

//includes

#include "SurfaceRenderer.h"

namespace {
/****************
Helper functions:
****************/

GLfloat* makeBuffer(int width,int height,int numComponents,...)
	{
	va_list ap;
	va_start(ap,numComponents);
	GLfloat fill[4];
	for(int i=0;i<numComponents&&i<4;++i)
		fill[i]=GLfloat(va_arg(ap,double));
	va_end(ap);
	
	GLfloat* buffer=new GLfloat[height*width*numComponents];
	GLfloat* bPtr=buffer;
	for(int y=0;y<height;++y)
		for(int x=0;x<width;++x,bPtr+=numComponents)
			for(int i=0;i<numComponents;++i)
				bPtr[i]=fill[i];
	
	return buffer;
	}

GLhandleARB compileVertexShader(const char* shaderFileName)
	{
	/* Construct the full shader source file name: */
	std::string fullShaderFileName=SHADERDIR;
	fullShaderFileName.push_back('/');
	fullShaderFileName.append(shaderFileName);
	fullShaderFileName.append(".vs");
	
	/* Compile and return the vertex shader: */
	return glCompileVertexShaderFromFile(fullShaderFileName.c_str());
	}

GLhandleARB compileFragmentShader(const char* shaderFileName)
	{
	/* Construct the full shader source file name: */
	std::string fullShaderFileName=SHADERDIR;
	fullShaderFileName.push_back('/');
	fullShaderFileName.append(shaderFileName);
	fullShaderFileName.append(".fs");
	
	/* Compile and return the fragment shader: */
	return glCompileFragmentShaderFromFile(fullShaderFileName.c_str());
	}
}

/**************************************
Methods of class WaterTable2::DataItem:
**************************************/
VegetationTable::DataItem::DataItem(void)
	{
	/* Check for and initialize all required OpenGL extensions: */
	}

VegetationTable::DataItem::~DataItem(void)
	{
	// glDeleteObjectARB(...)
	}

/************************************
Static elements of class WaterTable2:
************************************/

const char* WaterTable2::vertexShaderSource="\
	void main()\n\
		{\n\
		/* Use standard vertex position: */\n\
		gl_Position=ftransform();\n\
		}\n";

/****************************
Methods of class WaterTable2:
****************************/

// calcDerivative

VegetationTable::VegetationTable(
		GLsizei width, 
		GLsizei height, 
		const Plane& basePlane,
		const Point basePlaneCorners[4])
	{
	/* Initialize the water table size: */
	size[0]=width;
	size[1]=height;

	/* Project the corner points to the base plane and calculate their centroid: */
	Point bpc[4];
	Point::AffineCombiner centroidC;
	for(int i=0;i<4;++i)
		{
		bpc[i]=basePlane.project(basePlaneCorners[i]);
		centroidC.addPoint(bpc[i]);
		}
	Point baseCentroid=centroidC.getPoint();
	
	/* Calculate the transformation from camera space to upright elevation model space: */
	typedef Point::Vector Vector;
	Vector z=basePlane.getNormal();
	Vector x=(bpc[1]-bpc[0])+(bpc[3]-bpc[2]);
	Vector y=Geometry::cross(z,x);
	baseTransform=Transform::translateFromOriginTo(baseCentroid);
	baseTransform*=Transform::rotate(Transform::Rotation::fromBaseVectors(x,y));
	baseTransform.doInvert();
	
	/* Calculate the domain of upright elevation model space: */
	domain=Box::empty;
	for(int i=0;i<4;++i)
		domain.addPoint(baseTransform.transform(bpc[i]));
	domain.min[2]=Scalar(-20);
	domain.max[2]=Scalar(100);
	
	/* Calculate the grid's cell size: */
	for(int i=0;i<2;++i)
		cellSize[i]=GLfloat((domain.max[i]-domain.min[i])/Scalar(size[i]));
	std::cout<<cellSize[0]<<" x "<<cellSize[1]<<std::endl;

	/* Initialize simulations parameters */
	// ...
	}

VegetationTable::~VegetationTable(void)
	{
	}

void VegetationTable::initContext(GLContextData& contextData) const
	{
	DataItem* dataItem=new DataItem;
	contextData.addDataItem(this,dataItem);
	
	glActiveTextureARB(GL_TEXTURE0_ARB);
	
	/* Create the vertex-centered bathymetry textures, replacing the outermost layer of cells with ghost cells: */
	{
	/* Create the cell-centered water texture: */
	glGenTextures(1,&dataItem->waterTextureObject);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,dataItem->waterTextureObject);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP);
	GLfloat* w=makeBuffer(size[0],size[1],1,0.0);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,GL_R32F,size[0],size[1],0,GL_LUMINANCE,GL_FLOAT,w);
	delete[] w;
	}

	/* Protect the newly-created textures: */
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,0);

	/* Save the currently bound frame buffer: */
	GLint currentFrameBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&currentFrameBuffer);

	/* Create the water frame buffer: */
	glGenFramebuffersEXT(1,&dataItem->waterFramebufferObject);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,dataItem->waterFramebufferObject);
	
	/* Attach the water texture to the water frame buffer: */
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_RECTANGLE_ARB,dataItem->waterTextureObject,0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadBuffer(GL_NONE);
	}
	
	/* Restore the previously bound frame buffer: */
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,currentFrameBuffer);
	}

	/* Create the water shader: */
	{
	GLhandleARB vertexShader=glCompileVertexShaderFromString(vertexShaderSource);
	GLhandleARB fragmentShader=compileFragmentShader("Water2WaterUpdateShader");
	dataItem->waterShader=glLinkShader(vertexShader,fragmentShader);
	glDeleteObjectARB(vertexShader);
	glDeleteObjectARB(fragmentShader);
	dataItem->waterShaderUniformLocations[0]=glGetUniformLocationARB(dataItem->waterShader,"bathymetrySampler");
	dataItem->waterShaderUniformLocations[1]=glGetUniformLocationARB(dataItem->waterShader,"quantitySampler");
	dataItem->waterShaderUniformLocations[2]=glGetUniformLocationARB(dataItem->waterShader,"waterSampler");
	}
}

