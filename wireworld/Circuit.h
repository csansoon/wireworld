#pragma once
#include "Chunk.h"
#include "Cell.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <map>
#include <fstream>
class Circuit
{
public:
	Circuit();
	~Circuit();

	void createChunk(const sf::Vector2i coord);

	void updateCell(const sf::Vector2f click);
	
	std::vector<Chunk> visibleChunks(const sf::Vector2i from, const sf::Vector2i to);

	int state(const sf::Vector2i cellCoord);


	void update();

	void paint(sf::Vector2i click, int mode);


	std::vector<Chunk> chunks;
	std::map<std::pair<int,int>, int> tags;

	bool load(std::string file);
	bool save(std::string file);

private:
	std::pair<int, int> firstChunk;
	std::pair<int, int> lastChunk;
};

