#include <SDL2/SDL.h>
#include <list>
#include <random>
#include <chrono>

class snake
{
	private:
		struct elem{int x;int y;};
		std::list<elem> body;
		void push_elem(int x,int y);
		void pop_elem();
		bool check_coord(int x,int y);
		bool alive;
		bool is_first(int x,int y);
		bool is_last(int x,int y);
		int get_dir(int x1,int y1,int x2,int y2);
		elem pomme;
		void nouv_pomme();
		std::minstd_rand gen;
		std::uniform_int_distribution<> xdis,ydis;
	public:
		snake(int x,int y,int dir);
		void advance(int dir);
		void display(SDL_Renderer * renderer,SDL_Texture * tex);
		bool is_alive();
	
	
};