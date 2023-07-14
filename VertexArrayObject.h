#pragma once
class VertexArrayObject
{
  private:
  public:
    unsigned int vao;

    VertexArrayObject();
    ~VertexArrayObject();
    void AddBuffer(int index, int size, unsigned int type, bool normalized, int stride, int pointer);
    void Bind();
    void Unbind();
};