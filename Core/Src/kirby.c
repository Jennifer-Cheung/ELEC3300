#include <stdio.h>
#include "kirby.h"
#include "lcd.h"
#include "images.h"
#include "kirbySprites.h"
#include "tiles.h"
#include "enemy.h"

const uint8_t KIRBY_WIDTH = 35;
const uint8_t KIRBY_HEIGHT = 32;
const uint16_t SCREEN_WIDTH = 320;
const uint16_t SCREEN_HEIGHT = 216;
const uint8_t TILE_LENGTH = 24;

uint8_t IDLE_WIDTH = 24;
uint8_t IDLE_HEIGHT = 22;
uint8_t IDLE_OFFSET = 5;

uint8_t WALKING_WIDTH = 26;
uint8_t WALKING_HEIGHT = 23;
uint8_t WALKING_OFFSET = 4;

uint8_t FLOATING_UP_WIDTH = 33;
uint8_t FLOATING_UP_HEIGHT = 32;
uint8_t FLOATING_UP_OFFSET = 1;

uint8_t FLOATING_DOWN_WIDTH = 33;
uint8_t FLOATING_DOWN_HEIGHT = 32;
uint8_t FLOATING_DOWN_OFFSET = 1;

uint8_t JUMPING_WIDTH = 24;
uint8_t JUMPING_HEIGHT = 24;
uint8_t JUMPING_OFFSET = 5;

uint8_t FALLING_WIDTH = 28;
uint8_t FALLING_HEIGHT = 25;
uint8_t FALLING_OFFSET = 3;

uint8_t SPITTING_WIDTH = 31;
uint8_t SPITTING_HEIGHT = 27;
uint8_t SPITTING_OFFSET = 2;

uint8_t SWALLOWED_IDLE_WIDTH = 31;
uint8_t SWALLOWED_IDLE_HEIGHT = 27;
uint8_t SWALLOWED_IDLE_OFFSET = 2;

uint8_t SWALLOWED_JUMP_WIDTH = 32;
uint8_t SWALLOWED_JUMP_HEIGHT = 30;
uint8_t SWALLOWED_JUMP_OFFSET = 1;

uint8_t SWALLOWED_FALL_WIDTH = 32;
uint8_t SWALLOWED_FALL_HEIGHT = 30;
uint8_t SWALLOWED_FALL_OFFSET = 1;

uint8_t SWALLOWING_WIDTH = 28;
uint8_t SWALLOWING_HEIGHT = 29;
uint8_t SWALLOWING_OFFSET = 3;

uint8_t SWALLOWED_WALK_WIDTH = 31;
uint8_t SWALLOWED_WALK_HEIGHT = 30;
uint8_t SWALLOWED_WALK_OFFSET = 2;

