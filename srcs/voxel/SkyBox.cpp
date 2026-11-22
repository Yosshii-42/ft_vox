#include "voxel/SkyBox.hpp"
#include <stb/stb_image.h>
#include <stdexcept>

SkyBox::SkyBox()
{
  setupMesh();
}

SkyBox::~SkyBox()
{
  if (textureID_ != 0)
    glDeleteTextures(1, &textureID_);
  glDeleteBuffers(1, &VBO_);
  glDeleteVertexArrays(1, &VAO_);
}

void  SkyBox::load(const std::vector<std::string>& paths)
{
  if (paths.empty() || paths.size() != 6)
    throw std::runtime_error("No texture paths provided: cloud");

  if (textureID_ != 0)
    glDeleteTextures(1, &textureID_);
  glGenTextures(1, &textureID_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID_);

  try
  {
    int firstWidth = 0;
    int firstHeight = 0;

    for (size_t i = 0; i < paths.size(); ++i)
    {
      int width = 0;
      int height = 0;
      int channel = 0;

      unsigned char* pixels = stbi_load(
        paths[i].c_str(), &width, &height, &channel, STBI_rgb_alpha);
      if (!pixels)
        throw std::runtime_error("Failed to load texture: " + paths[i]);

      if (width != height)
      {
        stbi_image_free(pixels);
        throw std::runtime_error("SkyBox texture must be square: " + paths[i]);
      }

      if (i == 0)
      {
        firstHeight = height;
        firstWidth = width;
      }
      else if (width != firstWidth || height != firstHeight)
      {
        stbi_image_free(pixels);
        throw std::runtime_error("SkyBox texture size do not match: " + paths[i]);
      }

      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0,
                   GL_RGBA8,
                   width, height, 0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   pixels
      );

      stbi_image_free(pixels);
    }
  }
  catch(const std::exception& e)
  {
    glDeleteTextures(1, &textureID_);
    textureID_ = 0;
    throw;
  }

  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // U
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // V
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void  SkyBox::bind() const
{
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID_);
}

void  SkyBox::setupMesh()
{
  if (VAO_ == 0)
    glGenVertexArrays(1, &VAO_);
  if (VBO_ == 0)
    glGenBuffers(1, &VBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);

  const float skyboxVetices[] = {
    // right
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,

    // left
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    
    // top
    -0.5f, 0.5f,  0.5f,
     0.5f, 0.5f,  0.5f,
     0.5f, 0.5f, -0.5f,
    
     0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f,  0.5f,
    
    // down
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
    
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    
    // front
    -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f,  0.5f, 0.5f,
    
     0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    
    // back
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f
  };

  glBufferData(GL_ARRAY_BUFFER,
               sizeof(skyboxVetices),
               skyboxVetices,
               GL_STATIC_DRAW
  );

  if (VAO_)
  {
    // position location = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
  }
  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void  SkyBox::draw(Shader& skyboxShader) const
{
  skyboxShader.use();
  bind();
  glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);
  
  // 描画
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
}
