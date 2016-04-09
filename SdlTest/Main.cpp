
#include <stdlib.h>


#include "SDL.h"

#include <cstdio>
#include <string>
#include <memory.h>

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

#define SCREEN_WIDTH 640            //窗口尺寸
#define SCREEN_HEIGHT 480

//游戏循环状态
#define GAME_STATE_INIT             0
#define GAME_STATE_START_LEVEL      1
#define GAME_STATE_RUN              2
#define GAME_STATE_SHUTDOWN         3
#define GAME_STATE_EXIT             4

//块定义
#define NUM_BLOCK_ROWS              6
#define NUM_BLOCK_COLUMNS           8

#define BLOCK_WIDTH                 64
#define BLOCK_HEIGHT                16
#define BLOCK_ORIGIN_X              8
#define BLOCK_ORIGIN_Y              8
#define BLOCK_X_GAP                 80
#define BLOCK_Y_GAP                 32

int game_state = GAME_STATE_INIT;//开始状态

								 //渲染的窗口
SDL_Window* gWindow = nullptr;

//窗口包含的表面
SDL_Surface* gScreenSurface = nullptr;

SDL_Renderer* gRenderer = nullptr;

//加载和显示到屏幕的图像

SDL_Texture* gTexture = nullptr;

unsigned char blocks[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS];

unsigned int pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

int min_clip_x = ((SCREEN_WIDTH / 2) - 100),      // clipping rectangle 
max_clip_x = ((SCREEN_WIDTH / 2) + 100),
min_clip_y = ((SCREEN_HEIGHT / 2) - 100),
max_clip_y = ((SCREEN_HEIGHT / 2) + 100);

//启动sdl，创建窗口
bool init();

bool loadMedia();

void close();

void Init_Blocks();

int Game_Main();

void Draw_Blocks();

void setPixels(int x, int y, unsigned int color);

void drawLine(int x1, int y1, int x2, int y2, unsigned int color);

int Clip_Line(int &x1, int &y1, int &x2, int &y2);

int main(int argc, char *argv[])
{
	printf("Start Game!\n");

	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		bool quit = false;

		SDL_Event e;

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}


			Game_Main();

		}
	}

	close();

	return 0;
}

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("sdl test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == nullptr)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				gScreenSurface = SDL_GetWindowSurface(gWindow);

				gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);

			}
		}
	}

	return success;
}


void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	SDL_Quit();
}

void setPixels(int x, int y, uint32_t color) {
	int index = (int)(SCREEN_WIDTH * y + x);

	pixels[index] = color;
}


//游戏循环
int Game_Main()
{

	if (game_state == GAME_STATE_INIT)
	{
		//  Init_Blocks();


		game_state = GAME_STATE_START_LEVEL;
	}
	else if (game_state == GAME_STATE_START_LEVEL)
	{

		game_state = GAME_STATE_RUN;
	}
	else if (game_state == GAME_STATE_RUN)
	{
		//清屏
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//   Draw_Blocks();
		memset(pixels, 255, 640 * 480 * sizeof(Uint32));

		// draw 1000 random lines
		for (int index = 0; index < 1000; index++)
		{
			int x1 = rand() % SCREEN_WIDTH;
			int y1 = rand() % SCREEN_HEIGHT;

			int x2 = rand() % SCREEN_WIDTH;
			int y2 = rand() % SCREEN_HEIGHT;

			if(Clip_Line(x1,y1,x2,y2))
				drawLine(x1, y1,x2, y2,rand() % 0xFFFFFFFF);

		} // end for index

		SDL_UpdateTexture(gTexture, NULL, pixels, 640 * sizeof(Uint32));

		//渲染贴图到屏幕
		SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);


		SDL_RenderPresent(gRenderer);

	}


	return 1;
}

void drawLine(int x1, int y1, int x2, int y2, unsigned int color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
	int uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
	int x = x1, y = y1, eps;//eps为累加误差

	eps = 0; 
	dx = abs(dx);
	dy = abs(dy);
	if (dx > dy)
	{
		for (x = x1; x != x2 + ux; x += ux)
		{
			setPixels(x, y, color);
			eps += dy;
			if ((eps << 1) >= dx)
			{
				y += uy;
				eps -= dx;
			}
		}
	}
	else
	{
		for (y = y1;y != y2 + uy; y += uy)
		{
			setPixels(x, y, color);
			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux;
				eps -= dy;
			}
		}
	}
}

