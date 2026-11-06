#pragma once

#include "Common.hpp"
#include "gl/Manipulator.hpp"
#include "gl/Camera.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
private:
  GLFWwindow* window_;
  int         width_;
  int         height_;
  double      lastMouseX_ = 0.0;
  double      lastMouseY_ = 0.0;
  bool        firstMouse_ = true;
  bool        mouseCaputured_ = true;
  bool        tabPressed_     = true;

  static void framebufferSizeCallback(GLFWwindow* window,
                                      int width,
                                      int height);

public:
  Window();
  ~Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  
  void  show();
  GLFWwindow* getWindow() const;
  void  processInput(float& textureScale, Camera& camera, float deltaTime);
  void  tabJudge();
  void  mouseOperate(Camera& camera);
  bool  shouldClose() const;
  float getAspectRatio() const;
};
