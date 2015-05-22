/* Headers */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <math.h>
#include <FreeImage.h>

// Shader macro
#define GLSL(src) "#version 150 core\n#extension GL_ARB_texture_rectangle : enable\n" #src

// Vertex shader
const GLchar* vertexShaderSrc = GLSL(
	in vec2 position;

	in vec2 texcoord;
	out vec2 Texcoord;

	void main(){
		Texcoord = texcoord;
		gl_Position = vec4(position.x, position.y, 0.0, 1.0);
	}
);

const GLchar* fragmentShaderSrc = GLSL(
	//uniform sampler2DRect vegetationSampler;

	in vec2 Texcoord;
	out vec4 outColor;
	uniform sampler2D tex;
	
	void main(){
		float v = 1-texture(tex, Texcoord).b;
		outColor = vec4(0.0, 0.0, v, 1.0);
	}
);

std::string readFile(const char* path){
	std::ifstream infile(path);
	std::string str((std::istreambuf_iterator<char>(infile)),
													(std::istreambuf_iterator<char>()));
	return str;
}

void createShaderProgram(
		const char* vertSrc, 
		const char* fragSrc, 
		GLuint& vertexShader, 
		GLuint& fragmentShader, 
		GLuint& shaderProgram)
{
	// Create and compile the vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE){
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf(buffer);
	}

	// Create and compile the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE){
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf(buffer);
	}

	// Link the vertex and fragment shader into a shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
}

void specifyVertexAttributes(GLuint shaderProgram)
{
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 
												4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
}

GLuint loadTexture(const GLchar* path)
{
	
	GLuint texture;
	glGenTextures(1, &texture);

	FIBITMAP *pImage;
	FIBITMAP *bitmap;
	int nWidth, nHeight;

	glBindTexture(GL_TEXTURE_2D, texture);
	//glBindTexture(GL_TEXTURE_RECTANGLE_ARB,0);
	bitmap = FreeImage_Load(
			FreeImage_GetFileType(path,0), path);
	pImage = FreeImage_ConvertTo32Bits(bitmap);
	nWidth = FreeImage_GetWidth(pImage);
	nHeight = FreeImage_GetHeight(pImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 
			0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
	FreeImage_Unload(pImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
	
	/*
	GLuint texture;
	glGenTextures(1, &texture);

	FIBITMAP *pImage;
	FIBITMAP *bitmap;
	int nWidth, nHeight;

	bitmap = FreeImage_Load(
			FreeImage_GetFileType(path,0), path);
	pImage = FreeImage_ConvertTo32Bits(bitmap);
	nWidth = FreeImage_GetWidth(pImage);
	nHeight = FreeImage_GetHeight(pImage);
	
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,texture);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,GL_LUMINANCE32F_ARB,nWidth,nHeight,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,0);
	FreeImage_Unload(pImage);

	return texture;
	*/
};

int main(void){
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()){
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800,600,"OpenGL", NULL, NULL); // Windowed

	if (!window){
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Initialize GLEW */
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		return -1;
	}

	//GLuint vertexBuffer;
	//glGenBuffers(1, &vertexBuffer);
	//printf("%u\n", vertexBuffer);

	/* Create a Vertex Array Object (VAO) */
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer

	GLfloat vertices[] = {
	//  Position      Texcoords
			-0.5f,  0.5f, 0.0f, 1.0f, // Top-left
			 0.5f,  0.5f, 1.0f, 1.0f, // Top-right
			 0.5f, -0.5f, 1.0f, 0.0f, // Bottom-right
			-0.5f, -0.5f, 0.0f, 0.0f  // Bottom-left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 
			sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an Element Buffer Object and copy the element order into it
	GLuint ebo;
	glGenBuffers(1, &ebo); // Generate 1 buffer
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			sizeof(elements), elements, GL_STATIC_DRAW);
	
	GLuint vertexShader, fragmentShader, shaderProgram;
	createShaderProgram(
			vertexShaderSrc, 
			fragmentShaderSrc, 
			vertexShader,
			fragmentShader,
			shaderProgram);
	glUseProgram(shaderProgram);

	specifyVertexAttributes(shaderProgram);

	//glActiveTextureARB(GL_TEXTURE0_ARB);
	GLuint tex = loadTexture("Textures/water.png");
	//glUniform1iARB(glGetUniformLocationARB(shaderProgram, "tex"), 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)){
		/* Render here */
		
		// Clear the screen to black
		glClearColor(0.0f,0.0f,0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteTextures(1, &tex);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
	return 0;
}
