#include "tiles.h"

const uint8_t NUM_GROUND_TILES = 41;
//const uint8_t NUM_SLOPE_TILES = 3;

const struct Tile groundTiles[41] = {
		{.xPos = 0, .yPos = 7*24}, {.xPos = 24, .yPos = 7*24}, {.xPos = 2*24, .yPos = 7*24}, {.xPos = 3*24, .yPos = 7*24}, {.xPos = 4*24, .yPos = 7*24}, {.xPos = 5*24, .yPos = 7*24}, {.xPos = 6*24, .yPos = 7*24}, // First ground block
		{.xPos = 7*24, .yPos = 6*24}, {.xPos = 8*24, .yPos = 6*24}, {.xPos = 9*24, .yPos = 6*24}, {.xPos = 10*24, .yPos = 6*24}, // Second ground block
		{.xPos = 11*24, .yPos = 5*24}, {.xPos = 11*24, .yPos = 4*24}, {.xPos = 11*24, .yPos = 3*24}, {.xPos = 12*24, .yPos = 5*24}, {.xPos = 12*24, .yPos = 4*24}, {.xPos = 12*24, .yPos = 3*24}, // Tower block
		{.xPos = 13*24, .yPos = 6*24}, {.xPos = 14*24, .yPos = 6*24}, {.xPos = 15*24, .yPos = 6*24}, {.xPos = 16*24, .yPos = 6*24}, {.xPos = 17*24, .yPos = 6*24}, {.xPos = 18*24, .yPos = 6*24}, // Fourth ground block
		{.xPos = 19*24, .yPos = 7*24}, {.xPos = 20*24, .yPos = 7*24}, {.xPos = 21*24, .yPos = 7*24}, // Ground block below slope
		{.xPos = 22*24, .yPos = 7*24}, {.xPos = 23*24, .yPos = 6*24}, {.xPos = 24*24, .yPos = 5*24}, // Stairs
		{.xPos = 25*24, .yPos = 5*24}, {.xPos = 25*24, .yPos = 6*24}, {.xPos = 25*24, .yPos = 7*24}, {.xPos = 25*24, .yPos = 8*24}, // First big wall
		{.xPos = 28*24, .yPos = 5*24}, {.xPos = 28*24, .yPos = 6*24}, {.xPos = 28*24, .yPos = 7*24}, {.xPos = 28*24, .yPos = 8*24}, // Second big wall
		{.xPos = 29*24, .yPos = 5*24}, {.xPos = 30*24, .yPos = 5*24}, {.xPos = 31*24, .yPos = 5*24}, {.xPos = 32*24, .yPos = 5*24} // Goal patch
};

enum TileType Tiles_getTileType(uint16_t xPos, uint16_t yPos) {
	/* Checks ground tiles */
	for (int i = 0; i < NUM_GROUND_TILES; i++) {
		uint16_t tileX = groundTiles[i].xPos;
		uint16_t tileY = groundTiles[i].yPos;
		if (xPos >= tileX && yPos >= tileY && xPos < (tileX + 24) && yPos < (tileY + 24)) {
			return GROUND;
		}
	}

	/* Does not pass the test, remains as an air tile */
	return AIR;
}
