#include "lunapc.h"

void HudInit(struct maindata *lunadata) {
	for(int i = 0; i < 40; i++) {
		lunadata->SCREEN[24 * 40 + i] = lunadata->hud.HUDDATA[i];
	}
}

void HudAddScore(struct maindata *lunadata) {
}

void HudDisplayWaveNumber(struct maindata *lunadata) {
	char buf[4];
	
	snprintf(buf, 4, "%03d", lunadata->enemies.CurrentWaveNumber);
	lunadata->SCREEN[24 * 40 + 36] = buf[0];
	lunadata->SCREEN[24 * 40 + 37] = buf[1];
	lunadata->SCREEN[24 * 40 + 38] = buf[2];
}

void HudDecPower(struct maindata *lunadata) {
	lunadata->player.PlayerPower--;
	if(lunadata->player.PlayerPower == 0) {
		lunadata->player.PlayerFireTimer[1] = 10;
		lunadata->player.PowerUpActive = 0;
	}
	HudDrawPower(lunadata);
}

void HudAddPower(struct maindata *lunadata) {
	lunadata->player.PlayerPower++;
	if(lunadata->player.PlayerPower >= 36) {
		lunadata->player.PlayerPower = 36;
		lunadata->player.PowerUpActive = 1;
		lunadata->player.PlayerFireTimer[1] = 5;
	}
	HudDrawPower(lunadata);
}

void HudDrawPower(struct maindata *lunadata) {
	int power;
	
	power = lunadata->player.PlayerPower;
	for(int i = 0; i < 9; i++) {
		if(power <= 0) {
			lunadata->SCREEN[24 * 40 + 21 + i] = 0;
			power = 0;
		} else if (power >= 4) {
			lunadata->SCREEN[24 * 40 + 21 + i] = 30;
		} else {
			lunadata->SCREEN[24 * 40 + 21 + i] = 26 + power;
		}
		
		power -= 4;
	}
}
