/*
 * LunAPC V0.01 2020-05-13
 * by AkmaFin
 */

#include <iostream>
#include "lunapc.h"

int main( int argc, char* args[] ) {
	maindata lunadata;
	
	std::cout << "LunAPC\r\n";
	GameInit(&lunadata);

	while(lunadata.gamestate != GAMESTATE_QUIT) {
		switch(lunadata.gamestate) {
			case GAMESTATE_RUNNING:
				GameLoop(&lunadata);
				break;
			case GAMESTATE_GAMEOVER:
				GameOver(&lunadata);
				break;
			default:
				MainMenu(&lunadata);
				break;
		}
	}
	
	GameClean(&lunadata);
}

void MainMenu(maindata *lunadata) {
	SDL_Event e;
/*	SDL_Texture *tex;
	SDL_Surface *surf;
	SDL_Color fg = {(Uint8)64, (Uint8)64, (Uint8)255, 255};
	SDL_Rect destrect = {0, 0, 100, 100};*/

	MapInit(lunadata);
	ClearScreen(lunadata);
	
/*	surf = TTF_RenderText_Solid(lunadata->font, "LunAPC", fg);
	tex = SDL_CreateTextureFromSurface(lunadata->mainrend, surf);
	SDL_RenderCopy(lunadata->mainrend, tex, NULL, &destrect);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
	SDL_RenderPresent(lunadata->mainrend);*/

	lunadata->IntroActive = 1;
	lunadata->map.MinGap = 17;
	lunadata->map.HscrollSpeed = 2;
	lunadata->SPRITE_ENA = 0;
	for(int i = 0; i < 40; i++) {
		MapGenerateColumn(lunadata);
		lunadata->map.NeedToShift = 1;
		MapScreenShift(lunadata);
	}

	DrawHighscore(lunadata);

	while(lunadata->gamestate == GAMESTATE_MENU) {
SDL_Delay(20);
		GameDrawScreen(lunadata);
		
		lunadata->GameOverSinTicker++;
		
		MapAdvanceMap(lunadata);
		MapScreenShift(lunadata);
		
		while(SDL_PollEvent(&e)) {

			switch(e.type) {

				case SDL_KEYDOWN:	
					switch(e.key.keysym.scancode) {

						case SDL_SCANCODE_ESCAPE:
							lunadata->gamestate = GAMESTATE_QUIT;
							break;
							
						case SDL_SCANCODE_RCTRL:
							lunadata->gamestate = GAMESTATE_RUNNING;
							break;
							
						default:
							break;
					}
					break;

				case SDL_QUIT:
					lunadata->gamestate = GAMESTATE_QUIT;
					break;

				default:
					break;
			}
		}
	}
}

