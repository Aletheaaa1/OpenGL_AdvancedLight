#pragma once

class VertexBufferObject
{
private:
	unsigned int VBO;

public:
	VertexBufferObject(const void* data, const int length);
	~VertexBufferObject();
	void Bind();
	void Unbind();
};