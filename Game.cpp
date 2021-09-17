#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Shader* shader;
float l = -1.6f, r = 1.6f, d = -0.9f, u = 0.9f;

Game::Game(uint32_t width, uint32_t height, const std::string & title)
	: m_Width{width}, m_Height{height}, m_Title{title}
{
	
}

Game::~Game()
{
	while (!m_KeyBoardStack.empty())
	{
		m_KeyBoardStack.pop();
	}

	while (!m_MouseClickStack.empty())
	{
		m_MouseClickStack.pop();
	}

	while (!m_MouseScrollStack.empty())
	{
		m_MouseScrollStack.pop();
	}

	glfwDestroyWindow(m_Window);
}

void Game::init()
{
	if (!glfwInit())
	{
		std::cerr << "Error : GLFW initialisation failed!" << '\n';
		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

	if (!m_Window)
	{
		std::cerr << "Error : GLFW window creation failed!" << '\n';
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Error : GLEW initialisation failed!" << '\n';
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		return;
	}

	glViewport(0, 0, m_Width, m_Height);
	m_Running = true;

	shader = new Shader("simple.glsl");
}

void Game::handleEvent()
{
	glfwPollEvents();
}

void Game::update()
{
}

void Game::draw()
{
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->use();
	shader->bind();
	shader->modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f,0.2f,0.2f));
	shader->viewMatrix = glm::mat4(1.0f);
	shader->projectionMatrix = glm::ortho(l, r, d, u, -1.0f, 1.0f);
	shader->draw();

	glfwSwapBuffers(m_Window);
}

void Game::addEventListener(InputEvent event, void (*keyBoardListener)(GLFWwindow*, int, int, int, int))
{
	glfwSetKeyCallback(m_Window, keyBoardListener);
	m_KeyBoardStack.push(keyBoardListener);
}

void Game::addEventListener(InputEvent event, void (*mouseMoveListener)(GLFWwindow*,double, double))
{
	if (event == InputEvent::MOUSE_POSITION)
	{
		glfwSetCursorPosCallback(m_Window, mouseMoveListener);
		m_MousePosStack.push(mouseMoveListener);
	}

	else if (event == InputEvent::MOUSE_SCROLL)
	{
		glfwSetScrollCallback(m_Window, mouseMoveListener);
		m_MouseScrollStack.push(mouseMoveListener);
	}
	
}

void Game::addEventListener(InputEvent event, void (*mouseClickListener)(GLFWwindow*, int, int, int))
{
	glfwSetMouseButtonCallback(m_Window, mouseClickListener);
	m_MouseClickStack.push(mouseClickListener);
}

void Game::popEventListener(InputEvent event)
{
	if (event == InputEvent::KEY_PRESS)
	{
		m_KeyBoardStack.pop();
		if (!m_KeyBoardStack.empty())
		{
			glfwSetKeyCallback(m_Window, m_KeyBoardStack.top());
		}
	}
	else if (event == InputEvent::MOUSE_SCROLL)
	{
		m_MouseScrollStack.pop();
		if (!m_MouseScrollStack.empty())
		{
			glfwSetScrollCallback(m_Window, m_MouseScrollStack.top());
		}
	}
	else if (event == InputEvent::MOUSE_CLICK)
	{
		m_MouseClickStack.pop();
		if (!m_MouseClickStack.empty())
		{
			glfwSetMouseButtonCallback(m_Window, m_MouseClickStack.top());
		}
	}
}

void Game::addWindowLayer()
{
}

void Game::popWindowLayer()
{
}

bool Game::running()
{
	return (!glfwWindowShouldClose(m_Window) && m_Running);
}

void Game::stop()
{
	m_Running = false;
}

GLFWwindow* Game::getWindow()
{
	return m_Window;
}

constexpr uint32_t Game::getWidth()
{
	return m_Width;
}

constexpr uint32_t Game::getHeight()
{
	return m_Height;
}

void Game::destroy()
{
	this->~Game();
}