uint16_t* idleKeyframes[1] = {
		KIRBY_IDLE_SPRITE_0,
//		KIRBY_IDLE_SPRITE_1,
//		KIRBY_IDLE_SPRITE_2
};
uint16_t* walkingKeyframes[5] = {
//		KIRBY_WALKING_SPRITE_0,
		KIRBY_WALKING_SPRITE_1,
//		KIRBY_WALKING_SPRITE_2,
		KIRBY_WALKING_SPRITE_3,
//		KIRBY_WALKING_SPRITE_4,
		KIRBY_WALKING_SPRITE_5,
//		KIRBY_WALKING_SPRITE_6,
		KIRBY_WALKING_SPRITE_7,
//		KIRBY_WALKING_SPRITE_8,
		KIRBY_WALKING_SPRITE_9
};
uint16_t* floatingUpKeyframes[3] = {
		KIRBY_FLOATING_SPRITE_1,
//		KIRBY_FLOATING_SPRITE_2,
		KIRBY_FLOATING_SPRITE_3,
//		KIRBY_FLOATING_SPRITE_4,
		KIRBY_FLOATING_SPRITE_5
};
uint16_t* floatingDownKeyframes[1] = {
//		KIRBY_FLOATING_SPRITE_0,
		KIRBY_FLOATING_SPRITE_1
};
uint16_t* jumpingKeyframes[1] = {
		KIRBY_JUMPING_SPRITE
};
uint16_t* fallingKeyframes[4] = {
//		KIRBY_FALLING_SPRITE_0,
		KIRBY_FALLING_SPRITE_1,
//		KIRBY_FALLING_SPRITE_2,
		KIRBY_FALLING_SPRITE_3,
//		KIRBY_FALLING_SPRITE_4,
		KIRBY_FALLING_SPRITE_5,
//		KIRBY_FALLING_SPRITE_6,
		KIRBY_FALLING_SPRITE_7
};
uint16_t* spittingKeyframes[3] = {
		KIRBY_SPITTING_SPRITE_0,
//		KIRBY_SPITTING_SPRITE_1,
		KIRBY_SPITTING_SPRITE_2,
//		KIRBY_SPITTING_SPRITE_3,
		KIRBY_SPITTING_SPRITE_4
};
uint16_t* swallowedIdleKeyframes[1] = {
		KIRBY_SWALLOWED_IDLE_SPRITE_0,
//		KIRBY_SWALLOWED_IDLE_SPRITE_1,
//		KIRBY_SWALLOWED_IDLE_SPRITE_2
};
uint16_t* swallowedJumpKeyframes[1] = {
		KIRBY_SWALLOWED_JUMP
};
uint16_t* swallowedFallKeyframes[3] = {
		KIRBY_SWALLOWED_FALL_0,
//		KIRBY_SWALLOWED_FALL_1,
		KIRBY_SWALLOWED_FALL_2,
//		KIRBY_SWALLOWED_FALL_3,
		KIRBY_SWALLOWED_FALL_4
};
uint16_t* swallowingKeyframes[2] = {
//		KIRBY_SWALLOWING_0,
		KIRBY_SWALLOWING_1,
//		KIRBY_SWALLOWING_2,
		KIRBY_SWALLOWING_3
};
uint16_t* swallowedWalkKeyframes[5] = {
//		KIRBY_SWALLOWED_WALK_0,
		KIRBY_SWALLOWED_WALK_1,
//		KIRBY_SWALLOWED_WALK_2,
		KIRBY_SWALLOWED_WALK_3,
//		KIRBY_SWALLOWED_WALK_4,
		KIRBY_SWALLOWED_WALK_5,
//		KIRBY_SWALLOWED_WALK_6,
		KIRBY_SWALLOWED_WALK_7,
//		KIRBY_SWALLOWED_WALK_8,
		KIRBY_SWALLOWED_WALK_9
};

uint16_t currentPos = 0;
/*
 * Moves the platform image one pixel off from the original position.
 *
 * uint16_t xPos: the updated x Position of the first column
 * */
void Kirby_movePlatform(uint16_t kirbyX) {
	uint16_t xPos = kirbyX - 160;
	if (currentPos < xPos) {
		// Scrolling to the right
		uint16_t targetCol = (xPos - 1) % 320;
		for (uint16_t row = 0; row < 216; row++) {
			LCD_DrawDot(targetCol, row, platformImg[882 * row + xPos + 320 - 1]);
		}
	} else if (currentPos > xPos) {
		// Scrolling to the left
		uint16_t targetCol = xPos % 320;
		for (uint16_t row = 0; row < 216; row++) {
			LCD_DrawDot(targetCol, row, platformImg[882 * row + xPos]);
		}
	} else {
		return;
	}
	currentPos = xPos;


	// Vertical scroll, starting from column = xPos
	  LCD_Write_Cmd(0x33);
	  LCD_Write_Data(0x00);
	  LCD_Write_Data(0x00);
	  LCD_Write_Data(0x01);
	  LCD_Write_Data(0x40);
	  LCD_Write_Data(0x00);
	  LCD_Write_Data(0x00);

	  LCD_Write_Cmd(0x37);
	  LCD_Write_Data((xPos % 320) >> 8);
	  LCD_Write_Data((xPos % 320) & 0xff);
}

void Kirby_redrawColumnLeft(struct Kirby* kirby) {
	// Redraw the first column
	for (int y = 0; y < KIRBY_HEIGHT; y++) {
	  uint16_t displayX = (kirby->xPos) % SCREEN_WIDTH;
	  uint16_t displayY = kirby->yPos + y;
	  uint16_t color = platformImg[882 * (displayY) + (kirby->xPos)];
	  LCD_DrawDot(displayX, displayY, color);
	}
}

