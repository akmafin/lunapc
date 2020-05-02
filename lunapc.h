#define GAMESTATE_QUIT				0
#define GAMESTATE_MENU				1
#define GAMESTATE_RUNNING			2
#define GAMESTATE_GAMEOVER			3

struct maindata{
	int gamestate;
	SDL_Window *mainwin;
	SDL_Renderer *mainrenderer;
	TTF_Font *font;
};

void mainmenu(maindata *lunadata);
