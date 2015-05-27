/* Headers */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <math.h>

std::string readFile(const char* path){
	std::ifstream infile(path);
	std::string str((std::istreambuf_iterator<char>(infile)),
													(std::istreambuf_iterator<char>()));
	return str;
}

void compileShader(const char* shaderSrc, GLuint& shader, GLuint shaderType){
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE){
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		printf(buffer);
	}
}

void createShaderProgram(
		const char* vertSrc, 
		const char* fragSrc, 
		GLuint& vertexShader, 
		GLuint& fragmentShader, 
		GLuint& shaderProgram)
{
	compileShader(vertSrc, vertexShader, GL_VERTEX_SHADER);
	compileShader(fragSrc, fragmentShader, GL_FRAGMENT_SHADER);

	// Link the vertex and fragment shader into a shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
}

int main(int argc, char** argv){
	if (argc != 3){
		fprintf(stderr, "Usage:\nglslValidator [vert|frag] path/to/shader\n");
		return 0;
	}

	GLFWwindow* window;

	///* Initialize the library */
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

	///* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Initialize GLEW */
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		return -1;
	}
	GLuint shaderType;	
	if (std::string("frag").compare(argv[1])==0){
		printf("Compiling fragment shader %s\n",argv[2]);
		shaderType = GL_FRAGMENT_SHADER;
	} else {
		printf("Compiling vertex shader %s\n",argv[2]);
		shaderType = GL_VERTEX_SHADER;
	}
	std::string shaderSrc = readFile(argv[2]);
	GLuint shader;
	compileShader(shaderSrc.c_str(), shader, shaderType);

	glfwTerminate();
	return 0;
}
