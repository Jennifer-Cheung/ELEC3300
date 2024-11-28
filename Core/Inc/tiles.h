#ifndef INC_TILES_H_
#define INC_TILES_H_

#include <stdio.h>

struct Tile {
	uint16_t xPos;
	uint16_t yPos;
};

enum TileType {
	AIR,
	GROUND,
	SLOPE
};

extern const uint8_t NUM_GROUND_TILES;
extern const struct Tile groundTiles[41];

extern const uint8_t NUM_SLOPE_TILES;
extern const struct Tile slopeTiles[3];

extern enum TileType Tiles_getTileType(uint16_t xPos, uint16_t yPos);
extern uint16_t Tiles_getYPosOnSlope(int xPos);

#endif /* INC_TILES_H_ */
