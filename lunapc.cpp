/*
 * LunAPC V0.80 2020-07-15
 * by AkmaFin
 */

#define SWVER			0
#define SWREV			80

#include <iostream>
#include <chrono>
#include <cmath>
#include "lunapc.h"

int main( int argc, char* args[] ) {
	maindata lunadata;
	
	std::cout << "LunAPC V" << SWVER << "." << SWREV << "\r\n";
	std::cout << "Running on " << SDL_GetPlatform() << "\n";
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
	return 0;
}

void MainMenu(maindata *lunadata) {
	SDL_Event e;

	Mix_PlayMusic(lunadata->sound.musicgame, -1);
	MapInit(lunadata);
	ClearScreen(lunadata);

	lunadata->IntroActive = 1;
	lunadata->map.MinGap = 17;
	lunadata->map.HscrollSpeed = 2;
	lunadata->SPRITE_ENA = 0;
	lunadata->MessageIndex = lunadata->MessageLength - 35;
	for(int i = 0; i < 8; i++)
		lunadata->SPRITE_PTRS[i] = 0;
	for(int i = 0; i < 40; i++) {
		MapGenerateColumn(lunadata);
		lunadata->map.NeedToShift = 1;
		MapScreenShift(lunadata);
	}

	DrawHighscore(lunadata);

	while(lunadata->gamestate == GAMESTATE_MENU) {
		GameDelay(lunadata);
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
						case SDL_SCANCODE_LCTRL:
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
	const Uint8 *keystate;

	lunadata->IntroActive = 0;
	MapInit(lunadata);
	PlayerInit(lunadata);
	EnemiesInit(lunadata);
	HudInit(lunadata);
	GenerateStars(lunadata);
	Mix_PlayMusic(lunadata->sound.musicgame, -1);

	while(lunadata->gamestate == GAMESTATE_RUNNING) {
		GameDelay(lunadata);
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
		lunadata->player.Joy = 0;

		while(SDL_PollEvent(&e)) {

			switch(e.type) {

				case SDL_KEYDOWN:	
					switch(e.key.keysym.scancode) {

						case SDL_SCANCODE_ESCAPE:
							lunadata->gamestate = GAMESTATE_QUIT;
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

		keystate=SDL_GetKeyboardState(NULL);
		
		if((keystate[SDL_SCANCODE_KP_8]) || (keystate[SDL_SCANCODE_W]))
			lunadata->player.Joy |= JOY_UP;
		if((keystate[SDL_SCANCODE_KP_2]) || (keystate[SDL_SCANCODE_S]))
			lunadata->player.Joy |= JOY_DOWN;
		if((keystate[SDL_SCANCODE_KP_4]) || (keystate[SDL_SCANCODE_A]))
			lunadata->player.Joy |= JOY_LEFT;
		if((keystate[SDL_SCANCODE_KP_6]) || (keystate[SDL_SCANCODE_D]))
			lunadata->player.Joy |= JOY_RIGHT;
		if((keystate[SDL_SCANCODE_RCTRL]) || (keystate[SDL_SCANCODE_LCTRL]))
			lunadata->player.Joy |= JOY_FIRE;
	
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

	Mix_HaltMusic();
	lunadata->map.HscrollSpeed = 0;
	lunadata->DeathAnimIndex = 0;
	CheckForHighscore(lunadata);
	
	while(lunadata->gamestate == GAMESTATE_GAMEOVER) {
		GameDelay(lunadata);
		GameDrawScreen(lunadata);
		
		lunadata->DeathAnimTimer[0]--;
		if(lunadata->DeathAnimTimer[0] < 0) {
			lunadata->DeathAnimTimer[0] = lunadata->DeathAnimTimer[1];
			if(lunadata->DeathAnimIndex < 4) {
				lunadata->DeathAnimIndex++;
				lunadata->SPRITE_PTRS[0] = lunadata->DeathFrames[lunadata->DeathAnimIndex];
				if(lunadata->DeathAnimIndex == 4)
					Mix_PlayMusic(lunadata->sound.musicgameover, 0);
			}
		}
		
		BulletsClear(lunadata);
		BulletsUpdate(lunadata);
		BulletsDraw(lunadata);

		if(lunadata->DeathAnimIndex < 4) {
			ClearStars(lunadata);
			MapAdvanceMap(lunadata);
			MapScreenShift(lunadata);
			DrawStars(lunadata);
			
			EnemiesUpdate(lunadata);
			EnemiesDraw(lunadata);
		} else {
			lunadata->map.HscrollSpeed = 0;
			
			if(lunadata->HighscoreAchieved) {
				for(int i = 0; i < 26; i++)
					lunadata->SCREEN[18 * 40 + 7 + i] = lunadata->HighScoreText[i];
			}
			
			lunadata->player.PlayerIsDead = 0;
			lunadata->SPRITE_ENA = 0xFF;
			for(int i = 0; i < 8; i++)
				lunadata->SPRITE_PTRS[i] = 144 + i;
			lunadata->GameOverSinTicker++;
			if(lunadata->GameOverSinTicker > 255)
				lunadata->GameOverSinTicker = 0;
			
			for(int i = 0; i < 4; i++)
				lunadata->SPRITE_X[i] = 70 + i * 26;
			for(int i = 4; i < 8; i++)
				lunadata->SPRITE_X[i] = 94 + i * 26;
			
			for(int i = 0; i < 8; i++)
				lunadata->SPRITE_Y[i] = lunadata->GameOverSinY[(i * 8 + lunadata->GameOverSinTicker) & 0xFF];
		}
		
		while(SDL_PollEvent(&e)) {

			switch(e.type) {

				case SDL_KEYDOWN:	
					switch(e.key.keysym.scancode) {

						case SDL_SCANCODE_ESCAPE:
							lunadata->gamestate = GAMESTATE_QUIT;
							break;
							
						case SDL_SCANCODE_RCTRL:
						case SDL_SCANCODE_LCTRL:
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
	SDL_Rect rect = {16, 0, 608, 400};
	char str[500];

	SDL_Init(SDL_INIT_VIDEO);
//	lunadata->mainwin = SDL_CreateWindow("LunAPC", 0, 0, 800, 600, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN);
	lunadata->mainwin = SDL_CreateWindow("LunAPC", 0, 0, 656, 400, SDL_WINDOW_SHOWN);
	lunadata->mainrend = SDL_CreateRenderer(lunadata->mainwin, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetClipRect(lunadata->mainrend, &rect);

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
	SoundInit(lunadata);
	
	for(int i = 0; i < 256; i++) {
		lunadata->enemies.SinY[0][i] = (int)(sin((i / 256.0) * (M_PI * 2)) * 80 + 88 + 50);
		lunadata->enemies.SinY[1][i] = lunadata->enemies.SinY[0][i];
		lunadata->enemies.SinY[2][i] = lunadata->enemies.SinY[0][i];
		lunadata->enemies.SinX[2][i] = (int)((cos(((i - 1) / 128.0) * (M_PI * 2)) * 64 + 32) - (cos(( i / 128.0) * (M_PI * 2)) * 64 + 32));
		lunadata->enemies.SinY[3][i] = (int)(sin((i / 128.0) * (M_PI * 2)) * 80 + 88 + 50);
		lunadata->enemies.SinX[3][i] = (int)((cos(((i - 1) / 64.0) * (M_PI * 2)) * 64 + 64) - (cos((i / 64.0) * (M_PI * 2)) * 64 + 64));
		lunadata->enemies.SinY[4][i] = (int)(sin((i / 128.0) * (M_PI * 2) * 0.5) * 80 + 88 + 50);
		lunadata->enemies.SinX[4][i] = (int)(((cos(((i - 1) / 64.0) * (M_PI * 2)) * 64 + 64) - (cos((i / 64.0) * (M_PI * 2)) * 64 + 64)) / 2);
		lunadata->enemies.SinX[5][i] = (int)(((cos(((i - 1) / 128.0) * (M_PI * 2)) * 96 + 32) - (cos((i / 128.0) * (M_PI * 2)) * 96 + 32)));
		lunadata->enemies.SinY[6][i] = lunadata->enemies.SinY[0][i];
		lunadata->enemies.SinY[7][i] = lunadata->enemies.SinY[3][i];
		lunadata->enemies.SinX[7][i] = lunadata->enemies.SinX[3][i];
		lunadata->GameOverSinY[i] = (int)(sin((i / 128.0) * (M_PI * 2)) * 0x18 + 0x80);
	}

	snprintf(str, 500, "     === LunAPC V%d.%d === by akmafin = running on %s = thanks to shallan, stepz, furroy and monstersgoboom for the c64 original = movement: WASD or numpad 2468 = fire: right or left CTRL", SWVER, SWREV, SDL_GetPlatform());
	lunadata->MessageLength = StrToDispStr(str, lunadata->MessageText, sizeof(str));
}

void GameClean(maindata *lunadata) {
	Mix_HaltMusic();
	if(lunadata->sound.musicgame)
		Mix_FreeMusic(lunadata->sound.musicgame);
	if(lunadata->sound.musicgameover)
		Mix_FreeMusic(lunadata->sound.musicgameover);
	
	for (int i = 0; i < NUM_SOUNDS; i++)
		Mix_FreeChunk(lunadata->sound.mixchunk[i]);

	Mix_CloseAudio();

	SDL_DestroyTexture(lunadata->gamefonttex);
	SDL_DestroyTexture(lunadata->gamespritetex);
	IMG_Quit();

	SDL_DestroyRenderer(lunadata->mainrend);
	SDL_DestroyWindow(lunadata->mainwin);
	SDL_Quit();
}

void GameDrawScreen(maindata *lunadata) {
	SDL_Rect chsrc = {0, 0, FONTTILE_WIDTH, FONTTILE_HEIGHT}, chdest = {0, 0, FONTTILE_WIDTH, FONTTILE_HEIGHT};
	SDL_Rect spsrc = {0, 0, SPRITETILE_WIDTH, SPRITETILE_HEIGHT}, spdest = {0, 0, SPRITETILE_WIDTH, SPRITETILE_HEIGHT};
	int ch, sp, firstrow = 0, lastrow = 23;

	SDL_SetRenderDrawColor(lunadata->mainrend,0,0,0,SDL_ALPHA_OPAQUE);
	SDL_RenderClear(lunadata->mainrend);
	SDL_SetRenderDrawBlendMode(lunadata->mainrend, SDL_BLENDMODE_NONE);

	if(lunadata->gamestate == GAMESTATE_MENU) {
		firstrow = 2;
		lastrow = 24;
	}
	
	for(int y = 0; y < 25; y++) {
		for( int x = 0; x < 40; x++) {
			ch = lunadata->SCREEN[y * 40 + x];
			chsrc.x = (ch % 32) * FONTTILE_WIDTH;
			chsrc.y = (ch / 32) * FONTTILE_HEIGHT;
				chdest.x = x * FONTTILE_WIDTH;
			if((y >= firstrow) && (y <= lastrow))
				chdest.x = x * FONTTILE_WIDTH + lunadata->map.Hscroll * 2;
			else
				chdest.x = x * FONTTILE_WIDTH;
			chdest.y = y * FONTTILE_HEIGHT;
			SDL_RenderCopy(lunadata->mainrend, lunadata->gamefonttex, &chsrc, &chdest);
		}
	}
	
	SDL_SetRenderDrawBlendMode(lunadata->mainrend, SDL_BLENDMODE_BLEND);

	ch = 128;
	for(int i = 7; i >= 0; i--) {
		if(lunadata->SPRITE_ENA & ch) {
			sp = lunadata->SPRITE_PTRS[i] - 64;
			if(sp >= 0) {
				spsrc.x = (sp % 8) * SPRITETILE_WIDTH;
				spsrc.y = (sp / 8) * SPRITETILE_HEIGHT;
				spdest.x = (lunadata->SPRITE_X[i] - 0x18) * 2;
				spdest.y = (lunadata->SPRITE_Y[i] - 0x32) * 2;
				SDL_RenderCopy(lunadata->mainrend, lunadata->gamespritetex, &spsrc, &spdest);
			}
		}
		ch = ch >> 1;
	}
	
	SDL_RenderPresent(lunadata->mainrend);
}

void GameDelay(maindata *lunadata) {
	static std::chrono::high_resolution_clock::time_point last_time;
	std::chrono::high_resolution_clock::time_point time;
	std::chrono::duration<double, std::milli> time_span;

	do {
		time = std::chrono::high_resolution_clock::now();
		time_span = time - last_time;
		if(time_span.count() < 18.0)
			SDL_Delay(1);
	} while(time_span.count() < 20.0);

	time = std::chrono::high_resolution_clock::now();
	last_time = time;
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
	for(int i = 0; i < 40; i++)
		lunadata->SCREEN[i] = lunadata->Hiscore[i];
}

void GenerateStars(struct maindata *lunadata) {
}

void DrawStars(struct maindata *lunadata) {
}

void ClearStars(struct maindata *lunadata) {
}

void CheckForHighscore(struct maindata *lunadata) {
	lunadata->HighscoreAchieved = 0;
	
	for(int i = 0; i < 7; i++) {
		if(lunadata->SCREEN[24 * 40 + 7 + i] > lunadata->Hiscore[21 + i])
			lunadata->HighscoreAchieved = 1;
	}
	
	if(lunadata->HighscoreAchieved) {
		for(int i = 0; i < 7; i++)
			lunadata->Hiscore[21 + i] = lunadata->SCREEN[24 * 40 + 7 + i];
	}
}

void StopSounds(struct maindata *lunadata) {
}

unsigned char CharToDispChar(unsigned char inchr) {
	unsigned char outchr = 0;

	if ((inchr >= 'a') && (inchr <= 'z'))
		outchr = inchr - 'a' + 1;
	else if ((inchr >= 'A') && (inchr <= 'Z'))
		outchr = inchr - 'A' + 1;
	else if ((inchr >= '0') && (inchr <= '9'))
		outchr = inchr - '0' + 48;
	else if (inchr == ',')
		outchr = 44;
	else if (inchr == '.')
		outchr = 46;
	else if (inchr == '=')
		outchr = 31;
	else if (inchr == ':')
		outchr = 33;

	return outchr;
}

int StrToDispStr(const char *instr, char *outstr, int outbuflen) {
	int len;

	len = strlen(instr);
	if (outbuflen < len)
		len = outbuflen;

	for (int i = 0; i < len; i++) {
		outstr[i] = CharToDispChar(instr[i]);
	}

	return len;
}
