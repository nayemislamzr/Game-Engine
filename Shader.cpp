#include "Shader.h"

Shader::Shader(const std::string& filePath)
{
	parseFile(filePath);
	createRectangle();
	compile(GL_VERTEX_SHADER);
	compile(GL_FRAGMENT_SHADER);
	link();
}

void Shader::use()
{
	glUseProgram(m_Program);
}

void Shader::bind()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void Shader::draw()
{
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}

void Shader::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Shader::setInt(const std::string& varname, int val)
{
	glUniform1d(glGetUniformLocation(m_Program, varname.c_str()), val);
}

Shader::~Shader()
{
	glDetachShader(m_Program, m_Shader);
	glDeleteProgram(m_Program);
}

void Shader::parseFile(const std::string& filePath)
{
	std::ifstream input(filePath);

	enum class type : int8_t
	{
		NONE = -1, VERTEX, FRAGMENT
	};

	std::string line;
	std::stringstream vertex, fragment;
	type shaderType = type::NONE;

	while (std::getline(input, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				shaderType = type::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				shaderType = type::FRAGMENT;
			}
		}
		else
		{
			if (shaderType == type::VERTEX)
			{
				vertex << line << '\n';
			}
			else if (shaderType == type::FRAGMENT)
			{
				fragment << line << '\n';
			}
		}
	}

	m_Vertex = vertex.str();
	m_Fragment = fragment.str();
}

void Shader::createRectangle()
{
	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	float vertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Shader::compile(GLenum shaderType)
{
	m_Shader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	if (shaderType == GL_VERTEX_SHADER)
		theCode[0] = m_Vertex.c_str();
	else if (shaderType == GL_FRAGMENT_SHADER)
		theCode[0] = m_Fragment.c_str();

	GLint codeLength[1];
	if (shaderType == GL_VERTEX_SHADER)
		codeLength[0] = strlen(m_Vertex.c_str());
	else if (shaderType == GL_FRAGMENT_SHADER)
		codeLength[0] = strlen(m_Fragment.c_str());

	glShaderSource(m_Shader, 1, theCode, codeLength);
	glCompileShader(m_Shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(m_Shader, sizeof(eLog), NULL, eLog);
		std::cerr << "Error compiling the " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << eLog << '\n';
		return;
	}

	glAttachShader(m_Program, m_Shader);
}

void Shader::link()
{
	m_Program = glCreateProgram();
	
	if (!m_Program)
	{
		std::cerr << "Failed to create shader\n";
		return;
	}

	compile(GL_VERTEX_SHADER);
	compile(GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(m_Program);
	glGetProgramiv(m_Program, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_Program, sizeof(eLog), NULL, eLog);
		std::cerr << "Error linking program: " << eLog << '\n';
		return;
	}

	glValidateProgram(m_Program);
	glGetProgramiv(m_Program, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_Program, sizeof(eLog), NULL, eLog);
		std::cerr << "Error validating program: " << eLog << '\n';
		return;
	}

	uniformModel = glGetUniformLocation(m_Program, "model");
	if (uniformModel < 0)
	{
		std::cout << "Could not find uniform model" << '\n';
	}

	uniformView = glGetUniformLocation(m_Program, "view");
	if (uniformView < 0)
	{
		std::cout << "Could not find uniform view" << '\n';
	}

	uniformProjection = glGetUniformLocation(m_Program, "projection");
	if (uniformProjection < 0)
	{
		std::cout << "Could not find uniform projection" << '\n';
	}

}