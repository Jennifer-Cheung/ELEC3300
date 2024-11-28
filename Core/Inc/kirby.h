#ifndef __KIRBY_H
#define __KIRBY_H

#include <stdint.h>

extern const uint8_t KIRBY_WIDTH;
extern const uint8_t KIRBY_HEIGHT;
extern const uint16_t SCREEN_WIDTH;
extern const uint16_t SCREEN_HEIGHT;
extern const uint8_t TILE_LENGTH;

//extern uint16_t* idleKeyframes[3];
//extern uint16_t* walkingKeyframes[10];
//extern uint16_t* floatingUpKeyframes[5];
//extern uint16_t* floatingDownKeyframes[2];
//extern uint16_t* jumpingKeyframes[1];
//extern uint16_t* fallingKeyframes[8];
//extern uint16_t* spittingKeyframes[5];
//extern uint16_t* swallowedIdleKeyframes[3];
//extern uint16_t* swallowedJumpKeyframes[1];
//extern uint16_t* swallowedFallKeyframes[5];
//extern uint16_t* swallowingKeyframes[4];
//extern uint16_t* swallowedWalkKeyframes[10];

enum Direction {
	RIGHT,
	LEFT
};

enum State {
	SPITTING,
	SWALLOWING,
	JUMPING,
	FLOATING_UP,
	SWALLOWED_JUMP,
	FALLING,
	FLOATING_DOWN,
	SWALLOWED_FALL,
	WALKING,
	SWALLOWED_WALK,
	IDLE,
	SWALLOWED_IDLE
};

enum Ability {
	NORMAL,
	CUTTER
};

struct Kirby {
	uint16_t xPos;
	uint16_t yPos;
	uint8_t hasSwallowed;
	uint8_t isFloating;
	uint8_t enableUp;
	uint8_t enableX;
	enum Direction direction;
	enum State state;
	uint8_t currentFrame;
	uint16_t remainingTicks;
	enum State previousState;
	enum Ability ability;

	/* This is locked in updateState and unlocked in renderSprite when timeout is reached
	 * Affected states:
	 * - spitting
	 * - jumping
	 * - swallowed jump
	 * */
	uint8_t enableStateChange;
};

/* These will not be called directly from outside. Only for internal use. */
extern void Kirby_movePlatform(uint16_t kirbyX);
extern void Kirby_redrawColumnLeft(struct Kirby* kirby);
extern void Kirby_redrawColumnRight(struct Kirby* kirby);
extern void Kirby_redrawRowUp(struct Kirby* kirby);
extern void Kirby_redrawRowDown(struct Kirby* kirby);
extern void Kirby_eraseSprite(struct Kirby* kirby);

extern void Kirby_displayKirbyFacingRight(uint8_t spriteWidth, uint8_t spriteHeight, uint8_t spriteOffset, uint16_t* frame, uint16_t xPos, uint16_t yPos);
extern void Kirby_displayKirbyFacingLeft(uint8_t spriteWidth, uint8_t spriteHeight, uint8_t spriteOffset, uint16_t* frame, uint16_t xPos, uint16_t yPos);

extern int Kirby_isOnGround(const struct Kirby* kirby);
extern int Kirby_isFalling(const struct Kirby* kirby);

// Returns the pixel directly below the sprite
extern void Kirby_getPointBelow(const struct Kirby* kirby, uint16_t* pointXPos, uint16_t* pointYPos);
// Returns the pixel to the left of the sprite tile
extern void Kirby_getPointLeft(const struct Kirby* kirby, uint16_t* pointXPos, uint16_t* pointYPos);
// Returns the pixel to the right of the sprite tile
extern void Kirby_getPointRight(const struct Kirby* kirby, uint16_t* pointXPos, uint16_t* pointYPos);

/* Public functions */
extern void Kirby_updateState(struct Kirby* kirby, uint8_t inputB, uint8_t inputA, uint8_t inputLeft, uint8_t inputRight); //[Waiting for checking]

extern void Kirby_moveX(struct Kirby* kirby, enum Direction direction);
extern void Kirby_moveY(struct Kirby* kirby);

extern void Kirby_renderSprite(struct Kirby* kirby);

extern void Kirby_onSwallow();

extern uint8_t Kirby_checkWin(struct Kirby* kirby);
extern uint8_t Kirby_checkLose(struct Kirby* kirby);

#endif // __KIRBY_H
