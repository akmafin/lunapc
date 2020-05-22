#include "lunapc.h"
#include <iostream>
void PlayerInit(maindata *lunadata) {
	lunadata->SPRITE_ENA = 0xFD;
	lunadata->player.PlayerX[1] = 128;
	lunadata->player.PlayerY[1] = 128;
	lunadata->player.PlayerX[0] = 0;
	lunadata->player.PlayerX[2] = 0;
	lunadata->player.PlayerY[0] = 0;
	
	lunadata->player.PlayerIsDead = 0;
	lunadata->player.PlayerScreenX = 0;
	lunadata->player.PlayerScreenY = 0;
	lunadata->player.PlayerPower = 0;
	lunadata->player.PowerUpActive = 0;
	
	lunadata->player.PlayerDrawn = 8;
	lunadata->player.PlayerFireTimer[0] = 10;
	lunadata->player.PlayerFireTimer[1] = 10;
}

void PlayerUpdate(maindata *lunadata) {
	lunadata->player.PlayerAnimTimer[0]--;
	if(lunadata->player.PlayerAnimTimer[0] < 0) {
		lunadata->player.PlayerAnimTimer[0] = lunadata->player.PlayerAnimTimer[1];
		lunadata->player.PlayerAnimIndex++;
		if(lunadata->player.PlayerAnimIndex == 3)
			lunadata->player.PlayerAnimIndex = 0;
	}
	PlayerGetCollision(lunadata);
}

void PlayerGetCollision(maindata *lunadata) {
	int ch, x, y;
	
	y = lunadata->player.PlayerY[1] - 0x34;
	y = y >> 3;
	if(y < 0)
		y = 0;
	if(y >= 0x1A)
		y = 0;
	lunadata->player.PlayerScreenY = y;

	x = ((lunadata->player.PlayerX[2] << 8) | lunadata->player.PlayerX[1]) - 0x1C - lunadata->map.Hscroll;
	x = x >> 3;
	if(x < 0)
		x = 0;
	if(x >= 39)
		x = 0;
	lunadata->player.PlayerScreenX = x;
	
	x = lunadata->player.PlayerScreenX + 41;
	y = lunadata->player.PlayerScreenY * 40;
	ch = lunadata->SCREEN[y + x];
	ch = ch | (lunadata->SCREEN[y + x + 1]);
	x += 39;
	ch = ch | (lunadata->SCREEN[y + x]);
	ch = ch | (lunadata->SCREEN[y + x + 1]);

	if(ch > 0) {
		lunadata->player.PlayerIsDead = 1;
	} else {
		// sprite collision
	}
}

void PlayerDraw(maindata *lunadata) {
	lunadata->SPRITE_Y[0] = lunadata->player.PlayerY[1];
	lunadata->SPRITE_X[0] = (lunadata->player.PlayerX[2] << 8) | lunadata->player.PlayerX[1];
	lunadata->SPRITE_PTRS[0] = lunadata->player.PlayerFrames[lunadata->player.PlayerAnimIndex];
	if(lunadata->player.PlayerDrawn != 0)
		lunadata->player.PlayerDrawn --;
}