void Kirby_redrawColumnRight(struct Kirby* kirby) {
	// Redraw the last column
	for (int y = 0; y < KIRBY_HEIGHT; y++) {
		uint16_t displayX = (kirby->xPos + KIRBY_WIDTH) % SCREEN_WIDTH;
		uint16_t displayY = kirby->yPos + y;
		uint16_t color = platformImg[882 * (displayY) + kirby->xPos + KIRBY_WIDTH];
		LCD_DrawDot(displayX, displayY, color);
	}
}

void Kirby_redrawRowUp(struct Kirby* kirby) {
	for (int x = 0; x < KIRBY_WIDTH; x++) {
		uint16_t displayX = (kirby->xPos + x) % SCREEN_WIDTH;
		uint16_t displayY = kirby->yPos;
		uint16_t color = platformImg[882 * displayY + kirby->xPos + x];
		LCD_DrawDot(displayX, displayY, color);
	}
}

void Kirby_redrawRowDown(struct Kirby* kirby) {
	for (int x = 0; x < KIRBY_WIDTH; x++) {
		uint16_t displayX = (kirby->xPos + x) % SCREEN_WIDTH;
		uint16_t displayY = kirby->yPos + KIRBY_HEIGHT - 1;
		uint16_t color = platformImg[882 * displayY + kirby->xPos + x];
		LCD_DrawDot(displayX, displayY, color);
	}
}

void Kirby_eraseSprite(struct Kirby* kirby) {
	for (int y = 0; y < KIRBY_HEIGHT; y++) {
		for (int x = 0; x < KIRBY_WIDTH; x++) {
			uint16_t displayX = (kirby->xPos + x) % SCREEN_WIDTH;
			uint16_t displayY = kirby->yPos + y;
			uint16_t color = platformImg[882 * displayY + kirby->xPos + x];
			LCD_DrawDot(displayX, displayY, color);
		}
	}
}

//void Kirby_displayKirbyFacingRight(const uint16_t* frame, uint16_t xPos, uint16_t yPos) {
//	for (int x = 0; x < 26; x++) {
//		uint16_t displayXPos = (xPos + x) % 320;
//		for (int y = 0; y < 23; y++) {
//			uint16_t displayYPos = yPos + y;
//			uint16_t color = frame[23 * x + y];
//			// Replacing color
//			if (color == 0x4B4E) {
//				uint16_t newColor = platformImg[882 * (displayYPos) + (xPos + x)];
//				color = newColor;
//			}
//			LCD_DrawDot(displayXPos, displayYPos, color);
//		}
//	}
//}
//
//void Kirby_displayKirbyFacingLeft(const uint16_t* frame, uint16_t xPos, uint16_t yPos) {
//	for (int x = 0; x < 26; x++) {
//		uint16_t displayXPos = (xPos + 25 - x) % 320;
//		for (int y = 0; y < 23; y++) {
//			uint16_t displayYPos = yPos + y;
//			uint16_t color = frame[23 * x + y];
//			// Replacing color
//			if (color == 0x4B4E) {
//				uint16_t newColor = platformImg[882 * (displayYPos) + (xPos + 25 - x)];
//				color = newColor;
//			}
//			LCD_DrawDot(displayXPos, displayYPos, color);
//		}
//	}
//}

void Kirby_displayKirbyFacingRight(uint8_t spriteWidth, uint8_t spriteHeight, uint8_t spriteOffset, uint16_t* frame, uint16_t xPos, uint16_t yPos) {
	for (int x = 0; x < spriteWidth; x++) {
	  uint16_t displayXPos = (xPos + spriteOffset + x) % SCREEN_WIDTH;
	  for (int y = spriteHeight - 1; y >= 0; y--) {
		  uint16_t displayYPos = yPos + y + (KIRBY_HEIGHT - spriteHeight);
		  uint16_t color = frame[y * spriteWidth + x];
		  if (color == 0x4B4E) {
			  uint16_t newColor = platformImg[882 * (displayYPos) + (xPos + spriteOffset + x)];
			  color = newColor;
		  }
		  LCD_DrawDot(displayXPos, displayYPos, color);
	  }
	}
}

