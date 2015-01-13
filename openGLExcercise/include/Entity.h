#ifndef _ENTITY_H
#define _ENTITY_H

#include <GL\glew.h>
#include <GL\wglew.h>
#include <glm\glm.hpp>
#include <vector>

#include <iostream>

using namespace glm;

//generic Vertex

struct Vertex{
	vec4 positions;
	vec4 colors;
	vec2 uvs;
};

class Entity{
public:

	void Initialize(vec4& a_position, vec4& a_color, GLuint& a_textureID){
		position = a_position;
		color = a_color;
		textureID = a_textureID;

	}

protected:
	GLuint uiVBO;
	GLuint uiIBO;
	GLuint textureID;
	Vertex* verticesBuffer;
	std::vector<vec4> modelVertices;
	std::vector<vec2> modelUVs;
	vec4 position;
	vec4 color;
	vec2 uv;

	virtual void LoadModelVertices() = 0;
	virtual void LoadModelUVs() = 0;

	void UpdateVBO(){
		//bind vbo
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		//allocate space for vertices on the graphics card
		//size of buffer needs to be 3 vec4 for vertices and 3 vec4 for 
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* modelVertices.size(), NULL, GL_STATIC_DRAW);
		//get pointer to allocated space on the graphics card
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		//copy data to graphics card
		memcpy(vBuffer, verticesBuffer, sizeof(Vertex)* modelVertices.size());
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void UpdateIBO(){
		//bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		//allocate space for index info on  the graphics card
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelVertices.size() * sizeof(char), NULL, GL_STATIC_DRAW);
		//get pointer to newly allocated space on GPU
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		//specify order to draw vertices
		//in this case it's in sequential order
		for (int i = 0; i < modelVertices.size(); i++)
		{
			((char*)iBuffer)[i] = i;
		}
		//unmap and unbind 
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void UpdateVertices(){
		for (int i = 0; i < modelVertices.size(); i++){
			verticesBuffer[i].positions[0] = modelVertices[i].x + position.x;
			verticesBuffer[i].positions[1] = modelVertices[i].y + position.y;
			verticesBuffer[i].positions[2] = 0;
			verticesBuffer[i].positions[3] = 1;
			verticesBuffer[i].colors[0] = color.r;
			verticesBuffer[i].colors[1] = color.g;
			verticesBuffer[i].colors[2] = color.b;
			verticesBuffer[i].colors[3] = color.a;
			verticesBuffer[i].uvs[0] = uv.x;
			verticesBuffer[i].uvs[1] = uv.y;
		}
		UpdateVBO();
		UpdateIBO();
	}

};


#endif