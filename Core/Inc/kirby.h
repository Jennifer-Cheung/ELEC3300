#ifndef __KIRBY
#define __KIRBY

const uint8_t KIRBY_WIDTH = 26;
const uint8_t KIRBY_HEIGHT = 23;

struct Kirby {
	uint16_t xPos;
	uint16_t yPos;
	const uint16_t* idleKeyframes[3];
	const uint16_t* walkingKeyframes[10];
};

#endif
