#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <glm\glm.hpp>
using namespace glm;

class Player
{

public:

	Player(){
		//Origin point of player
		origin = vec2(1024.0f * .5f, 768.0f * .5f);
		origin = vec2(1024.0f * .5f, 768.0f * .5f);
		//Vertecis offset from origin
		vert1 = vec2(0, 100);
		vert2 = vec2(100, -100);
		vert3 = vec2(-100, -100);
		vert1 = vec2(0, 100);
		vert2 = vec2(100, -100);
		vert3 = vec2(-100, -100);
	}

	vec2 origin;
	vec2 vert1;
	vec2 vert2;
	vec2 vert3;
};

//#include "Entity.h"
//
//class Player : 
//	public Entity
//{
//public:
//	Player()
//	{
//		LoadModelVertices();
//		LoadModelUVs();
//		verticesBuffer = new Vertex[modelVertices.size()];
//	}
//
//	~Player()
//	{
//		//delete verticesBuffer;
//	}
//
//	void Draw()
//	{
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
//
//		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)* 2));
//
//		glDrawElements(GL_TRIANGLES, modelVertices.size(), GL_UNSIGNED_BYTE, NULL);
//
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	}
//
//private:
//	void LoadModelVertices()
//	{
//		modelVertices.push_back(glm::vec4(0, 10.0f, 0, 1));
//		modelVertices.push_back(glm::vec4(-5.0f, -10.0f, 0, 1));
//		modelVertices.push_back(glm::vec4(5.0f, -10.0f, 0, 1));
//	}
//
//	void LoadModelUVs(){
//		modelUVs.push_back(glm::vec2(0.5f, 1.0f));
//		modelUVs.push_back(glm::vec2(0.2f, 0.5f));
//		modelUVs.push_back(glm::vec2(0.7f, 0.5f));
//	}
//};
#endif