#ifndef INC_ENEMY_H_
#define INC_ENEMY_H_

#include "images.h"
#include "lcd.h"
#include "kirby.h"

// 32x28 max sprite
extern const uint8_t ENEMY_WIDTH;
extern const uint8_t ENEMY_HEIGHT;

enum EnemyState {
	ENEMY_IDLE,
	ENEMY_ATTACKING
};

extern void Enemy_displayEnemyFacingLeft(uint8_t fullHeight, uint16_t kirbyX, uint8_t spriteWidth, uint8_t spriteHeight, uint8_t spriteOffset, const uint16_t* frame, uint16_t xPos, uint16_t yPos);

extern void Enemy_updateEnemy();
extern void Enemy_eraseEnemy();
extern void Enemy_eraseBoomerang();

extern uint8_t showEnemy;

#endif /* INC_ENEMY_H_ */