void Kirby_displayKirbyFacingLeft(uint8_t spriteWidth, uint8_t spriteHeight, uint8_t spriteOffset, uint16_t* frame, uint16_t xPos, uint16_t yPos) {
	for (int x = 0; x < spriteWidth; x++) {
	  uint16_t displayXPos = (xPos + spriteOffset + spriteWidth - x) % SCREEN_WIDTH;
	  for (int y = spriteHeight - 1; y >= 0; y--) {
		  uint16_t displayYPos = yPos + y + (KIRBY_HEIGHT - spriteHeight);
		  uint16_t color = frame[y * spriteWidth + x];
		  if (color == 0x4B4E) {
			  uint16_t newColor = platformImg[882 * (displayYPos) + (xPos + spriteOffset + spriteWidth - x)];
			  color = newColor;
		  }
		  LCD_DrawDot(displayXPos, displayYPos, color);
	  }
	}
}

int Kirby_isOnGround(const struct Kirby* kirby) {
	// Checks if Kirby is on a ground tile
	uint16_t pointX;
	uint16_t pointY;
	Kirby_getPointBelow(kirby, &pointX, &pointY);
//	LCD_DrawDot(pointX, pointY, 0xf800); // For debug use
	if (Tiles_getTileType(pointX, pointY) == GROUND || kirby->xPos + 17 > 456 && kirby->xPos + 17 < 528) {
		return 1;
	}

	// Function still does not return, Kirby is not on ground
	return 0;
}

int Kirby_isFalling(const struct Kirby* kirby) {
	if (Kirby_isOnGround(kirby)) {
		return 0;
	} else {
		if (kirby->enableUp) {
			return 0;
		} else {
			return 1;
		}
	}
}

void Kirby_getPointBelow(const struct Kirby* kirby, uint16_t* pointXPos, uint16_t* pointYPos) {
	uint16_t xPos = kirby->xPos;
	uint16_t yPos = kirby->yPos;
	*pointXPos = xPos + 17;
	*pointYPos = yPos + KIRBY_HEIGHT;
}

void Kirby_getPointLeft(const struct Kirby* kirby, uint16_t* pointXPos, uint16_t* pointYPos) {
	uint16_t xPos = kirby->xPos;
	uint16_t yPos = kirby->yPos;
	*pointXPos = xPos + 5;
	*pointYPos = yPos + KIRBY_HEIGHT - 1;
}

void Kirby_getPointRight(const struct Kirby* kirby, uint16_t* pointXPos, uint16_t* pointYPos) {
	uint16_t xPos = kirby->xPos;
	uint16_t yPos = kirby->yPos;
	*pointXPos = xPos + KIRBY_WIDTH - 5;
	*pointYPos = yPos + KIRBY_HEIGHT - 1;
}

