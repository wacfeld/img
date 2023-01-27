// -*- mode: c++ -*-

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define error(fmt, ...) do { fprintf(stderr, "%s: %d: %s: " fmt, __FILE__, __LINE__, __func__ __VA_OPT__(,) __VA_ARGS__); getchar();/*close(); exit(1);*/ } while(0)

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

SDL_Window *gwin;
SDL_Surface *gsurf;

void close()
{
  SDL_DestroyWindow(gwin);
  gwin = NULL;
  gsurf = NULL;

  //Quit SDL subsystems
  SDL_Quit();
}

SDL_Surface *loadBMP(const char *name)
{
  SDL_Surface *bmp = SDL_LoadBMP(name);
  if(!bmp)
  {
    error( "Unable to load image %s! SDL Error: %s\n", name, SDL_GetError());
  }

  SDL_Surface *opt = SDL_ConvertSurface( bmp, gsurf->format, 0 );
  if( opt == NULL )
  {
    error( "Unable to optimize image %s! SDL Error: %s\n", name, SDL_GetError() );
  }

  SDL_FreeSurface( bmp );
  return opt;
}

void init(const char *title, int w, int h)
{

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    error( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
  }
  else
  {
    //Create window
    gwin = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN );
    if( gwin == NULL )
    {
      error( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    }

    gsurf = SDL_GetWindowSurface( gwin );
  }
}

int main(int argc, char **args)
{

  initscr();
	printw("Hello World !!!");
	refresh();
	int c = getch();
	// addch(c);
	refresh();
	getch();
	endwin();

	return 0;

  // // initialize window
  // init("tetrui", SCREEN_WIDTH, SCREEN_HEIGHT);
  // SDL_FillRect( gsurf, NULL, SDL_MapRGB( gsurf->format, 0xFF, 0xFF, 0xFF ) );
  // SDL_UpdateWindowSurface( gwin );

  return 0;
}
