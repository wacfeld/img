// -*- mode: c++ -*-

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <vector>

#define error(fmt, ...) do { endwin(); close(); fprintf(stderr, "%s: %d: %s: " fmt, __FILE__, __LINE__, __func__ __VA_OPT__(,) __VA_ARGS__); exit(1);} while(0)

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 500

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

int main(int argc, char **argv)
{
	// while(true)
	// {
	//   c = getch();
	//   move(0,0);
	//   clrtoeol();
	//   printw("%d", c);
	//   // refresh();
	// }

  if(argc == 1)
  {
    error( "no images provided\n");
    return 0;
  }
  
  // init SDL2 window
  init("tetrui", SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_FillRect( gsurf, NULL, SDL_MapRGB( gsurf->format, 0xFF, 0xFF, 0xFF ) );
  SDL_UpdateWindowSurface( gwin );

  // load images
  // https://wiki.libsdl.org/SDL2/SDL_Surface
  std::vector<SDL_Surface *> imgs;
  for(int i = 1; i < argc; i++)
  {
    printf("loading %s\n", argv[i]);
    SDL_Surface *s = loadBMP(argv[i]);
    imgs.push_back(s);
    fprintf(stderr, "loaded image %s (%dx%d)\n", argv[i], s->w, s->h);
  }
  
  // init ncurses
  initscr();
	refresh();

  SDL_Rect src;
  src.w = imgs[0]->w;
  src.h = imgs[0]->h;
  src.x = 0;
  src.y = 0;

  int c;

  SDL_BlitSurface(imgs[0], &src, gsurf, NULL);
  SDL_UpdateWindowSurface(gwin);
    
  while(true)
  {
    c = getch();
    move(0,0);
    clrtoeol();
    if(c == 'k')
      src.y -= 10;
    if(c == 'j')
      src.y += 10;
    if(c == 'l')
      src.x += 10;
    if(c == 'h')
      src.x -= 10;
    if(c == 'q')
      break;
    
    printw("%d %d", src.x, src.y);
    
    SDL_FillRect( gsurf, NULL, SDL_MapRGB( gsurf->format, 0xFF, 0xFF, 0xFF ) );
    SDL_BlitSurface(imgs[0], &src, gsurf, NULL);
    SDL_UpdateWindowSurface(gwin);

    
  }

  return 0;
  
	// end ncurses
	getch();
	endwin();

  // end SDL2 window
  close();

  return 0;
}
