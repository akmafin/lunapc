/*
 * LunAPC
 * by AkmaFin
 */

#include <iostream>
#include "sdl2_includes.h"
#include "lunapc.h"

int main( int argc, char* args[] ) {
	maindata lunadata;
	SDL_Texture *tex;
	SDL_Surface *surf;
	SDL_Color fg = {(Uint8)64, (Uint8)64, (Uint8)255, 255};
	SDL_Rect destrect = {0, 0, 100, 100};
	
	SDL_Init(SDL_INIT_VIDEO);
	lunadata.mainwin = SDL_CreateWindow("LunAPC", 0, 0, 640, 400, SDL_WINDOW_SHOWN);
	lunadata.mainrenderer = SDL_CreateRenderer(lunadata.mainwin, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(lunadata.mainrenderer,0,0,0,0);
	SDL_RenderClear(lunadata.mainrenderer);
	
	TTF_Init();
	lunadata.font=TTF_OpenFont("assets/Aileron-SemiBold.ttf", 32);
	surf = TTF_RenderText_Solid(lunadata.font, "LunAPC", fg);
	tex = SDL_CreateTextureFromSurface(lunadata.mainrenderer, surf);
	SDL_RenderCopy(lunadata.mainrenderer, tex, NULL, &destrect);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
	SDL_RenderPresent(lunadata.mainrenderer);

	std::cout << "LunAPC\r\n";
	lunadata.gamestate = GAMESTATE_MENU;

	while(lunadata.gamestate != GAMESTATE_QUIT) {
        if(lunadata.gamestate == GAMESTATE_MENU)
            mainmenu(&lunadata);
        if(lunadata.gamestate == GAMESTATE_RUNNING)
            mainmenu(&lunadata);
        if(lunadata.gamestate == GAMESTATE_GAMEOVER)
            mainmenu(&lunadata);
	}
	
	TTF_CloseFont(lunadata.font);
	TTF_Quit();
	SDL_DestroyRenderer(lunadata.mainrenderer);
	SDL_DestroyWindow(lunadata.mainwin);
	SDL_Quit();
}

void mainmenu(maindata *lunadata) {
	SDL_Event e;
	
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
}

