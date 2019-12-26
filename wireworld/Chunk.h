#pragma once
#include <iostream>
#include "Cell.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <set>
class Chunk
{
public:
	// 100 x 100 Cells
	Chunk();
	~Chunk();

	std::vector<int> adjacents;	// UP RIGHT DOWN LEFT
	sf::Vector2i position;

	std::vector<std::vector<int>> matrix;
	
	std::set<std::pair<int,int>> activeCells;	//Global coordinates
};