int Clip_Line(int &x1, int &y1, int &x2, int &y2)
{
	// this function clips the sent line using the globally defined clipping
	// region

	// internal clipping codes
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008
#define CLIP_CODE_S  0x0004
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001

#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009 
#define CLIP_CODE_SW 0x0005

	int xc1 = x1,
		yc1 = y1,
		xc2 = x2,
		yc2 = y2;

	int p1_code = 0,
		p2_code = 0;

	// determine codes for p1 and p2
	if (y1 < min_clip_y)
		p1_code |= CLIP_CODE_N;
	else
		if (y1 > max_clip_y)
			p1_code |= CLIP_CODE_S;

	if (x1 < min_clip_x)
		p1_code |= CLIP_CODE_W;
	else
		if (x1 > max_clip_x)
			p1_code |= CLIP_CODE_E;

	if (y2 < min_clip_y)
		p2_code |= CLIP_CODE_N;
	else
		if (y2 > max_clip_y)
			p2_code |= CLIP_CODE_S;

	if (x2 < min_clip_x)
		p2_code |= CLIP_CODE_W;
	else
		if (x2 > max_clip_x)
			p2_code |= CLIP_CODE_E;

	// try and trivially reject
	if ((p1_code & p2_code))
		return(0);

	// test for totally visible, if so leave points untouched
	if (p1_code == 0 && p2_code == 0)
		return(1);

	// determine end clip point for p1
	switch (p1_code)
	{
	case CLIP_CODE_C: break;

	case CLIP_CODE_N:
	{
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1)*(x2 - x1) / (y2 - y1);
	} break;
	case CLIP_CODE_S:
	{
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1)*(x2 - x1) / (y2 - y1);
	} break;

	case CLIP_CODE_W:
	{
		xc1 = min_clip_x;
		yc1 = y1 + 0.5 + (min_clip_x - x1)*(y2 - y1) / (x2 - x1);
	} break;

	case CLIP_CODE_E:
	{
		xc1 = max_clip_x;
		yc1 = y1 + 0.5 + (max_clip_x - x1)*(y2 - y1) / (x2 - x1);
	} break;

	// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
	{
		// north hline intersection
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1)*(x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x)
		{
			// east vline intersection
			xc1 = max_clip_x;
			yc1 = y1 + 0.5 + (max_clip_x - x1)*(y2 - y1) / (x2 - x1);
		} // end if

	} break;

	case CLIP_CODE_SE:
	{
		// south hline intersection
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1)*(x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x)
		{
			// east vline intersection
			xc1 = max_clip_x;
			yc1 = y1 + 0.5 + (max_clip_x - x1)*(y2 - y1) / (x2 - x1);
		} // end if

	} break;

	case CLIP_CODE_NW:
	{
		// north hline intersection
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1)*(x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x)
		{
			xc1 = min_clip_x;
			yc1 = y1 + 0.5 + (min_clip_x - x1)*(y2 - y1) / (x2 - x1);
		} // end if

	} break;

	case CLIP_CODE_SW:
	{
		// south hline intersection
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1)*(x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x)
		{
			xc1 = min_clip_x;
			yc1 = y1 + 0.5 + (min_clip_x - x1)*(y2 - y1) / (x2 - x1);
		} // end if

	} break;

	default:break;

	} // end switch

	  // determine clip point for p2
	switch (p2_code)
	{
	case CLIP_CODE_C: break;

	case CLIP_CODE_N:
	{
		yc2 = min_clip_y;
		xc2 = x2 + (min_clip_y - y2)*(x1 - x2) / (y1 - y2);
	} break;

	case CLIP_CODE_S:
	{
		yc2 = max_clip_y;
		xc2 = x2 + (max_clip_y - y2)*(x1 - x2) / (y1 - y2);
	} break;

	case CLIP_CODE_W:
	{
		xc2 = min_clip_x;
		yc2 = y2 + (min_clip_x - x2)*(y1 - y2) / (x1 - x2);
	} break;

	case CLIP_CODE_E:
	{
		xc2 = max_clip_x;
		yc2 = y2 + (max_clip_x - x2)*(y1 - y2) / (x1 - x2);
	} break;

	// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
	{
		// north hline intersection
		yc2 = min_clip_y;
		xc2 = x2 + 0.5 + (min_clip_y - y2)*(x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x)
		{
			// east vline intersection
			xc2 = max_clip_x;
			yc2 = y2 + 0.5 + (max_clip_x - x2)*(y1 - y2) / (x1 - x2);
		} // end if

	} break;

	case CLIP_CODE_SE:
	{
		// south hline intersection
		yc2 = max_clip_y;
		xc2 = x2 + 0.5 + (max_clip_y - y2)*(x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x)
		{
			// east vline intersection
			xc2 = max_clip_x;
			yc2 = y2 + 0.5 + (max_clip_x - x2)*(y1 - y2) / (x1 - x2);
		} // end if

	} break;

	case CLIP_CODE_NW:
	{
		// north hline intersection
		yc2 = min_clip_y;
		xc2 = x2 + 0.5 + (min_clip_y - y2)*(x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x)
		{
			xc2 = min_clip_x;
			yc2 = y2 + 0.5 + (min_clip_x - x2)*(y1 - y2) / (x1 - x2);
		} // end if

	} break;

	case CLIP_CODE_SW:
	{
		// south hline intersection
		yc2 = max_clip_y;
		xc2 = x2 + 0.5 + (max_clip_y - y2)*(x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x)
		{
			xc2 = min_clip_x;
			yc2 = y2 + 0.5 + (min_clip_x - x2)*(y1 - y2) / (x1 - x2);
		} // end if

	} break;

	default:break;

	} // end switch

	  // do bounds check
	if ((xc1 < min_clip_x) || (xc1 > max_clip_x) ||
		(yc1 < min_clip_y) || (yc1 > max_clip_y) ||
		(xc2 < min_clip_x) || (xc2 > max_clip_x) ||
		(yc2 < min_clip_y) || (yc2 > max_clip_y))
	{
		return(0);
	} // end if

	  // store vars back
	x1 = xc1;
	y1 = yc1;
	x2 = xc2;
	y2 = yc2;

	return(1);

} // end Clip_Line