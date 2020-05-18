#ifndef lunapc_h
#define lunapc_h

#include "sdl2_includes.h"
#include "bullets.h"
#include "enemies.h"
#include "hud.h"
#include "map.h"
#include "player.h"
#include "sound.h"

#define GAMESTATE_QUIT				0
#define GAMESTATE_MENU				1
#define GAMESTATE_RUNNING			2
#define GAMESTATE_GAMEOVER			3

#define SUBTUNE_INTRO				0
#define SUBTUNE_GAME				0
#define SUBTUNE_GAMEOVER			1
#define SUBTUNE_BLANK				2

#define FONTTILE_WIDTH				16
#define FONTTILE_HEIGHT				16
#define SPRITETILE_WIDTH			48
#define SPRITETILE_HEIGHT			42

struct maindata {
	SDL_Window *mainwin;
	SDL_Renderer *mainrend;
	SDL_Texture *gamefonttex, *gamespritetex;
	TTF_Font *font;
	int gamestate, IntroActive = 0;
	int DeathAnimTimer[2] = {0, 4};
	int DeathAnimIndex = 0, DeathFrames[5] = {68, 69, 70, 71, 67};
	int GameOverFirePressed = 0, GameOverFireCountdown = 0, GameOverMusicTimer = 0;
	int GameOverSinTicker = 0, GameOverColRamp[8] = {1, 11, 3, 12, 4, 2, 9, 8};
	int SCREEN[1000], COLORRAM[1000], RandomNum = 0x62;
	struct bullets bullets;
	struct enemies enemies;
	struct map map;
};

void MainMenu(struct maindata *lunadata);
void GameInit(struct maindata *lunadata);
void GameClean(struct maindata *lunadata);
void GameDrawScreen(maindata *lunadata);
void ClearScreen(struct maindata *lunadata);
void ClearColor(struct maindata *lunadata);
void RandomInit(struct maindata *lunadata);
void RandomGet(struct maindata *lunadata);
void DrawHighscore(struct maindata *lunadata);
void GenerateStars(struct maindata *lunadata);
void DrawStars(struct maindata *lunadata);
void ClearStars(struct maindata *lunadata);
void CheckForHighscore(struct maindata *lunadata);
void StopSounds(struct maindata *lunadata);

#endif
