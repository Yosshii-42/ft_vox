#include "gl/Shader.hpp"
#include <stdexcept>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
  : ID_(0)
{
  // 1. retrieve the vertex/fragment source code from filePath
  std::string   vertexCode;
  std::string   fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // ensure ifstream objects can throw exceptions
  vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure& e)
  {
    throw std::runtime_error(std::string("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: ") + e.what());
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // 2. compile shaders
  unsigned int  vertex = 0;
  unsigned int  fragment = 0;
  try
  {
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    if (vertex == 0)
      throw std::runtime_error("Failed to create vertex shader");
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragment == 0)
      throw std::runtime_error("Failed to create fragment shader");
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // shader program
    ID_ = glCreateProgram();
    if (ID_ == 0)
      throw std::runtime_error("Failed to create program shader");
    glAttachShader(ID_, vertex);
    glAttachShader(ID_, fragment);
    glLinkProgram(ID_);
    checkCompileErrors(ID_, "PROGRAM");
  }
  catch(...)
  {
    // errorの場合リソースを解放する
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (ID_ != 0)
    {
      glDeleteProgram(ID_);
      ID_ = 0;
    }
    throw;
  }
  
  // Successfully linked
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() {
  if (ID_ != 0)
    glDeleteProgram(ID_);
}

void  Shader::setMatrices(const Mat4& model, const Mat4& view, const Mat4& projection)
{
  setMat4("model", model);
  setMat4("view", view);
  setMat4("projection", projection);
}

// activate the shader
void  Shader::use()
{
  glUseProgram(ID_);
}

// set functions
void  Shader::setBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(ID_, name.c_str()), (int)value);
}

void  Shader::setInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}

void  Shader::setFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}

void  Shader::setVec3(const std::string& name, const Vec3& vec) const
{
  glUniform3f(glGetUniformLocation(ID_, name.c_str()), vec.x, vec.y, vec.z);
}

void  Shader::setVec3(const std::string& name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(ID_, name.c_str()), x, y, z);
}

void  Shader::setVec4(const std::string&name, const Vec4& vec) const
{
  glUniform4f(glGetUniformLocation(ID_, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void  Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(ID_, name.c_str()), x, y, z, w);
}

void  Shader::setMat4(const std::string& name, const Mat4& mat) const {
  glad_glUniformMatrix4fv(
    glGetUniformLocation(ID_, name.c_str()),
    1,
    GL_TRUE,
    mat.data()
  );
}

// check functions
void  Shader::checkCompileErrors(unsigned int shader, const std::string& type)
{
  int   success;
  char  infoLog[1024];

  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      throw std::runtime_error("ERROR::SHADER_COMPILATION_ERROR of type: " +
                               type +
                               "\n" +
                               infoLog +
                               "\n -- --------------------------------------------------- -- ");
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      throw std::runtime_error("ERROR::PROGRAM_LINKING_ERROR of type: " +
                               type +
                               "\n" +
                               infoLog +
                               "\n -- --------------------------------------------------- -- ");
    }
  }
}
