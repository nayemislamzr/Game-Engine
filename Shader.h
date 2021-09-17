#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader() = delete;
	Shader(const std::string& filePath);
	void use();
	void bind();
	void draw();
	void unbind();
	void setInt(const std::string& varname, int val);
	~Shader();
private:
	void parseFile(const std::string& filePath);
	void createRectangle();
	void compile(GLenum);
	void link();
	uint32_t  m_VAO, m_VBO, m_IBO;
	uint32_t m_Program, m_Shader;
	std::string m_Vertex, m_Fragment;
public:
	int32_t uniformModel = -1, uniformView = -1, uniformProjection = -1;
	glm::mat4 modelMatrix = glm::mat4(1.0f), viewMatrix = glm::mat4(1.0f), projectionMatrix = glm::mat4(1.0f);
};