void Kirby_updateState(struct Kirby* kirby, uint8_t inputB, uint8_t inputA, uint8_t inputLeft, uint8_t inputRight) {
	kirby->previousState = kirby->state;

//	// Displays inputs
//	LCD_Clear(10, 210, 100, 20, 0xFFFF);
//	char buffer[8];
//	sprintf(buffer, "%c %c %c %c", inputB ? 'B' : ' ', inputA ? 'A' : ' ', inputLeft ? 'L' : ' ', inputRight ? 'R' : ' ');
//	LCD_DrawString(10, 210, buffer);

	/* Timeout */
//	if (kirby->state == SPITTING || kirby->state == SWALLOWING || kirby->state == JUMPING || kirby->state == FLOATING_UP || kirby->state == SWALLOWED_JUMP) {
//		if (kirby->remainingTicks == 0) {
//			kirby->state = IDLE;
//			kirby->enableUp = 0;
//			Kirby_updateState(kirby, inputB, inputA, inputLeft, inputRight);
//		}
//		return;
//	}
	if (!(kirby->enableStateChange)) {
		return;
	}

	/* B Button */
	if (inputB) {
		if (kirby->hasSwallowed) {
			kirby->state = SPITTING;
			kirby->hasSwallowed = 0;
			kirby->isFloating = 0;
			kirby->enableStateChange = 0;
		} else {
			kirby->state = SWALLOWING;
		}
		return;
	} else if (kirby->previousState == FLOATING_DOWN && Kirby_isOnGround(kirby)) {
		kirby->state = SPITTING;
		kirby->hasSwallowed = 0;
		kirby->isFloating = 0;
		kirby->enableStateChange = 0;
		return;
	}

	if (kirby->state == SWALLOWING && kirby->currentFrame == 1 && kirby->xPos > 316) {
		// Kirby has swallowed enemy
		kirby->hasSwallowed = 1;
		Kirby_onSwallow();
	}

	/* A Button */
	if (inputA) {
		if (kirby->hasSwallowed && !(kirby->isFloating)) {
			kirby->state = SWALLOWED_JUMP;
			kirby->enableUp = 1;
			kirby->enableStateChange = 0;
		} else if (Kirby_isOnGround(kirby)) {
			kirby->state = JUMPING;
			kirby->enableUp = 1;
			kirby->enableStateChange = 0;
		} else {
			kirby->state = FLOATING_UP;
			kirby->isFloating = 1;
			kirby->hasSwallowed = 1;
			kirby->enableUp = 1;
		}
		return;
	} else {
		kirby->enableUp = 0;
	}

	/* Falling */
	if (Kirby_isFalling(kirby)) {
		if (kirby->hasSwallowed == 0) {
			kirby->state = FALLING;
		} else if (kirby->isFloating) {
			kirby->state = FLOATING_DOWN;
		} else {
			kirby->state = SWALLOWED_FALL;
		}
		return;
	} else {
		kirby->isFloating = 0;
	}

	/* Arrow Keys */
	if (inputLeft || inputRight) {
		if (kirby->hasSwallowed) {
			kirby->state = SWALLOWED_WALK;
		} else {
			kirby->state = WALKING;
		}
		return;
	}

	/* Idle */
	if (kirby->hasSwallowed) {
		kirby->state = SWALLOWED_IDLE;
	} else {
		kirby->state = IDLE;
	}
	return;
}

static uint8_t moveYLock = 1;

void Kirby_moveX(struct Kirby* kirby, enum Direction direction) {
	/* Sets enableX */
	if (kirby->state == SPITTING || kirby->state == SWALLOWING) {
		kirby->enableX = 0;
	} else {
		kirby->enableX = 1;
	}

	if (kirby->enableX) {
		kirby->direction = direction;
		if (direction == LEFT) {
			// Checks if the left pixel is a ground tile
			uint16_t leftX = 0;
			uint16_t leftY = 0;
			Kirby_getPointLeft(kirby, &leftX, &leftY);
			if (Tiles_getTileType(leftX, leftY) == GROUND) {
				return;
			}

			// Checks the slope
			if (kirby->xPos + 17 > 456 && kirby->xPos + 17 < 528) {
				moveYLock = 0;
				if ((kirby->xPos + 17 - 455) % 3 == 0) {
					// Moves up
					Kirby_redrawRowDown(kirby);
					kirby->yPos--;
				}
			} else {
				moveYLock = 1;
			}

			Kirby_redrawColumnRight(kirby);
			if (kirby->xPos > 0) {
				if (kirby->xPos > 160 && kirby->xPos < 696) {
				  Kirby_movePlatform(kirby->xPos);
				}
				kirby->xPos--;
			}
		} else if (direction == RIGHT) {
			// Checks if the right pixel is a ground tile
			uint16_t rightX = 0;
			uint16_t rightY = 0;
			Kirby_getPointRight(kirby, &rightX, &rightY);
			if (Tiles_getTileType(rightX, rightY) == GROUND) {
				return;
			}

			// Checks the slope
			if (kirby->xPos + 17 > 456 && kirby->xPos + 17 < 528) {
				moveYLock = 0;
				if ((kirby->xPos + 17 - 456) % 3 == 0) {
					// Moves down
					Kirby_redrawRowUp(kirby);
					kirby->yPos++;
				}
			} else {
				moveYLock = 1;
			}

			Kirby_redrawColumnLeft(kirby);
			if (kirby->xPos < 829) {
			  if (kirby->xPos > 160 && kirby->xPos < 696) {
				  Kirby_movePlatform(kirby->xPos);
			  }
			  kirby->xPos++;
			}
		}
	}
}

