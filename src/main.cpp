#include <SDL2/SDL.h>
#include <lodepng.h>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>
#include "snake.hpp"

SDL_Texture * loadPNG(const std::string& filename, SDL_Renderer * renderer)
{
	std::vector<unsigned char> image;
	unsigned w, h;
	unsigned error = lodepng::decode(image, w, h, filename);

	SDL_Texture *tex = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_STATIC,w,h);
	SDL_Rect rect;
	rect.x=0;
	rect.y=0;
	rect.w=w;
	rect.h=h;
	SDL_UpdateTexture(tex,&rect,&image[0],sizeof(unsigned char)*4*w);
	SDL_SetTextureBlendMode(tex,SDL_BLENDMODE_BLEND);
	return tex;
}

Uint32 timer_callback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}


int main (int argc, char** argv)
{
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
	SDL_TimerID timer;
	int done =0;

	SDL_Texture * assets;

	int t=0;





	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);

	SDL_Rect tilesrect,tiledrect,gorect;;
	tilesrect.w=16;tilesrect.h=16;
	tiledrect.w=16;tiledrect.h=16;
	gorect.x=gorect.y=0;
	gorect.w=320;gorect.h=192;
	bool up,down,left,right,space;

	up=down=left=right=space=false;
	int dir=6;
	int ndir=6;
	window= SDL_CreateWindow("snake",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,384,SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer,320,192);

	assets = loadPNG("snake.png",renderer);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	snake *sn= new snake(10,6,dir);

	timer = SDL_AddTimer(20,timer_callback,NULL);

	while (done==0)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{ done =1;};
			if(event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{ done = 1 ;};
				if (event.key.keysym.sym == SDLK_UP)
				{
					up=true;
				};
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					down=true;
				};
				if (event.key.keysym.sym == SDLK_LEFT)
				{
					left=true;
				};
				if (event.key.keysym.sym == SDLK_RIGHT)
				{
					right=true;
				};
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					if(!sn->is_alive())
					{
						delete sn;
						dir=6;
						ndir=6;
						sn=new snake(10,6,dir);
					}
				};
			};
			if(event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_UP)
				{
					up=false;
				};
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					down=false;
				};
				if (event.key.keysym.sym == SDLK_LEFT)
				{
					left=false;
				};
				if (event.key.keysym.sym == SDLK_RIGHT)
				{
					right=false;
				};
			};
			if(event.type == SDL_USEREVENT)
			{
				// update data

				if(up==true && dir != 2)
				{
					ndir = 8;
				};
				if(down==true && dir != 8)
				{
					ndir = 2;
				};
				if(left==true && dir != 6)
				{
					ndir = 4;
				};
				if(right==true && dir !=4)
				{
					ndir = 6;
				};
				t++;
				if( t>= 7)
				{
					if(sn->is_alive())
					{
						dir = ndir;
						sn->advance(dir);
					}
					t=0;
				}
			}
		};
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		//display
		tilesrect.x=48;
		tilesrect.y=16;
		int i,j;
		for(i=0;i<12;i++)
		{
			for(j=0;j<20;j++)
			{
				tiledrect.x=j*16;
				tiledrect.y=i*16;
				SDL_RenderCopy(renderer,assets,&tilesrect,&tiledrect);
			}
		}
		tilesrect.x=32;tilesrect.y=0;
		for(i=1;i<19;i++)
		{
			tiledrect.x=i*16;
			tiledrect.y=0;
			SDL_RenderCopy(renderer,assets,&tilesrect,&tiledrect);
			tiledrect.y=176;
			SDL_RenderCopy(renderer,assets,&tilesrect,&tiledrect);
		}
		tilesrect.x=32;tilesrect.y=16;
		for(i=1;i<11;i++)
		{
			tiledrect.y=i*16;
			tiledrect.x=0;
			SDL_RenderCopy(renderer,assets,&tilesrect,&tiledrect);
			tiledrect.x=304;
			SDL_RenderCopy(renderer,assets,&tilesrect,&tiledrect);
		}
		tilesrect.x=0;tilesrect.y=0;tiledrect.x=0;tiledrect.y=0;
		SDL_RenderCopy(renderer,assets,&tilesrect,&tiledrect);
		tilesrect.x=16;tilesrect.y=0;tiledrect.x=304;tiledrect.y=0;
		SDL_RenderCopy(renderer,assets,&tilesrect,&tiledrect);
		tilesrect.x=0;tilesrect.y=16;tiledrect.x=0;tiledrect.y=176;
		SDL_RenderCopy(renderer,assets,&tilesrect,&tiledrect);
		tilesrect.x=16;tilesrect.y=16;tiledrect.x=304;tiledrect.y=176;
		SDL_RenderCopy(renderer,assets,&tilesrect,&tiledrect);

		sn->display(renderer,assets);

		if(!sn->is_alive())
		{
			SDL_SetRenderDrawColor(renderer,255,0,0,128);
			SDL_RenderFillRect(renderer,&gorect);
		}





		SDL_RenderPresent(renderer);
	}

	delete sn;
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
