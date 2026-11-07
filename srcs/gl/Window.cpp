#include "gl/Window.hpp"
#include "Common.hpp"
#include "gl/Camera.hpp"
#include <stdexcept>

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
  glfwSwapInterval(1);
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, Window::framebufferSizeCallback);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // マウスの移動量取得

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

void Window::processInput(float& textureScale, Camera& camera,
                          float deltaTime, DrawWorld& world)
{
  // esc
	if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window_, true);

  // [→] 右回転
  if (glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS)
    camera.turnRight(deltaTime);
  // [←] 左回転
  if (glfwGetKey(window_, GLFW_KEY_LEFT ) == GLFW_PRESS)
    camera.turnLeft(deltaTime);
  // [↑] x軸上回転
  if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS)
    camera.turnUp(deltaTime);
  // [↓] x軸下回転
  if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS)
    camera.turnDown(deltaTime);
  
  // [A] 左へ移動
  if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
    camera.moveLeft(deltaTime);
  // [D] 右へ移動
  if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
    camera.moveRight(deltaTime);
  // [W] 前へ移動
  if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
    camera.moveForward(deltaTime);
  // [S] 後ろへ移動
  if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
    camera.moveBackward(deltaTime);
  // [Space] 上へ移動
  if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.moveUp(deltaTime);
  // [Right Shift] 下へ移動
  if (glfwGetKey(window_, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    camera.moveDown(deltaTime);

  // [C] Textureを細かくする
  if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS)
    textureScale *= 1.01f;
  // [V] Textureを大きくする
  if (glfwGetKey(window_, GLFW_KEY_V) == GLFW_PRESS)
    textureScale *= 0.99f;

  // tab判定
  tabJudge();

  // マウス操作
  mouseOperate(camera);

  // 描画距離
  // [1] Short(3チャンク)
  if (glfwGetKey(window_, GLFW_KEY_1) == GLFW_PRESS)
    world.setRenderRadius(3);

  // [2] Medium(5チャンク)
  if (glfwGetKey(window_, GLFW_KEY_2) == GLFW_PRESS)
    world.setRenderRadius(5);

  // [3] Long(8チャンク)
  if (glfwGetKey(window_, GLFW_KEY_3) == GLFW_PRESS)
    world.setRenderRadius(8);
}

// tab判定
void  Window::tabJudge()
{
  bool  tabDown = glfwGetKey(window_, GLFW_KEY_TAB) == GLFW_PRESS;
  if (tabDown && !tabPressed_)
  {
    mouseCaputured_ = !mouseCaputured_;
    glfwSetInputMode(window_, GLFW_CURSOR,
                     mouseCaputured_ ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    firstMouse_ = true;
  }
  tabPressed_ = tabDown;  
}

// マウス操作
void  Window::mouseOperate(Camera& camera)
{
  double  mouseX;
  double  mouseY;

  glfwGetCursorPos(window_, &mouseX, &mouseY);

  if (firstMouse_)
  {
    lastMouseX_ = mouseX;
    lastMouseY_ = mouseY;
    firstMouse_ = false;
  }

  float deltaX = static_cast<float>(mouseX - lastMouseX_);
  float deltaY = static_cast<float>(mouseY - lastMouseY_);
  lastMouseX_ = mouseX;
  lastMouseY_ = mouseY;
  camera.rotate(deltaX, deltaY);
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

