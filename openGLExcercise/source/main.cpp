#include <GL\glew.h>
#include <GL\wglew.h>

//GLEW includes must b e first!
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <soil\SOIL.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>

#include "player.h"

#define GLEW_STATIC

using namespace std;
using namespace glm;

GLuint CreateShader(GLenum a_ShaderType, const char* a_strShaderFile);

GLuint CreateProgram(const char* a_vertex, const char* a_frag);

float* getOrtho(float left, float right, float bottom, float top, float a_fNear, float a_fFar);

Player spaceShip;

int main()
{
	srand(time(nullptr));

	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make window's context current 
	glfwMakeContextCurrent(window);

	//start GLEW
	if (glewInit() != GLEW_OK)
	{
		//openGL didn't start shutdown GLFW and return error code
		glfwTerminate();
		return -1;
	}

	//Testing SOIL
	GLuint tex_2d = SOIL_load_OGL_texture(
		"img.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	//Check for error in laod process
	if (0 == tex_2d){
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	//Vertex Structure
	struct Vertex{
		float fPositions[4];
		float fColors[4];
	};

	struct Vertex2{
		vec4 positions;
		vec4 colors;
	};

	cout << spaceShip.origin.x << " " << spaceShip.origin.y << endl;
	cout << sizeof(vec4) << endl;
	cout << sizeof(Vertex) << endl;
	cout << sizeof(Vertex2) << endl;

	Vertex2* playerBuffer = new Vertex2[3];
	playerBuffer[0].positions = vec4(spaceShip.origin.x + spaceShip.vert1.x, spaceShip.origin.y + spaceShip.vert1.y, 0, 1.0f);
	playerBuffer[0].colors = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	playerBuffer[1].positions = vec4(spaceShip.origin.x + spaceShip.vert2.x, spaceShip.origin.y + spaceShip.vert2.y, 0, 1.0f);
	playerBuffer[1].colors = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	playerBuffer[2].positions = vec4(spaceShip.origin.x + spaceShip.vert3.x, spaceShip.origin.y + spaceShip.vert3.y, 0, 1.0f);
	playerBuffer[2].colors = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	

	//Create some vertices
	Vertex* myShape2 = new Vertex[3];
	myShape2[0].fPositions[0] = 100.0f;
	myShape2[0].fPositions[1] = 100.0f;
	myShape2[1].fPositions[0] = 200.0f;
	myShape2[1].fPositions[1] = 200.0f;
	myShape2[2].fPositions[0] = 300.0f;
	myShape2[2].fPositions[1] = 100.0f;
	for (int i = 0; i < 3; i++){
		myShape2[i].fPositions[2] = 0.0f;
		myShape2[i].fPositions[3] = 1.0f;
		myShape2[i].fColors[0] = 1.0f;
		myShape2[i].fColors[1] = 0.0f;
		myShape2[i].fColors[2] = 0.0f;
		myShape2[i].fColors[3] = 1.0f;
	}

	//create random 'star' vertices
	Vertex* myShape = new Vertex[100];
	for (int i = 0; i < 100; i++){
		myShape[i].fPositions[0] = rand() % 1024;
		myShape[i].fPositions[1] = rand() % 768;
		myShape[i].fPositions[2] = 0.0f;
		myShape[i].fPositions[3] = 1.0f;
		myShape[i].fColors[0] = 1.0f;
		myShape[i].fColors[1] = 1.0f;
		myShape[i].fColors[2] = 0.0f;
		myShape[i].fColors[3] = 1.0f;
	}

	//create ID for a vertex buffer object
	GLuint uiVBO;
	glGenBuffers(1, &uiVBO);


	//Create ID for indesx buffer object
	GLuint uiIBO;
	glGenBuffers(1, &uiIBO);

	//create ID for a vertex buffer object
	GLuint uiVBO2;
	glGenBuffers(1, &uiVBO2);


	//Create ID for indesx buffer object
	GLuint uiIBO2;
	glGenBuffers(1, &uiIBO2);

	//check it succeeded
	if (uiVBO != 0){
		//bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		//allocate space for the vertices on the graphics card
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 100, NULL, GL_STATIC_DRAW);
		//get pointer to allocated space on the graphics card
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to the graphics card
		memcpy(vBuffer, myShape, sizeof(Vertex)* 100);

		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//check it succeeded
	if (uiIBO != 0)
	{
		//bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		//allocate space for index info on the graphics card
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 100 * sizeof(char), NULL, GL_STATIC_DRAW);
		//get pointer to newly allocated space on the graphics card
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		//specify the order we'd like to draw our vertices.
		//In this case they are in sequential order
		for (int i = 0; i < 100; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		//unmap and unbind
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	//check it succeeded
	if (uiVBO2 != 0){
		//bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO2);
		//allocate space for the vertices on the graphics card
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2)* 3, NULL, GL_STATIC_DRAW);
		//get pointer to allocated space on the graphics card
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to the graphics card
		memcpy(vBuffer, playerBuffer, sizeof(Vertex2)* 3);

		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//check it succeeded
	if (uiIBO2 != 0)
	{
		//bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO2);
		//allocate space for index info on the graphics card
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(char), NULL, GL_STATIC_DRAW);
		//get pointer to newly allocated space on the graphics card
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		//specify the order we'd like to draw our vertices.
		//In this case they are in sequential order
		for (int i = 0; i < 3; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		//unmap and unbind
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	//create shader program
	GLuint programFlat = CreateProgram(".\\source\\VertexShader.glsl", ".\\source\\FlatFragmentShader.glsl");

	//find the position of the matrix variable int the shader program
	GLuint IDFlat = glGetUniformLocation(programFlat, "MVP");

	//set up mapping to the screen to pixel coordinates
	float* orthographicProjection = getOrtho(0, 1024, 0, 720, 0, 100);
	//Matrix4::GetOrthographicProjection(0, 1024, 0, 720, 0, 100).Get(orthographicProjection);


	glPointSize(5);

	//loop until user closes the window
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		////enable shaders
		glUseProgram(programFlat);

		//send ortho projection info to shader
		glUniformMatrix4fv(IDFlat, 1, GL_FALSE, orthographicProjection);	

		////specify where vertex array is
		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vertexPositions);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vertexColors);

		////draw code here
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

		//vbo draw

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);

		//enable the vertex array states
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		/*Since the data is in the same array, we need to specify the gap between
		vertices (A whole Vertex structure instance) and the offset of the data
		from the beginning of the structure instance. The positions are at the
		start, so their offset is 0. But the colours are after the positions, so
		they are offset by the size of the position data */
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));

		//draw to the screen
		//glDrawArrays(GL_POINTS, 0, 3);
		glDrawElements(GL_POINTS, 100, GL_UNSIGNED_BYTE, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, uiVBO2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO2);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)(sizeof(float)* 4));

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, NULL);


		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//swap front and back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}

	glDeleteBuffers(1, &uiIBO);

	glfwTerminate();
	return 0;
}


