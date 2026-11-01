#include "gl/Crosshair.hpp"

Crosshair::Crosshair()
  : vao_(0), vbo_(0)
{
  const float vertices[] {
    0.0f, -0.02f,
    0.0f, 0.02f,
    -0.02f, 0.0f,
    0.02f, 0.0f
  };

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vao_);

  glBufferData(GL_ARRAY_BUFFER,
               sizeof(vertices),
               vertices,
               GL_STATIC_DRAW
  );
  
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                        2 * sizeof(float), reinterpret_cast<void*>(0)
  );
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Crosshair::~Crosshair() {
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
}

void  Crosshair::draw(Shader& shader, float aspectRatio) const
{
  shader.use();

  shader.setFloat("aspectRatio", aspectRatio);
  glBindVertexArray(vao_);
  glDrawArrays(GL_LINES, 0, 4);
  glBindVertexArray(0);
}
