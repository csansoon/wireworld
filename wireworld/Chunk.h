#pragma once
#include "Cell.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
class Chunk
{
public:
	// 100 x 100 Cells
	Chunk();
	~Chunk();

	std::vector<int> adjacents;	// UP RIGHT DOWN LEFT
	sf::Vector2i position;
};

