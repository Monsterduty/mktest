//SDL2_image template
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
//declaring window an surfaces to use
SDL_Window *window;
SDL_Surface *imageSurface;
SDL_Surface *windowSurface;

void Init()
{
	//initializing SDL2
	SDL_Init( SDL_INIT_VIDEO );

	//creating window and getting the surface
	window = SDL_CreateWindow("SDL_image example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 500, SDL_WINDOW_SHOWN);
	windowSurface = SDL_GetWindowSurface(window);
}

void close()
{
	//freeing the memory of the surface and destroy the window
	SDL_FreeSurface(imageSurface);
	SDL_FreeSurface(windowSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char const *argv[])
{
	Init();

	//loading image
	std::string imagePath = std::string( argv[0] ).substr(0, std::string(argv[0]).find_last_of("/") + 1 );// this ensure to search specifically in the executable directory.
	imagePath += "image.jpg"; //this image is on the directory of the executable.
	imageSurface = IMG_Load( imagePath.c_str() );

	//creating rectangle where will be the image
	SDL_Rect dest = { 30, 80, imageSurface->w, imageSurface->h };
	
	//the time the window is displayed
	int time = 5;

	while( time > 0 )
	{
		//this draw a black rectangle over all the window every update
		SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 0));

		//this draw the image over the surface of the window in the place especified by the rect
		SDL_BlitSurface( imageSurface, NULL, windowSurface, &dest );

		//updating the window surface to see the changes
		SDL_UpdateWindowSurface(window);

		time--;
		SDL_Delay(1000);
	}

	// close the app
	close();
	return 0;
}