void Kirby_moveY(struct Kirby* kirby) {
	if (moveYLock) {
		if (Kirby_isFalling(kirby)) {
			// Moves down
			Kirby_redrawRowUp(kirby);
			if (!Kirby_isOnGround(kirby)) {
				kirby->yPos++;
			}
		} else if (kirby->enableUp) {
			// Moves up
			Kirby_redrawRowDown(kirby);
			if (kirby->yPos > 0) {
				kirby->yPos--;
			}
		}
	}
}

void Kirby_renderSprite(struct Kirby* kirby) {
	enum Direction direction = kirby->direction;
	uint8_t* currentFrame = &kirby->currentFrame;
	uint16_t* remainingTicks = &kirby->remainingTicks;
	enum State currentState = kirby->state;
	enum State previousState = kirby->previousState;
	uint16_t xPos = kirby->xPos;
	uint16_t yPos = kirby->yPos;
	uint8_t* enableUp = &kirby->enableUp;
	uint8_t* enableStateChange = &kirby->enableStateChange;

	uint8_t spriteWidth = 0;
	uint8_t spriteHeight = 0;
	uint8_t spriteOffset = 0;

	// Displays state
//	LCD_Clear(10, 210, 100, 20, 0xFFFF);
//	char buffer[10];
//	if (currentState == IDLE) {
//		sprintf(buffer, "%s", "IDLE");
//	} else if (currentState == WALKING) {
//		sprintf(buffer, "%s", "WALK");
//	} else if (currentState == FLOATING_UP) {
//		sprintf(buffer, "%s", "FL_UP");
//	} else if (currentState == FLOATING_DOWN) {
//		sprintf(buffer, "%s", "FL_DN");
//	} else if (currentState == SPITTING) {
//		sprintf(buffer, "%s", "SPIT");
//	} else if (currentState == SWALLOWED_IDLE) {
//		sprintf(buffer, "%s", "SW_IDLE");
//	} else if (currentState == SWALLOWING) {
//		sprintf(buffer, "%s", "SWALLOW");
//	} else if (currentState == SWALLOWED_WALK) {
//		sprintf(buffer, "%s", "SW_WALK");
//	} else if (currentState == JUMPING) {
//		sprintf(buffer, "%s", "JUMP");
//	} else if (currentState == FALLING) {
//		sprintf(buffer, "%s", "FALL");
//	} else if (currentState == SWALLOWED_JUMP) {
//		sprintf(buffer, "%s", "SW_JUMP");
//	} else if (currentState == SWALLOWED_FALL) {
//		sprintf(buffer, "%s", "SW_FALL");
//	} else {
//		sprintf(buffer, "%s", "Unacc.");
//	}
//	LCD_DrawString(200, 210, buffer);

	uint8_t walkingTicks = 1;
	uint8_t floatingTicks = 1;
	uint8_t jumpingTicks = 10;
	uint8_t fallingTicks = 1;
	uint8_t spittingTicks = 1;
	uint8_t swallowedTicks = 2;
	uint8_t swallowingTicks = 1;

	if (previousState != currentState) {
		*currentFrame = 0;
		Kirby_eraseSprite(kirby);

		/* Initializes remaining ticks based on states */
		if (currentState == IDLE) {
			*remainingTicks = 250;
		} else if (currentState == WALKING) {
			*remainingTicks = walkingTicks;
		} else if (currentState == FLOATING_UP) {
			*remainingTicks = floatingTicks;
		} else if (currentState == FLOATING_DOWN) {
			*remainingTicks = floatingTicks;
		} else if (currentState == JUMPING) {
			*remainingTicks = jumpingTicks;
		} else if (currentState == FALLING) {
			*remainingTicks = fallingTicks;
		} else if (currentState == SPITTING) {
			*remainingTicks = spittingTicks;
		} else if (currentState == SWALLOWED_IDLE) {
			*remainingTicks = 250;
		} else if (currentState == SWALLOWED_JUMP) {
			*remainingTicks = jumpingTicks;
		} else if (currentState == SWALLOWED_FALL) {
			*remainingTicks = swallowedTicks;
		} else if (currentState == SWALLOWING) {
			*remainingTicks = swallowingTicks;
		} else if (currentState == SWALLOWED_WALK) {
			*remainingTicks = swallowedTicks;
		}
	}

	/* Renders sprite based on states */
	uint16_t* sprite = idleKeyframes[0];

	if (currentState == IDLE) {
//		if (*remainingTicks == 0) {
//			// Updates current frame
//			*currentFrame = (*currentFrame + 1) % 3;
//			// Updates remaining ticks
//			if (*currentFrame == 0) {
//				*remainingTicks = 2500;
//			} else {
//				*remainingTicks = 50;
//			}
//		}
		// Sets sprite and decrease remaining ticks
		sprite = idleKeyframes[0];
		spriteWidth = IDLE_WIDTH;
		spriteHeight = IDLE_HEIGHT;
		spriteOffset = IDLE_OFFSET;
	} else if (currentState == WALKING) {
		if (*remainingTicks == 0) {
			// Updates current frame
			*currentFrame = (*currentFrame + 1) % 5;
			// Updates remaining ticks
			*remainingTicks = walkingTicks;
		}
		// Sets sprite and decrease remaining ticks
		sprite = walkingKeyframes[*currentFrame];
		*remainingTicks = *remainingTicks - 1;
		spriteWidth = WALKING_WIDTH;
		spriteHeight = WALKING_HEIGHT;
		spriteOffset = WALKING_OFFSET;
	} else if (currentState == FLOATING_UP) {
		if (*remainingTicks == 0) {
			// Updates current frame
			*currentFrame = (*currentFrame + 1) % 3;
			// Updates remaining ticks
			*remainingTicks = floatingTicks;
		}
		// Sets sprite and decreases remaining ticks
		sprite = floatingUpKeyframes[*currentFrame];
		*remainingTicks = *remainingTicks - 1;
		spriteWidth = FLOATING_UP_WIDTH;
		spriteHeight = FLOATING_UP_HEIGHT;
		spriteOffset = FLOATING_UP_OFFSET;
	} else if (currentState == FLOATING_DOWN) {
//		if (*remainingTicks == 0) {
//			// Updates current frame
//			*currentFrame = !(*currentFrame);
//			// Updates remaining ticks
//			*remainingTicks = 20;
//		}
		// Sets sprite and decreases remaining ticks
		sprite = floatingDownKeyframes[0];
		spriteWidth = FLOATING_DOWN_WIDTH;
		spriteHeight = FLOATING_DOWN_HEIGHT;
		spriteOffset = FLOATING_DOWN_OFFSET;
	} else if (currentState == JUMPING) {
		if (*remainingTicks == 0) {
			// Timeout reached, disable jump
			*enableUp = 0;
			*enableStateChange = 1;
		}
		// Sets sprite and decreases remaining ticks
		sprite = jumpingKeyframes[*currentFrame];
		spriteWidth = JUMPING_WIDTH;
		spriteHeight = JUMPING_HEIGHT;
		spriteOffset = JUMPING_OFFSET;
		if (*remainingTicks > 0) {
			*remainingTicks = *remainingTicks - 1;
		}
	} else if (currentState == FALLING) {
		if (*remainingTicks == 0) {
			// Updates current frame
			if (*currentFrame < 3) {
				*currentFrame = *currentFrame + 1;
			}
			// Updates remaining ticks
			*remainingTicks = fallingTicks;
		}
		// Sets sprite and decreases remaining ticks
		sprite = fallingKeyframes[*currentFrame];
		*remainingTicks = *remainingTicks - 1;
		spriteWidth = FALLING_WIDTH;
		spriteHeight = FALLING_HEIGHT;
		spriteOffset = FALLING_OFFSET;
	} else if (currentState == SPITTING) {
		if (*remainingTicks == 0) {
			// Updates currentFrame
			if (*currentFrame < 2) {
				*currentFrame = *currentFrame + 1;
			} else {
				// Timeout reached
				*enableStateChange = 1;
			}
			// Updates remaining ticks
			*remainingTicks = spittingTicks;
		}
		// Sets sprite and decreases remaining ticks
		sprite = spittingKeyframes[*currentFrame];
		*remainingTicks = *remainingTicks - 1;
		spriteWidth = SPITTING_WIDTH;
		spriteHeight = SPITTING_HEIGHT;
		spriteOffset = SPITTING_OFFSET;
	} else if (currentState == SWALLOWED_IDLE) {
//		if (*remainingTicks == 0) {
//			// Updates current frame
//			*currentFrame = (*currentFrame + 1) % 3;
//			// Updates remaining ticks
//			if (*currentFrame == 0) {
//				*remainingTicks = 2500;
//			} else {
//				*remainingTicks = 50;
//			}
//		}
		// Sets sprite and decrease remaining ticks
		sprite = swallowedIdleKeyframes[0];
		*remainingTicks = *remainingTicks - 1;
		spriteWidth = SWALLOWED_IDLE_WIDTH;
		spriteHeight = SWALLOWED_IDLE_HEIGHT;
		spriteOffset = SWALLOWED_IDLE_OFFSET;
	} else if (currentState == SWALLOWED_JUMP) {
		if (*remainingTicks == 0) {
			// Timeout reached, disable jump
			*enableUp = 0;
			*enableStateChange = 1;
		}
		// Sets sprite and decreases remaining ticks
		sprite = swallowedJumpKeyframes[0];
		spriteWidth = SWALLOWED_JUMP_WIDTH;
		spriteHeight = SWALLOWED_JUMP_HEIGHT;
		spriteOffset = SWALLOWED_JUMP_OFFSET;
		if (*remainingTicks > 0) {
			*remainingTicks = *remainingTicks - 1;
		}
	} else if (currentState == SWALLOWED_FALL) {
		if (*remainingTicks == 0) {
			// Updates current frame
			if (*currentFrame < 2) {
				*currentFrame = *currentFrame + 1;
			}
			// Updates remaining ticks
			*remainingTicks = swallowedTicks;
		}
		// Sets sprite and decreases remaining ticks
		sprite = swallowedFallKeyframes[*currentFrame];
		*remainingTicks = *remainingTicks - 1;
		spriteWidth = SWALLOWED_FALL_WIDTH;
		spriteHeight = SWALLOWED_FALL_HEIGHT;
		spriteOffset = SWALLOWED_FALL_OFFSET;
	} else if (currentState == SWALLOWED_WALK) {
		if (*remainingTicks == 0) {
			// Updates current frame
			*currentFrame = (*currentFrame + 1) % 5;
			// Updates remaining ticks
			*remainingTicks = swallowedTicks;
		}
		// Sets sprite and decrease remaining ticks
		sprite = swallowedWalkKeyframes[*currentFrame];
		*remainingTicks = *remainingTicks - 1;
		spriteWidth = SWALLOWED_WALK_WIDTH;
		spriteHeight = SWALLOWED_WALK_HEIGHT;
		spriteOffset = SWALLOWED_WALK_OFFSET;
	} else if (currentState == SWALLOWING) {
		if (*remainingTicks == 0) {
			// Updates currentFrame
			if (*currentFrame == 0) {
				*currentFrame = 1;
			}
			// Updates remaining ticks
			*remainingTicks = swallowingTicks;
		}
		// Sets sprite and decreases remaining ticks
		sprite = swallowingKeyframes[*currentFrame];
		*remainingTicks = *remainingTicks - 1;
		spriteWidth = SWALLOWING_WIDTH;
		spriteHeight = SWALLOWING_HEIGHT;
		spriteOffset = SWALLOWING_OFFSET;
	}

	if (direction == LEFT) {
		Kirby_displayKirbyFacingLeft(spriteWidth, spriteHeight, spriteOffset, sprite, xPos, yPos);
	} else {
		Kirby_displayKirbyFacingRight(spriteWidth, spriteHeight, spriteOffset, sprite, xPos, yPos);
	}
}

void Kirby_onSwallow() {
	// Erases sir kibble and the boomerang
	Enemy_eraseBoomerang();
	Enemy_eraseEnemy();
	showEnemy = 0;
}

uint8_t Kirby_checkWin(struct Kirby* kirby) {
	return kirby->xPos > 744;
}

uint8_t Kirby_checkLose(struct Kirby* kirby) {
	return kirby->yPos > 192;
}
