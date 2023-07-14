#include <glad/glad.h>

#include "Render.h"
#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
	GLCall(glGenVertexArrays(1, &vao));
	glBindVertexArray(vao);
}

VertexArrayObject::~VertexArrayObject()
{
	GLCall(glDeleteVertexArrays(1, &vao));
}

void VertexArrayObject::AddBuffer(int index, int size, unsigned int type, bool normalized, int stride, int pointer)
{
	GLCall(glVertexAttribPointer(index, size, type, normalized, stride, (void*)pointer));
	GLCall(glEnableVertexAttribArray(index));
}

void VertexArrayObject::Bind()
{
	GLCall(glBindVertexArray(vao));
}

void VertexArrayObject::Unbind()
{
	GLCall(glBindVertexArray(0));
}