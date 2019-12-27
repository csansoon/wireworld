#include "Circuit.h"
#include <iostream>
#include <set>
using namespace std;


Circuit::Circuit()
{
	firstChunk = { 0, 0 };
	lastChunk = { 0, 0 };
}


Circuit::~Circuit()
{
}

void Circuit::createChunk(const sf::Vector2i coord) {
	Chunk newChunk;
	newChunk.position = coord;
	//newChunk.position.x = (coord.x / 100) * 100;
	//newChunk.position.y = (coord.y / 100) * 100;

	//Set adjacents:
	for (int p = 0; p < 4; ++p) {
		sf::Vector2i newPos = newChunk.position;
		if (p == 0) newPos.y -= 100;	//UP
		else if (p == 1) newPos.x += 100;	//RIGHT
		else if (p == 2) newPos.y += 100;	//DOWN
		else			 newPos.x -= 100;	//LEFT

		for (int i = 0; i < chunks.size(); ++i) {
			if (chunks[i].position == newPos) {
				newChunk.adjacents[p] = i;
				chunks[i].adjacents[(p + 2) % 4] = chunks.size();
				break;
			}
		}
	}

	//Add to vector and map
	tags[{ newChunk.position.x, newChunk.position.y }] = chunks.size();
	chunks.push_back(newChunk);

	std::cout << "[Created chunk at (" << newChunk.position.x << "," << newChunk.position.y << ")]" << std::endl;
	if (newChunk.position.x < firstChunk.first) firstChunk.first = newChunk.position.x;
	if (newChunk.position.y < firstChunk.second) firstChunk.second = newChunk.position.y;
	if (newChunk.position.x > lastChunk.first) lastChunk.first = newChunk.position.x;
	if (newChunk.position.y > lastChunk.first) lastChunk.first = newChunk.position.y;
	
}


void Circuit::updateCell(const sf::Vector2f click) {
	pair<int, int> coord = { click.x, click.y };

	pair<int, int> chunkCoord = coord;
	if (chunkCoord.first < 0) chunkCoord.first -= 100;
	if (chunkCoord.second < 0) chunkCoord.second -= 100;
	chunkCoord = { (chunkCoord.first / 100) * 100, (chunkCoord.second / 100) * 100 };

	std::cout << "Pressed coord [" << coord.first << "," << coord.second << "], on chunk (" << chunkCoord.first << "," << chunkCoord.second << ")." << std::endl;

	if (tags.find(chunkCoord) == tags.end()) createChunk(sf::Vector2i(chunkCoord.first, chunkCoord.second));
}


std::vector<Chunk> Circuit::visibleChunks(const sf::Vector2i from, const sf::Vector2i to) {

	pair<int, int> fromChunk = { from.x, from.y };
	if (fromChunk.first < 0) fromChunk.first -= 100;
	if (fromChunk.second < 0) fromChunk.second -= 100;
	fromChunk = { (fromChunk.first / 100) * 100, (fromChunk.second / 100) * 100 };

	pair<int, int> toChunk = { to.x, to.y };
	if (toChunk.first < 0) toChunk.first -= 100;
	if (toChunk.second < 0) toChunk.second -= 100;
	toChunk = { (toChunk.first / 100) * 100, (toChunk.second / 100) * 100 };

	std::vector<Chunk> visibles;


	for (int i = fromChunk.first; i <= toChunk.first; i += 100) {
		for (int j = fromChunk.second; j <= toChunk.second; j += 100) {
			if (tags.find({ i,j }) != tags.end()) visibles.push_back(chunks[tags[{i, j}]]);
		}
	}

	return visibles;
}



int Circuit::state(const sf::Vector2i cellCoord) {
	pair<int, int> coord = { cellCoord.x, cellCoord.y };

	pair<int, int> localCoord = { coord.first % 100, coord.second % 100 };
	if (localCoord.first < 0) localCoord.first = 100 + localCoord.first;
	if (localCoord.second < 0) localCoord.second = 100 + localCoord.second;

	pair<int, int> chunkCoord = coord;
	if (chunkCoord.first < 0) chunkCoord.first -= 100;
	if (chunkCoord.second < 0) chunkCoord.second -= 100;
	chunkCoord = { (chunkCoord.first / 100) * 100, (chunkCoord.second / 100) * 100 };

	if (tags.find(chunkCoord) == tags.end()) return 0;

	return chunks[tags[chunkCoord]].matrix[localCoord.first][localCoord.second];
}


