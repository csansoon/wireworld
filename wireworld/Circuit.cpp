#include "Circuit.h"
#include <iostream>
using namespace std;


Circuit::Circuit()
{
}


Circuit::~Circuit()
{
}

void Circuit::createChunk(const sf::Vector2i coord) {
	Chunk newChunk;
	newChunk.position.x = (coord.x / 100) * 100;
	newChunk.position.y = (coord.y / 100) * 100;
	
	//Set adjacents:
	for (int p = 0; p < 4; ++p) {
		sf::Vector2i newPos = newChunk.position;
		if		(p == 0) newPos.y -= 100;	//UP
		else if (p == 1) newPos.x += 100;	//RIGHT
		else if (p == 2) newPos.y += 100;	//DOWN
		else			 newPos.x -= 100;	//LEFT

		for(int i = 0; i < chunks.size(); ++i) {
			if (chunks[i].position == newPos) {
				newChunk.adjacents[p] = i;
				chunks[i].adjacents[(p+2)%4] = chunks.size();
				break;
			}
		}
	}

	//Add to vector and map
	//tags[newChunk.position] = chunks.size();
	chunks.push_back(newChunk);

	std::cout << "[Created chunk at (" << newChunk.position.x << "," << newChunk.position.y << ")]" << std::endl;
}


void Circuit::updateCell(const sf::Vector2f click) {
	sf::Vector2i coord = sf::Vector2i(click);
	std::cout << "Pressed coord [" << coord.x << "," << coord.y << "]" << std::endl;

	//if (tags.find((coord * 100) / 100) == tags.end()) createChunk(coord);
}