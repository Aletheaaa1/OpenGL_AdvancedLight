#include "VertexBufferObject.h"
#include "Render.h"

#include <glad/glad.h>

VertexBufferObject::VertexBufferObject(const void* data, const int length)
{
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, length, data, GL_STATIC_DRAW));
}

VertexBufferObject::~VertexBufferObject()
{
	GLCall(glDeleteBuffers(1, &VBO));
}

void VertexBufferObject::Bind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
}

void VertexBufferObject::Unbind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}