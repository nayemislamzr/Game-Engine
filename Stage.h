#pragma once

#include <vector>
#include <string>

#include "Game.h"

class Stage : public Game
{
public:
	Stage() = default;
	Stage(std::vector<std::string>& audios, std::vector<std::string>& images);
	~Stage();
	void init();
	virtual void update() = 0;
	virtual void draw() = 0;
	void clean();
protected:
	float m_StageWidth, m_StageHeight;
};
