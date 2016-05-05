#include <windows.h>   // include important windows stuff

#include "SDL.h"

#include <cstdio>

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

#pragma warning(disable:4244) //全部关掉

#define SCREEN_WIDTH 640            //窗口尺寸
#define SCREEN_HEIGHT 480

								 
SDL_Window* gWindow = nullptr;//渲染的窗口


SDL_Surface* gScreenSurface = nullptr;//窗口包含的表面

SDL_Renderer* gRenderer = nullptr;


SDL_Texture* gTexture = nullptr;//加载和显示到屏幕的图像


UINT pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

									
bool init();//启动sdl，创建窗口

void close();

int Game_Main();


void setPixels(int x, int y, UINT color);

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

				gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
				
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

void setPixels(int x, int y, UINT color) 
{
	int index = (int)(SCREEN_WIDTH * y + x);

	pixels[index] = color;
}


//游戏循环
int Game_Main()
{
	//清屏
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//清空像素
	memset(pixels, 0xFF0000, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

	//设置像素点
	setPixels(10, 10, 0xFF0000);
	setPixels(20, 20, 0x00FF00);

	SDL_UpdateTexture(gTexture, NULL, pixels, 640 * sizeof(Uint32));

	//渲染贴图到屏幕
	SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

	//显示
	SDL_RenderPresent(gRenderer);

	return 1;
}