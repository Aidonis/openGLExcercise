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

#include "entity.h"
#include "player.h"

#define GLEW_STATIC

using namespace std;
using namespace glm;

GLuint CreateShader(GLenum a_ShaderType, const char* a_strShaderFile);

GLuint CreateProgram(const char* a_vertex, const char* a_frag);

unsigned int loadTexture(const char* a_filename, int& a_width, int& a_height, int& a_bpp);

float* getOrtho(float left, float right, float bottom, float top, float a_fNear, float a_fFar);

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

	//Vertex Structure
	struct Vertex{
		float fPositions[4];
		float fColors[4];
		float fUVs[2];
	};

	struct Vertex2{
		vec4 positions;
		vec4 colors;
		vec2 fUVs;
	};

	//load and setup texture ID
	int width = 32, height = 32, bpp = 4;
	GLuint textureID = loadTexture(".\\assets\\dirt.png", width, height, bpp);

	std::cout << "Texture : " << textureID << endl;

	//Initialize player
	Player spaceShip;
	spaceShip.Initialize(vec4(1024 / 2.0, 720 / 2.0, 0, 0), vec4(1, 0, 1, 1), textureID);

	//cout << spaceShip.origin.x << " " << spaceShip.origin.y << endl;
	//cout << sizeof(vec4) << endl;
	//cout << sizeof(vec2) << endl;
	//cout << sizeof(Vertex) << endl;
	//cout << sizeof(Vertex2) << endl;

	//Vertex2* playerBuffer = new Vertex2[3];
	//playerBuffer[0].positions = vec4(spaceShip.origin.x + spaceShip.vert1.x, spaceShip.origin.y + spaceShip.vert1.y, 0, 1.0f);
	//playerBuffer[0].colors = vec4(1.0f, 1.0f, 1.0f, 0.0f);
	//playerBuffer[1].positions = vec4(spaceShip.origin.x + spaceShip.vert2.x, spaceShip.origin.y + spaceShip.vert2.y, 0, 1.0f);
	//playerBuffer[1].colors = vec4(1.0f, 0.0f, 1.0f, 0.0f);
	//playerBuffer[2].positions = vec4(spaceShip.origin.x + spaceShip.vert3.x, spaceShip.origin.y + spaceShip.vert3.y, 0, 1.0f);
	//playerBuffer[2].colors = vec4(1.0f, 0.0f, 1.0f, 0.0f);
	//playerBuffer[0].fUVs = vec2(0.5f, 1.0f);//top of triangle
	//playerBuffer[1].fUVs = vec2(0.2f, 0.5f);//bottom left
	//playerBuffer[2].fUVs = vec2(0.7f,0.5f);//bottom right

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
		myShape[i].fColors[3] = 0.0f;
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

	////check it succeeded
	//if (uiVBO2 != 0){
	//	//bind VBO
	//	glBindBuffer(GL_ARRAY_BUFFER, uiVBO2);
	//	//allocate space for the vertices on the graphics card
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2)* 3, NULL, GL_STATIC_DRAW);
	//	//get pointer to allocated space on the graphics card
	//	GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	//	//copy data to the graphics card
	//	memcpy(vBuffer, playerBuffer, sizeof(Vertex2)* 3);

	//	//unmap and unbind buffer
	//	glUnmapBuffer(GL_ARRAY_BUFFER);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//}

	////check it succeeded
	//if (uiIBO2 != 0)
	//{
	//	//bind IBO
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO2);
	//	//allocate space for index info on the graphics card
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(char), NULL, GL_STATIC_DRAW);
	//	//get pointer to newly allocated space on the graphics card
	//	GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	//	//specify the order we'd like to draw our vertices.
	//	//In this case they are in sequential order
	//	for (int i = 0; i < 3; i++)
	//	{
	//		((char*)iBuffer)[i] = i;
	//	}
	//	//unmap and unbind
	//	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//}


	//create shader program
	GLuint programFlat = CreateProgram(".\\source\\VertexShader.glsl", ".\\source\\FlatFragmentShader.glsl");

	//create textured shader program
	GLuint programTexture = CreateProgram(".\\source\\VertexShader.glsl", ".\\source\\TexturedFragmentShader.glsl");


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

		//enable the vertex array states
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);


		//bind buffer / texture
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);

		/*Since the data is in the same array, we need to specify the gap between
		vertices (A whole Vertex structure instance) and the offset of the data
		from the beginning of the structure instance. The positions are at the
		start, so their offset is 0. But the colours are after the positions, so
		they are offset by the size of the position data */
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

		//draw to the screen
		//glDrawArrays(GL_POINTS, 0, 3);
		glDrawElements(GL_POINTS, 100, GL_UNSIGNED_BYTE, NULL);

		glUseProgram(programTexture);
		//send ortho projection info to shader
		glUniformMatrix4fv(IDFlat, 1, GL_FALSE, orthographicProjection);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO2);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)(sizeof(vec4)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)(sizeof(vec4)* 2));

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, NULL);

		//Movement Shits
		//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		//	//move forward
		//	for (int i = 0; i < 3; i++){
		//		playerBuffer[i].positions.y += 1.0f;
		//		if (playerBuffer[i].positions.y >= 720){
		//			playerBuffer[i].positions.y = 0;
		//		}
		//	}
		//	glBindBuffer(GL_ARRAY_BUFFER, uiVBO2);
		//	GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//	//copy to card
		//	memcpy(vBuffer, playerBuffer, sizeof(Vertex2)* 3);
		//	//unmap and unbind
		//	glUnmapBuffer(GL_ARRAY_BUFFER);
		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		//}

		//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		//	//move forward
		//	for (int i = 0; i < 3; i++){
		//		playerBuffer[i].positions.y -= 1.0f;
		//		if (playerBuffer[i].positions.y <= 0){
		//			playerBuffer[i].positions.y = 720;
		//		}
		//	}
		//	glBindBuffer(GL_ARRAY_BUFFER, uiVBO2);
		//	GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//	//copy to card
		//	memcpy(vBuffer, playerBuffer, sizeof(Vertex2)* 3);
		//	//unmap and unbind
		//	glUnmapBuffer(GL_ARRAY_BUFFER);
		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		//}



		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

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

//Load texture via SOIL
unsigned int loadTexture(const char* a_pFilename, int & a_iWidth, int & a_iHeight, int & a_iBPP)
{
	unsigned int uiTextureID = 0;
	//check file exists
	if (a_pFilename != nullptr)
	{
		//read in image data from file
		unsigned char* pImageData = SOIL_load_image(a_pFilename, &a_iWidth, &a_iHeight, &a_iBPP, SOIL_LOAD_AUTO);

		//check for successful read
		if (pImageData)
		{
			//create opengl texture handle
			uiTextureID = SOIL_create_OGL_texture(pImageData, a_iWidth, a_iHeight, a_iBPP,
				SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
			//clear what was read in from file now that it is stored in the handle
			SOIL_free_image_data(pImageData);
		}

		//check for errors
		if (uiTextureID == 0)
		{
			std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
		}
		return uiTextureID;
	}
}