void Circuit::update() {

	for (int c = 0; c < chunks.size(); ++c) {
		vector<vector<int>> newMatrix = chunks[c].matrix;

		for (auto it = chunks[c].activeCells.begin(); it != chunks[c].activeCells.end(); ++it) {
			
			pair<int, int> cell = { it->first, it->second };
			int counter = 0;

			switch (state(sf::Vector2i(cell.first,cell.second))) {
				case 0:			//blank
					break;

				case 1:			//cable
					//Look 4 adjacencies: becomes head if it has 1 or 2 adjacent heads
					counter = 0;
					for (int i = 0; i < 8; ++i) {
						if		(i == 0 and state(sf::Vector2i(cell.first, cell.second-1)) == 2) ++counter; //UP
						else if (i == 1 and state(sf::Vector2i(cell.first+1, cell.second-1)) == 2) ++counter; //UP + RIGHT
						else if (i == 2 and state(sf::Vector2i(cell.first+1, cell.second)) == 2) ++counter; //RIGHT
						else if (i == 3 and state(sf::Vector2i(cell.first+1, cell.second+1)) == 2) ++counter; //RIGHT + DOWN
						else if (i == 0 and state(sf::Vector2i(cell.first, cell.second+1)) == 2) ++counter; //DOWN
						else if (i == 1 and state(sf::Vector2i(cell.first-1, cell.second+1)) == 2) ++counter; //DOWN + LEFT
						else if (i == 2 and state(sf::Vector2i(cell.first-1, cell.second)) == 2) ++counter; //LEFT
						else if (i == 3 and state(sf::Vector2i(cell.first-1, cell.second-1)) == 2) ++counter; //LEFT + UP
					}
					if (counter == 1 or counter == 2) {
						newMatrix[it->first - chunks[c].position.x][it->second - chunks[c].position.y] = 2;
					}
					break;

				case 2:			//head
					newMatrix[it->first - chunks[c].position.x][it->second - chunks[c].position.y] = 3;
					break;

				case 3:			//tail
					newMatrix[it->first - chunks[c].position.x][it->second - chunks[c].position.y] = 1;
					break;

			}
		}

		chunks[c].matrix = newMatrix;
	}
}

void Circuit::paint(sf::Vector2i click, int mode) {
	
	std::cout << "[Pressed at coord (" << click.x << "," << click.y << ") -> mode = " << mode << "]" << std::endl;

	pair<int, int> coord = { click.x, click.y };
	pair<int, int> localCoord = { click.x % 100, click.y % 100 };
	if (localCoord.first < 0) localCoord.first = 100 + localCoord.first;
	if (localCoord.second < 0) localCoord.second = 100 + localCoord.second;

	pair<int, int> chunkCoord = coord;
	if (chunkCoord.first < 0) chunkCoord.first -= 100;
	if (chunkCoord.second < 0) chunkCoord.second -= 100;
	chunkCoord = { (chunkCoord.first / 100) * 100, (chunkCoord.second / 100) * 100 };

	std::cout << "[CHUNK (" << chunkCoord.first << "," << chunkCoord.second << ") - LOCAL COORDINATE (" << localCoord.first << "," << localCoord.second << ")]" << std::endl;

	//std::cout << "Pressed coord [" << coord.first << "," << coord.second << "], on chunk (" << chunkCoord.first << "," << chunkCoord.second << ") and changed its state to " << mode << "." << std::endl;

	if (tags.find(chunkCoord) == tags.end()) createChunk(sf::Vector2i(chunkCoord.first, chunkCoord.second));
	
	if (mode == 0) chunks[tags[chunkCoord]].activeCells.erase(coord);
	else if (state(sf::Vector2i(coord.first, coord.second)) == 0) chunks[tags[chunkCoord]].activeCells.insert(coord);
	chunks[tags[chunkCoord]].matrix[localCoord.first][localCoord.second] = mode;
	
}


bool Circuit::load(std::string filename) {
	ifstream file;
	file.open("saves/" + filename);
	if (not file.is_open()) return false;

	int x, y;
	while (file >> x >> y) {

		createChunk(sf::Vector2i(x, y));

		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 100; ++j) {
				int cell;
				file >> cell;
				
				chunks[chunks.size() - 1].matrix[i][j] = cell;

				if (cell != 0) {
					chunks[chunks.size() - 1].activeCells.insert({ x + i, y + j });
				}
			}
		}
	}
	file.close();
	return true;
}

bool Circuit::save(std::string filename) {
	ofstream file;
	file.open("saves/" + filename);
	if (not file.is_open()) return false;

	for (int c = 0; c < chunks.size(); ++c) {
		//file << chunks[c].position.x << chunks[c].position.y;
		file.write(reinterpret_cast<const char*>(&chunks[c].position.y), sizeof(int));
		file.write(reinterpret_cast<const char*>(&chunks[c].position.y), sizeof(int));
		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 100; ++j) {
				//file << chunks[c].matrix[i][j];
				file.write(reinterpret_cast<const char*>(&chunks[c].matrix[i][j]), sizeof(int));
			}
		}
	}
	file.close();
	return true;
}