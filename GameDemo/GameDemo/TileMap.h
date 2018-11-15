#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Textures.h"
#include "Sprites.h"
#include "Debug.h"


#define TILE_SIZE 32

class TileMap
{
	int rows;
	int cols;
	int map_Height;
	int map_Width;

	int **matrix;

	LPCWSTR filePath_tex;
	LPCWSTR filePath_data;
public:
	TileMap();
	~TileMap();
	void LoadResources();
};

