#ifndef player_h
#define player_h

struct player {
	int PlayerFrames[3] = {64, 65, 66};
	int PlayerAnimIndex = 0, PlayerAnimTimer[2] = {0, 4};
	int PlayerX[3] = {0, 0, 0}, PlayerY[2] = {0, 0};
	int PlayerScreenX = 0, PlayerScreenY = 0;
	int PlayerSpeedX[2] = {0, 2}, PlayerSpeedY[2] = {0, 2};
	int PlayerFireTimer[2] = {0, 10};
	int PlayerDrawn = 0, PlayerPower = 0;
	int PowerUpActive = 0, PlayerIsDead = 0;
};

void PlayerInit(maindata *lunadata);
void PlayerUpdate(maindata *lunadata);
void PlayerGetCollision(maindata *lunadata);
void PlayerDraw(maindata *lunadata);

#endif
