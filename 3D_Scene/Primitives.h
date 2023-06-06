#pragma once
#define _USE_MATH_DEFINES

#include <glew.h>
#include <glfw3.h>

#include <vector>
#include <cmath>
#include "Vertex.h"

class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

public:
	Primitive()
	{

	}

	virtual ~Primitive()
	{

	}

	//functions
	void set(const Vertex* vertices,
		const unsigned nrOfVertices,
		const GLuint* indices,
		const unsigned nrOfIndices)
	{
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertices.push_back(vertices[i]);
		}

		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indices.push_back(indices[i]);
		}
	}

	inline Vertex* getVertices() { return this->vertices.data(); }
	inline GLuint* getIndices() { return this->indices.data(); }
	inline const unsigned getNrOfVertices() { return this->vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->indices.size(); }
};

class Pyramid : public Primitive
{
public:
	Pyramid()
		: Primitive()
	{
		const float REPEAT = 1.f;
		
		Vertex vertices[] =
		{
			//position						//color									//texture

			//sides
			//v0 (top)
			glm::vec3(0.f, 1.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, REPEAT),
			//v1
			glm::vec3(-1.f, -1.f, 1.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 0.f),
			//v2
			glm::vec3(1.f, -1.f, 1.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(REPEAT, 0.f),
			//v3
			glm::vec3(1.f, -1.f, -1.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 0.f),
			//v4
			glm::vec3(-1.f, -1.f, -1.f),	glm::vec3(1.f, 0.f, 0.f),		glm::vec2(REPEAT, 0.f),

			//base
			//v5 (4, base)
			glm::vec3(-1.f, -1.f, -1.f),	glm::vec3(1.f, 0.f, 0.f),		glm::vec2(REPEAT, REPEAT),
			//v6 (3, base)
			glm::vec3(1.f, -1.f, -1.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(REPEAT, 0.f),
			//v7 (1, base)
			glm::vec3(-1.f, -1.f, 1.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 0.f),
			//v8 (2, base)
			glm::vec3(1.f, -1.f, 1.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.F, REPEAT),
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		// to reuse vertices
		GLuint indices[] =
		{
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 1,
			5, 6, 7,
			8, 6, 7
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class Quad : public Primitive
{
public:
	Quad()
		: Primitive()
	{
		const float REPEAT = 1.f;

		Vertex vertices[] =
		{
			//Position								//Color									//Texture
			glm::vec3(-0.5f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),			glm::vec2(0.f, REPEAT),
			glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec3(0.f, 1.f, 0.f),			glm::vec2(0.f, 0.f),
			glm::vec3(0.5f, -0.5f, 0.f),			glm::vec3(0.f, 0.f, 1.f),			glm::vec2(REPEAT, 0.f),
			glm::vec3(0.5f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),			glm::vec2(REPEAT, REPEAT),
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2,	//Triangle 1
			0, 2, 3		//Triangle 2
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class Cube : public Primitive
{
public:
	Cube()
		: Primitive()
	{
		const float REPEAT = 1.f;
		
		Vertex vertices[] =
		{
			//Position								//Color								//Texture
			glm::vec3(-0.5f, 0.5f, 0.5f),			glm::vec3(0.21f, 0.21f, 0.82f),		glm::vec2(0.f, REPEAT),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec3(0.21f, 0.21f, 0.82f),		glm::vec2(0.f, 0.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec3(0.21f, 0.21f, 0.82f),		glm::vec2(REPEAT, 0.f),
			glm::vec3(0.5f, 0.5f, 0.5f),			glm::vec3(0.21f, 0.21f, 0.82f),		glm::vec2(REPEAT, REPEAT),

			glm::vec3(0.5f, 0.5f, -0.5f),			glm::vec3(0.21f, 0.21f, 0.82f),		glm::vec2(0.f, REPEAT),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec3(0.21f, 0.21f, 0.82f),		glm::vec2(0.f, 0.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec3(0.21f, 0.21f, 0.82f),		glm::vec2(REPEAT, 0.f),
			glm::vec3(-0.5f, 0.5f, -0.5f),			glm::vec3(0.21f, 0.21f, 0.82f),		glm::vec2(REPEAT, REPEAT)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2,
			0, 2, 3,

			7, 6, 1,
			7, 1, 0,

			4, 5, 6,
			4, 6, 7,

			3, 2, 5,
			3, 5, 4
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class Plane : public Primitive
{
public:
	Plane()
		: Primitive()
	{
		const float REPEAT = 3.f;
		
		Vertex vertices[] =
		{
			//position						//color							//texture

			//v0 (bottom left)
			glm::vec3(-5.f, 0.f, -5.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, REPEAT),
			//v1 (top left)
			glm::vec3(-5.f, 0.f, 5.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),
			//v2 (top right)
			glm::vec3(5.f, 0.f, 5.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(REPEAT, 0.f),
			//v3 (bottom right)
			glm::vec3(5.f, 0.f, -5.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(REPEAT, REPEAT)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		// to reuse vertices
		GLuint indices[] =
		{
			0, 3, 2,
			2, 1, 0
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};