void GameLoop(maindata *lunadata) {
	SDL_Event e;
	int pos;
	
	lunadata->IntroActive = 0;
	MapInit(lunadata);
	PlayerInit(lunadata);
	EnemiesInit(lunadata);
	HudInit(lunadata);
	GenerateStars(lunadata);

	while(lunadata->gamestate == GAMESTATE_RUNNING) {
SDL_Delay(20);
		GameDrawScreen(lunadata);
		
		ClearStars(lunadata);
		EnemiesDraw(lunadata);
		BulletsClear(lunadata);
		BulletsUpdate(lunadata);
		MapAdvanceMap(lunadata);
		
		MapScreenShift(lunadata);
		BulletsDraw(lunadata);
		DrawStars(lunadata);
		EnemiesUpdate(lunadata);
				
		while(SDL_PollEvent(&e)) {

			switch(e.type) {

				case SDL_KEYDOWN:	
					switch(e.key.keysym.scancode) {

						case SDL_SCANCODE_ESCAPE:
							lunadata->gamestate = GAMESTATE_QUIT;
							break;
							
						case SDL_SCANCODE_KP_8:
							if(lunadata->player.PlayerY[1] >= 0x34) {
								pos = ((lunadata->player.PlayerY[1] << 8) | (lunadata->player.PlayerY[0])) - ((lunadata->player.PlayerSpeedY[1] << 8) | (lunadata->player.PlayerSpeedY[0]));
								lunadata->player.PlayerY[0] = pos & 0xFF;
								lunadata->player.PlayerY[1] = pos >> 8;
							}
							break;

						case SDL_SCANCODE_KP_2:
							if(lunadata->player.PlayerY[1] < 0xDE) {
								pos = ((lunadata->player.PlayerY[1] << 8) | (lunadata->player.PlayerY[0])) + ((lunadata->player.PlayerSpeedY[1] << 8) | (lunadata->player.PlayerSpeedY[0]));
								lunadata->player.PlayerY[0] = pos & 0xFF;
								lunadata->player.PlayerY[1] = pos >> 8;
							}
							break;

						case SDL_SCANCODE_KP_4:
							if((lunadata->player.PlayerX[2]) || ((lunadata->player.PlayerX[2] == 0) && (lunadata->player.PlayerX[1] >= 0x24))) {
								pos = ((lunadata->player.PlayerX[2] << 16) | (lunadata->player.PlayerX[1] << 8) | (lunadata->player.PlayerX[0])) - ((lunadata->player.PlayerSpeedX[1] << 8) | (lunadata->player.PlayerSpeedX[0]));
								lunadata->player.PlayerX[0] = pos & 0xFF;
								lunadata->player.PlayerX[1] = (pos >> 8) & 0xFF;
								lunadata->player.PlayerX[2] = pos >> 16;
							}
							break;

						case SDL_SCANCODE_KP_6:
							if((lunadata->player.PlayerX[2] == 0) || ((lunadata->player.PlayerX[2]) && (lunadata->player.PlayerX[1] < 0x36))) {
								pos = ((lunadata->player.PlayerX[2] << 16) | (lunadata->player.PlayerX[1] << 8) | (lunadata->player.PlayerX[0])) + ((lunadata->player.PlayerSpeedX[1] << 8) | (lunadata->player.PlayerSpeedX[0]));
								lunadata->player.PlayerX[0] = pos & 0xFF;
								lunadata->player.PlayerX[1] = (pos >> 8) & 0xFF;
								lunadata->player.PlayerX[2] = pos >> 16;
							}
							break;

						case SDL_SCANCODE_RCTRL:
							if(lunadata->player.PlayerFireTimer[0] == 0) {
								lunadata->player.PlayerFireTimer[0] = lunadata->player.PlayerFireTimer[1];
								BulletsAdd(lunadata, lunadata->player.PlayerScreenX + 3, lunadata->player.PlayerScreenY + 1);
							}
							else
								lunadata->player.PlayerFireTimer[0]--;
							break;
							
						default:
							break;
					}
					break;

				case SDL_QUIT:
					lunadata->gamestate = GAMESTATE_QUIT;
					break;

				default:
					break;
			}
		}
		
		PlayerUpdate(lunadata);
		PlayerDraw(lunadata);
		
		if(lunadata->player.PlayerIsDead)
			lunadata->gamestate = GAMESTATE_GAMEOVER;
		
		if((lunadata->player.PowerUpActive) && ((lunadata->ZP_COUNTER & 7) == 0))
			HudDecPower(lunadata);
		
		lunadata->ZP_COUNTER++;
	}
}

void GameOver(maindata *lunadata) {
	SDL_Event e;

	lunadata->map.HscrollSpeed = 0;
	lunadata->DeathAnimIndex = 0;
	CheckForHighscore(lunadata);
	
	while(lunadata->gamestate == GAMESTATE_GAMEOVER) {
SDL_Delay(20);
		GameDrawScreen(lunadata);
		
		BulletsClear(lunadata);
		BulletsUpdate(lunadata);
		
		ClearStars(lunadata);
		MapAdvanceMap(lunadata);
		MapScreenShift(lunadata);
		BulletsDraw(lunadata);
		DrawStars(lunadata);
		
		EnemiesUpdate(lunadata);
		EnemiesDraw(lunadata);
		
		while(SDL_PollEvent(&e)) {

			switch(e.type) {

				case SDL_KEYDOWN:	
					switch(e.key.keysym.scancode) {

						case SDL_SCANCODE_ESCAPE:
							lunadata->gamestate = GAMESTATE_QUIT;
							break;
							
						case SDL_SCANCODE_RCTRL:
							lunadata->gamestate = GAMESTATE_MENU;
							break;
							
						default:
							break;
					}
					break;

				case SDL_QUIT:
					lunadata->gamestate = GAMESTATE_QUIT;
					break;

				default:
					break;
			}
		}
	}
}

