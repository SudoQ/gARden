/***********************************************************************
VegetationTable - Class to simulate vegetation.
***********************************************************************/

#ifndef VEGETATIONTABLE_INCLUDED
#define VEGETATIONTABLE_INCLUDED

/* Forward declarations: */
class SurfaceRenderer;

class VegetationTable:public GLObject
	{
		/* Embedded classes: */
		public:
			typedef double Scalar;
			typedef Geometry::Point<Scalar,3> Point;
			typedef Geometry::Plane<Scalar,3> Plane;
			typedef Geometry::Box<Scalar,3> Box;
			typedef Geometry::OrthonormalTransformation<Scalar,3> Transform;
		private:
			struct DataItem:public GLObject::DataItem // Structure holding per-context state
				{
					/* Elements: */
					public:
						GLuint vegetationTextureObject; // Color texture
						GLuint vegetationFramebufferObject; // Frame buffer used for the vegetation rendering step
						GLhandleARB vegetationShader; // Shader to render vegetation
						GLint vegetationShaderUniformLocations[1]; // FIXME Number of shader uniforms
					
						/* Constructors and destructors: */
						DataItem(void);
						virtual ~DataItem(void);
				};
		/* Elements */
		static const char* vertexShaderSource; // Source code of vertex shader shared by all shader programs
		GLsizei size[2]; // Width and height of vegetation table in pixels
		Transform baseTransform; // Transformation from camera space to upright elevation map space
		Box domain; // Domain of elevation map space in rotated camera space
		GLfloat cellSize[2]; // Width and height of vegetation table cells in world coordinate units

		/* Private methods: */
		// calcDerivative

		/* Constructors and destructors: */
		public:
		VegetationTable(GLsizei width, GLsizei height, const Plane& basePlane, const Point basePlaneCorners[4]);
		virtual ~VegetationTable(void);

		/* Methods from GLObject: */
		virtual void initContext(GLContextData& contextData) const;
		
		/* New methods: */
		// void setElevationRange(...);
		const Transform& getBaseTransform(void) const // Returns the transformation from camera space to upright elevation map space
			{
			return baseTransform;
			}
		const Box& getDomain(void) const // Returns the vegetation table's domain in rotated camera space
			{
			return domain;
			}
	};

#endif

