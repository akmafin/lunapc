#ifndef sdl2_includes_h
#define sdl2_includes_h

	#if (_MSC_VER > 0) // Windows Visual Studio
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <cstdlib>
	#else // Linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
	#endif

#endif