void GameInit(maindata *lunadata) {
	SDL_Surface *surf;
	SDL_Rect rect = {16, 0, 624, 400};

	SDL_Init(SDL_INIT_VIDEO);
	lunadata->mainwin = SDL_CreateWindow("LunAPC", 0, 0, 656, 400, SDL_WINDOW_SHOWN);
	lunadata->mainrend = SDL_CreateRenderer(lunadata->mainwin, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetClipRect(lunadata->mainrend, &rect);

	TTF_Init();
	lunadata->font=TTF_OpenFont("assets/Aileron-SemiBold.ttf", 32);
	
	IMG_Init(IMG_INIT_PNG);
	surf = IMG_Load("assets/font.png");
	lunadata->gamefonttex = SDL_CreateTextureFromSurface(lunadata->mainrend, surf);
	SDL_FreeSurface(surf);
	surf = IMG_Load("assets/sprites.png");
	lunadata->gamespritetex = SDL_CreateTextureFromSurface(lunadata->mainrend, surf);
	SDL_FreeSurface(surf);

	lunadata->gamestate = GAMESTATE_MENU;
	RandomInit(lunadata);
	for(int i = 0; i < MAX_BULLETS; i++)
		lunadata->bullets.BulletType[i] = 0;
}

void GameClean(maindata *lunadata) {
	SDL_DestroyTexture(lunadata->gamefonttex);
	SDL_DestroyTexture(lunadata->gamespritetex);
	IMG_Quit();
	TTF_CloseFont(lunadata->font);
	TTF_Quit();
	SDL_DestroyRenderer(lunadata->mainrend);
	SDL_DestroyWindow(lunadata->mainwin);
	SDL_Quit();
}

void GameDrawScreen(maindata *lunadata) {
	SDL_Rect chsrc = {0, 0, FONTTILE_WIDTH, FONTTILE_HEIGHT}, chdest = {0, 0, FONTTILE_WIDTH, FONTTILE_HEIGHT};
	SDL_Rect spsrc = {0, 0, SPRITETILE_WIDTH, SPRITETILE_HEIGHT}, spdest = {0, 0, SPRITETILE_WIDTH, SPRITETILE_HEIGHT};
	int ch, sp;

	SDL_SetRenderDrawColor(lunadata->mainrend,0,0,0,SDL_ALPHA_OPAQUE);
	SDL_RenderClear(lunadata->mainrend);
	SDL_SetRenderDrawBlendMode(lunadata->mainrend, SDL_BLENDMODE_NONE);
	
	for(int y = 0; y < 25; y++) {
		for( int x = 0; x < 40; x++) {
			ch = lunadata->SCREEN[y * 40 + x];
			chsrc.x = (ch % 32) * FONTTILE_WIDTH;
			chsrc.y = (ch / 32) * FONTTILE_HEIGHT;
			chdest.x = x * FONTTILE_WIDTH + lunadata->map.Hscroll * 2;
			chdest.y = y * FONTTILE_HEIGHT;
			SDL_RenderCopy(lunadata->mainrend, lunadata->gamefonttex, &chsrc, &chdest);
		}
	}
	
	SDL_SetRenderDrawBlendMode(lunadata->mainrend, SDL_BLENDMODE_BLEND);

	ch = 128;
	for(int i = 7; i >= 0; i--) {
		if(lunadata->SPRITE_ENA & ch) {
			sp = lunadata->SPRITE_PTRS[i] - 64;
			spsrc.x = (sp % 8) * SPRITETILE_WIDTH;
			spsrc.y = (sp / 8) * SPRITETILE_HEIGHT;
			spdest.x = (lunadata->SPRITE_X[i] - 0x14) * 2;
			spdest.y = (lunadata->SPRITE_Y[i] - 0x33) * 2;
			SDL_RenderCopy(lunadata->mainrend, lunadata->gamespritetex, &spsrc, &spdest);
		}
		ch = ch >> 1;
	}
	
	SDL_RenderPresent(lunadata->mainrend);
}

void ClearScreen(maindata *lunadata) {
	for(int i = 0; i < 1000; i++)
		lunadata->SCREEN[i] = 0;
}

void ClearColor(struct maindata *lunadata) {
	for(int i = 0; i < 1000; i++)
		lunadata->COLORRAM[i] = 0;
}

void RandomInit(struct maindata *lunadata) {
	lunadata->RandomNum = 0x62;
}

void RandomGet(struct maindata *lunadata) {
	lunadata->RandomNumState ^= lunadata->RandomNumState << 13;
	lunadata->RandomNumState ^= lunadata->RandomNumState >> 17;
	lunadata->RandomNumState ^= lunadata->RandomNumState << 5;
	lunadata->RandomNum = lunadata->RandomNumState & 0xFF;
	if(lunadata->RandomNum > 0x80)
		lunadata->RandomNum -= 0x100;
}

void DrawHighscore(struct maindata *lunadata) {
}

void GenerateStars(struct maindata *lunadata) {
}

void DrawStars(struct maindata *lunadata) {
}

void ClearStars(struct maindata *lunadata) {
}

void CheckForHighscore(struct maindata *lunadata) {
}

void StopSounds(struct maindata *lunadata) {
}
