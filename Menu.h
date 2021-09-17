#pragma once

#include "Stage.h"

class Menu : protected Stage
{
public:
	Menu();
	~Menu();
	void update() override;
	void draw() override;
};