GLuint CreateShader(GLenum a_ShaderType, const char* a_ShaderFile)
{
	std::string shaderCode;
	//open shader file
	std::ifstream shaderStream(a_ShaderFile);
	//if that worked ok, load file line by line
	if (shaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(shaderStream, line))
		{
			shaderCode += "\n" + line;
		}
		shaderStream.close();
	}

	//convert to cstring
	char const* shaderSourcePointer = shaderCode.c_str();

	//create shader ID
	GLuint shader = glCreateShader(a_ShaderType);
	//load source code
	glShaderSource(shader, 1, &shaderSourcePointer, NULL);

	//compile shader
	glCompileShader(shader);

	//check for errors
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

		const char* shaderType = NULL;
		switch (a_ShaderType)
		{
		case GL_VERTEX_SHADER:
			shaderType = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "fragment";
			break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", shaderType, infoLog);
		delete[] infoLog;
	}

	return shader;

}

GLuint CreateProgram(const char* a_vertex, const char* a_frag)
{
	//ofstream file;
	//file.open(a_vertex, ios::in);
	//cout << file.is_open() << endl;

	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, a_vertex));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, a_frag));

	//create shader program ID
	GLuint program = glCreateProgram();

	//attach shaders
	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glAttachShader(program, *shader);
	}

	//link program
	glLinkProgram(program);

	//check for link errors and output them
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);

		fprintf(stderr, "Linker failure: %s\n", infoLog);
		delete[] infoLog;
	}

	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glDetachShader(program, *shader);
		glDeleteShader(*shader);
	}
	return program;
}

float* getOrtho(float left, float right, float bottom, float top, float a_fNear, float a_fFar)
{
	//to correspond with mat4 in the shader
	//ideally this function would be part of your matrix class
	//however I wasn't willing to write your matrix class for you just to show you this
	//so here we are in array format!
	//add this to your matrix class as a challenge if you like!
	float* toReturn = new float[12];
	toReturn[0] = 2.0 / (right - left);;
	toReturn[1] = toReturn[2] = toReturn[3] = toReturn[4] = 0;
	toReturn[5] = 2.0 / (top - bottom);
	toReturn[6] = toReturn[7] = toReturn[8] = toReturn[9] = 0;
	toReturn[10] = 2.0 / (a_fFar - a_fNear);
	toReturn[11] = 0;
	toReturn[12] = -1 * ((right + left) / (right - left));
	toReturn[13] = -1 * ((top + bottom) / (top - bottom));
	toReturn[14] = -1 * ((a_fFar + a_fNear) / (a_fFar - a_fNear));
	toReturn[15] = 1;
	return toReturn;
}