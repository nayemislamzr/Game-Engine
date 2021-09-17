#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>

class Texture
{
public:
	Texture() = default;
	Texture(float width, float height, const std::string& imgSrc);
	~Texture();
private:
	uint32_t texture;
	uint32_t* loadImage(const std::string& imgSrc);
};

