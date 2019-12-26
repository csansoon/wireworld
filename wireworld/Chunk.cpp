#include "Chunk.h"
using namespace std;



Chunk::Chunk()
{
	adjacents = std::vector<int>(4, -1);
	matrix = vector<vector<int>> (100,vector<int>(100,0));
}


Chunk::~Chunk()
{
}
