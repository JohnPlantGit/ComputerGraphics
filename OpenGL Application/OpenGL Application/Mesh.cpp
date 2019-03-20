#include "Mesh.h"
#include "gl_core_4_4.h"
#include "Texture.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	// check that the mesh is not initialized already
	assert(vao == 0);

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind buffers
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	if (indexCount != 0)
	{
		glGenBuffers(1, &ibo);

		// bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// fill vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else
	{
		triCount = vertexCount / 3;
	}

	// unbind buffers to stop accidentally modifying data
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitialiseBox(glm::vec3 extents)
{
	// check that the mesh is not initialized already
	assert(vao == 0);

	Vertex vertices[8];
	vertices[0].position = { -0.5f * extents.x, 0.5f * extents.y, 0.5f * extents.z, 1 };
	vertices[1].position = { 0.5f * extents.x, 0.5f * extents.y, 0.5f * extents.z, 1 };
	vertices[2].position = { -0.5f * extents.x, 0.5f * extents.y, -0.5f * extents.z, 1 };
	vertices[3].position = { 0.5f * extents.x, 0.5f * extents.y, -0.5f * extents.z, 1 };

	vertices[4].position = { -0.5f * extents.x, -0.5f * extents.y, 0.5f * extents.z, 1 };
	vertices[5].position = { 0.5f * extents.x, -0.5f * extents.y, 0.5f * extents.z, 1 };
	vertices[6].position = { -0.5f * extents.x, -0.5f * extents.y, -0.5f * extents.z, 1 };
	vertices[7].position = { 0.5f * extents.x, -0.5f * extents.y, -0.5f * extents.z, 1 };

	unsigned int indices[36] = 
	{
		0,1,2,
		2,3,1,

		0,1,5,
		5,4,0,

		2,0,4,
		4,6,2,

		3,2,6,
		6,7,3,

		1,3,7,
		7,5,1,

		5,4,6,
		6,7,5
	};


	Initialise(8, vertices, 36, indices);
}

void Mesh::InitialiseQuad()
{
	// check that the mesh is not initialized already
	assert(vao == 0);

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind buffers
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// 2 triangles
	Vertex vertices[6];
	vertices[0].position = glm::vec4(-0.5f, 0, 0.5f, 1);
	vertices[0].texCoord = { 0, 1 };
	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].position = glm::vec4(0.5f, 0, 0.5f, 1);
	vertices[1].texCoord = { 1, 1 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].position = glm::vec4(-0.5f, 0, -0.5f, 1);
	vertices[2].texCoord = { 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };

	vertices[3].position = glm::vec4(-0.5f, 0, -0.5f, 1);
	vertices[3].texCoord = { 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].position = glm::vec4(0.5f, 0, 0.5f, 1);
	vertices[4].texCoord = { 1, 1 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].position = glm::vec4(0.5f, 0, -0.5f, 1);
	vertices[5].texCoord = { 1, 0 };
	vertices[5].normal = { 0, 1, 0, 0 };

	// fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// enable the second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// enable third element as texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// unbind buffers to stop accidentally modifying data
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	triCount = 2;
}

void Mesh::Draw()
{
	glBindVertexArray(vao); // re-bind vertex array for use

	// using index buffer or just vertices
	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}
}
