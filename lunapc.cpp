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
				break;
			case GAMESTATE_GAMEOVER:
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
	SDL_Texture *tex;
	SDL_Surface *surf;
	SDL_Color fg = {(Uint8)64, (Uint8)64, (Uint8)255, 255};
	SDL_Rect destrect = {0, 0, 100, 100};

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
	for(int i = 0; i < 40; i++) {
		MapGenerateColumn(lunadata);
		lunadata->map.NeedToShift = 1;
		MapScreenShift(lunadata);
	}

	DrawHighscore(lunadata);

	while(lunadata->gamestate == GAMESTATE_MENU) {
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
							
						default:break;
					}
					break;

				case SDL_QUIT:
					lunadata->gamestate = GAMESTATE_QUIT;
					break;

				default:break;
			}
		}
		
SDL_Delay(20);
		GameDrawScreen(lunadata);
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
	SDL_Rect src = {0, 0, FONTTILE_WIDTH, FONTTILE_HEIGHT}, dest = {0, 0, FONTTILE_WIDTH, FONTTILE_HEIGHT};
	int ch;

	SDL_SetRenderDrawColor(lunadata->mainrend,0,0,0,0);
	SDL_RenderClear(lunadata->mainrend);
	
	for(int y = 0; y < 25; y++) {
		for( int x = 0; x < 40; x++) {
			ch = lunadata->SCREEN[y * 40 + x];
			src.x = (ch % 32) * FONTTILE_WIDTH;
			src.y = (ch / 32) * FONTTILE_HEIGHT;
			dest.x = x * FONTTILE_WIDTH + lunadata->map.Hscroll * 2;
			dest.y = y * FONTTILE_HEIGHT;
			SDL_RenderCopy(lunadata->mainrend, lunadata->gamefonttex, &src, &dest);
		}
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
	lunadata->RandomNum = (lunadata->RandomNum + 1) & 0xFF;
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
