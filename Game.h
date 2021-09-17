#pragma once

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl.lib")

#include <iostream>
#include <string>
#include <stdlib.h>
#include <functional>
#include <stack>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Texture.h"

extern float l, r, d, u;

enum class InputEvent
{
	KEY_PRESS = 0,
	MOUSE_POSITION,
	MOUSE_SCROLL,
	MOUSE_CLICK
};

class Game
{
public:
	Game() = default;
	~Game();
	Game(uint32_t width, uint32_t height,const std::string& title);
	void init();
	void handleEvent();
	virtual void update();
	virtual void draw();
	void addEventListener(InputEvent, void (*m_KeyBoardListener)(GLFWwindow*, int, int, int, int));
	void addEventListener(InputEvent, void (*m_MouseMoveListener)(GLFWwindow*, double, double));
	void addEventListener(InputEvent, void (*m_MouseClickListener)(GLFWwindow*, int, int, int));
	void popEventListener(InputEvent);
	void addWindowLayer();
	void popWindowLayer();
	bool running();
	void stop();
	GLFWwindow* getWindow();
	constexpr uint32_t getWidth();
	constexpr uint32_t getHeight();
	void destroy();
private:
	const std::string m_Title;
	uint32_t m_Width, m_Height;
	GLFWwindow* m_Window = nullptr;
	bool m_Running = false;
	std::stack <void (*)(GLFWwindow*, int, int, int, int)> m_KeyBoardStack;
	std::stack <void (*)(GLFWwindow*, double xPos, double yPos)> m_MouseScrollStack;
	std::stack <void (*)(GLFWwindow*, double xPos, double yPos)> m_MousePosStack;
	std::stack <void (*)(GLFWwindow*, int, int, int)> m_MouseClickStack;
};

