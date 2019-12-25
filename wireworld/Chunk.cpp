#include "Chunk.h"



Chunk::Chunk()
{
	adjacents = std::vector<int>(4, -1);
}


Chunk::~Chunk()
{
}
