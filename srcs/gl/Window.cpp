#include "gl/Window.hpp"
#include "gl/Instance.hpp"
#include "Common.hpp"

Window::Window()
 : window_(NULL),
   width_(Common::DEFAULT_WIDTH),
   height_(Common::DEFAULT_HEIGHT)
{
// reset cwd
#ifdef __APPLE__
glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif
  
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // windowを非表示で生成

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw: window creation
  // ---------------------
  window_ = glfwCreateWindow(width_,
                             height_,
                             "ft_vox",
                             NULL,
                             NULL);
	if (window_ == NULL)
	{
		glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window_);
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, Window::framebufferSizeCallback);

	// glad: OpenGL関数を呼び出す前にGLADを初期化する
  // -----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
    glfwDestroyWindow(window_);
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLAD");
	}

  int framebufferWidth;
  int framebufferHeight;
  glfwGetFramebufferSize(window_, &framebufferWidth, &framebufferHeight);
  width_ = framebufferWidth;
  height_ = framebufferHeight;
  glViewport(0, 0, width_, height_);
}

Window::~Window() 
{
  if (window_ != NULL)
    glfwDestroyWindow(window_);
  glfwTerminate();
}

void  Window::show()
{
  glfwShowWindow(window_);
}

GLFWwindow*  Window::getWindow() const
{
  return (window_);
}

void Window::processInput(Manipulator& manipulator,
                          float& textureScale)
{
  // esc
	if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window_, true);

  // [→] 右回転
  if (glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS)
    manipulator.rotateY(Mat4::radians(1.0f));
  // [←] 左回転
  if (glfwGetKey(window_, GLFW_KEY_LEFT ) == GLFW_PRESS)
    manipulator.rotateY(Mat4::radians(-1.0f));
  // [↑] x軸上回転
  if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS)
    manipulator.rotateX(Mat4::radians(-1.0f));
  // [↓] x軸下回転
  if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS)
    manipulator.rotateX(Mat4::radians(1.0f));
  // [Q] z軸右回転
  if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS)
    manipulator.rotateZ(Mat4::radians(1.0f));
  // [E] z軸左回転
  if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS)
    manipulator.rotateZ(Mat4::radians(-1.0f));
  
  // [R] 上移動
  if (glfwGetKey(window_, GLFW_KEY_R) == GLFW_PRESS)
    manipulator.translateY(0.01f);
  // [F] 下移動
  if (glfwGetKey(window_, GLFW_KEY_F) == GLFW_PRESS)
    manipulator.translateY(-0.01f);
  // [A] 左移動
  if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
    manipulator.translateX(-0.01f);
  // [D] 右移動
  if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
    manipulator.translateX(0.01f);
  // [W] 前へ移動
  if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
    manipulator.translateZ(0.01f);
  // [S] 後ろへ移動
  if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
    manipulator.translateZ(-0.01f);

  // [Z] 拡大
  if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS)
    manipulator.scale(1.01f);
  // [X] 縮小
  if (glfwGetKey(window_, GLFW_KEY_X) == GLFW_PRESS)
    manipulator.scale(0.99f);

  // [C] Textureを細かくする
  if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS)
    textureScale *= 1.01f;

  // [V] Textureを大きくする
  if (glfwGetKey(window_, GLFW_KEY_V) == GLFW_PRESS)
    textureScale *= 0.99f;

  // [Space] 個別回転
  if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS)
    manipulator.rotateRandomAxis(Mat4::radians(1.0f));
}

bool  Window::shouldClose() const
{
  return glfwWindowShouldClose(window_);
}

float Window::getAspectRatio() const
{
  if (height_ == 0)
    return 1.0f;
  
  return static_cast<float>(width_) / static_cast<float>(height_);
}

// ウィンドウのサイズが変更されるたびに呼び出されるコールバック関数
void  Window::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	// (void)window;

  // OpenGLにレンダリングウィンドウのサイズを伝える
	// 最初の２つの引数でウィンドウの左下隅の位置を設定する
	glViewport(0, 0, width, height);
  Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (self == NULL)
    return ;
  self->width_ = width;
  self->height_ = height;
}

