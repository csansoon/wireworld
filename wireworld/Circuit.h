#pragma once
#include "Chunk.h"
#include "Cell.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <map>
class Circuit
{
public:
	Circuit();
	~Circuit();

	void createChunk(const sf::Vector2i coord);

	void updateCell(const sf::Vector2f click);
	
	std::vector<Chunk> chunks;
	//std::map<sf::Vector2i, int> tags;
};

