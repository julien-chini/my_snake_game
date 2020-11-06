#include "snake.hpp"

snake::snake(int x,int y,int dir) : 
gen(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()),
xdis(1,18),ydis(1,10)
{
	alive=true;
	push_elem(x,y);
	switch(dir)
	{
		case 8:
			push_elem(x,y-1);
		break;
		case 4:
			push_elem(x-1,y);
		break;
		case 2:
			push_elem(x,y+1);
		break;
		case 6:
			push_elem(x+1,y);
		break;
		default:break;
	}
	nouv_pomme();

};
void snake::nouv_pomme()
{
	int px,py;
	while(check_coord(px=xdis(gen),py=ydis(gen)));
	pomme.x=px;
	pomme.y=py;
};
void snake::push_elem(int x,int y)
{
	elem e;
	e.x=x;e.y=y;
	body.push_front(e);
};

void snake::pop_elem()
{
	body.pop_back();
};

bool snake::check_coord(int x,int y)
{
	if(x==0 || x== 19 || y==0 || y==11)
	{
		return true;
	}
	for(auto i = body.begin();i!=body.end();i++)
	{
		if(i->x==x && i->y==y)
		{
			return true;
		}
	}
	return false;
};
void snake::advance(int dir)
{
	elem fe= body.front();
	int nx,ny;
	nx=fe.x;ny=fe.y;
	switch(dir)
	{
		case 8:
			ny--;
		break;
		case 4:
			nx--;
		break;
		case 2:
			ny++;
		break;
		case 6:
			nx++;
		break;
		default:break;
	};
	if(check_coord(nx,ny))
	{
		alive=false;
	}
	else
	{
		push_elem(nx,ny);
		if(nx==pomme.x && ny== pomme.y)
		{
			nouv_pomme();
		} 
		else
		{
			pop_elem();
		};
	}
};
bool snake::is_alive()
{
	return alive;
};
bool snake::is_first(int x,int y)
{
	elem fe= body.front();
	if(fe.x==x && fe.y==y)
	{
		return true;
	}
	else
	{
		return false;
	}
};
bool snake::is_last(int x,int y)
{
	elem be= body.back();
	if(be.x==x && be.y==y)
	{
		return true;
	}
	else
	{
		return false;
	}
};
int snake::get_dir(int x1,int y1,int x2,int y2)
{
	if(x1==x2)
	{
		if(y1==y2-1)
		{return 8;}
		else if(y1==y2+1)
		{return 2;}
		else 
		{return 0;};
	}
	else if(y1==y2)
	{
		if(x1==x2-1)
		{return 4;}
		else if(x1==x2+1)
		{return 6;}
		else 
		{return 0;};
	}
	else
	{
		return 0;
	}
};
void snake::display(SDL_Renderer * renderer,SDL_Texture * tex)
{
	SDL_Rect srcrect,dstrect;
	srcrect.w=srcrect.h=dstrect.w=dstrect.h=16;
	srcrect.x=48;srcrect.y=0;
	
	dstrect.x=pomme.x*16;
	dstrect.y=pomme.y*16;
	SDL_RenderCopy(renderer,tex,&srcrect,&dstrect);
	int x,y;
	
	for(auto i = body.begin();i!=body.end();i++)
	{
		x=i->x;y=i->y;
		dstrect.x=x*16;
		dstrect.y=y*16;
		if(is_first(x,y))
		{
			auto j=i;
			j++;
			switch(get_dir(i->x,i->y,j->x,j->y))
			{
				case 8:
					srcrect.x=0;
					srcrect.y=32;
				break;
				case 4:
					srcrect.x=0;
					srcrect.y=48;
				break;
				case 2:
					srcrect.x=16;
					srcrect.y=48;
				break;
				case 6:
					srcrect.x=16;
					srcrect.y=32;
				break;
				default:break;
			};
			
		}
		else if(is_last(x,y))
		{
			auto j=i;
			j--;
			switch(get_dir(j->x,j->y,i->x,i->y))
			{
				case 8:
					srcrect.x=32;
					srcrect.y=32;
				break;
				case 4:
					srcrect.x=32;
					srcrect.y=48;
				break;
				case 2:
					srcrect.x=48;
					srcrect.y=48;
				break;
				case 6:
					srcrect.x=48;
					srcrect.y=32;
				break;
				default:break;
			};
			
		}
		else 
		{
			auto j=i;
			auto k=i;
			j++;k--;
			int dir1= get_dir(k->x,k->y,i->x,i->y);
			int dir2= get_dir(i->x,i->y,j->x,j->y);
			if(dir1==dir2 && ( dir1==4  || dir1==6  ))
			{
				srcrect.x=32;
				srcrect.y=0;
			};
			if(dir1==dir2 && ( dir1==8  || dir1==2  ))
			{
				srcrect.x=32;
				srcrect.y=16;
			};
			if((dir1==8 && dir2 == 6)||(dir1==4 && dir2==2))
			{
				srcrect.x=16;
				srcrect.y=16;
			};
			if((dir1==6 && dir2 == 2)||(dir1==8 && dir2==4))
			{
				srcrect.x=0;
				srcrect.y=16;
			};
			if((dir1==2 && dir2 == 6)||(dir1==4 && dir2==8))
			{
				srcrect.x=16;
				srcrect.y=0;
			};
			if((dir1==6 && dir2 == 8)||(dir1==2 && dir2==4))
			{
				srcrect.x=0;
				srcrect.y=0;
			};
		};
		
		SDL_RenderCopy(renderer,tex,&srcrect,&dstrect);
		
	};
};
