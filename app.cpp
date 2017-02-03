#include <SDL.h>
#include <SDL_rect.h>
#include <iostream>
#include <string>
#include <cmath>
#include <unistd.h>

//Screen dimension constants
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 320;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init(){
  //Initialization flag
  bool success = true;

  //Initialize SDL --> on PI is default HDMI
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
    std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
    success = false;
  }
  else{
    //Set texture filtering to linear
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear" ) ){
      std::cerr << "Warning: Linear texture filtering not enabled!" << std::endl;
    }
    //Create window
    gWindow = SDL_CreateWindow( "Pi4Eye Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_INPUT_GRABBED );
    if( gWindow == NULL ){
      std::cerr << "Window could not be created! SDL Error: " <<  SDL_GetError() << std::endl;
      success = false;
    }
    else{
      //Create renderer for window
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if( gRenderer == NULL ){
	std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
	success = false;
      }
      else{
	//Initialize renderer color
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      }
    }
  }
  return success;
}

void close()
{
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


int main( int argc, char* args[] ){

  //*****  ARGUMENT PARSNG TEST ****/
  //  int c;
  //  while (( c = getopt (argc, args, "hx:y:")) != -1 )
  //    switch (c)
  //    {
  //    case 'h':
  //      std::cout << "Help!!!" << std::endl;
  //      break;
  //    default:
  //      break;
  //      
  //    }
  float rad = 0.0f;
  float speed = 0.0002f;
  //Start up SDL and create window
  if( !init() ){
    std::cerr << "Failed to initialize!" << std::endl;
  }
  else{
    SDL_DisplayMode screenInfo;
    SDL_GetCurrentDisplayMode(0, &screenInfo);
    //Main loop flag
    bool quit = false;
    bool calibration = false;

    //Event handler
    SDL_Event e;

    SDL_Rect fillRect = { 0, (screenInfo.h / 2) - 5, 10, 10 };
    //While application is running
    while( !quit ){
      //Handle events on queue
      while( SDL_PollEvent( &e ) != 0 ){
	//User requests quit
	if( e.type == SDL_QUIT ){
	  quit = true;
	}
	else if( e.type == SDL_KEYDOWN ){
	  //Select surfaces based on key press
	  switch( e.key.keysym.sym ){
	  case SDLK_PLUS:
	  case SDLK_KP_PLUS:
	    speed += 0.001;
	    std::cout << "speed up to " << speed << std::endl;
	    break;
							
	  case SDLK_MINUS:
	  case SDLK_KP_MINUS:
	    speed -= 0.001;
	    std::cout << "speed down to " << speed << std::endl;
	    break;
						    
	  case SDLK_q:
	    quit=true;
	    break;

	  case SDLK_c:
	    std::cout << "Pressed c toggle calibration" << std::endl;
	    calibration = !calibration;
	    break;
						    
	  default:
	    std::cout << "Physical key " << SDL_GetScancodeName(e.key.keysym.scancode) << " seen as " << SDL_GetKeyName(e.key.keysym.sym) << " has no function attached " << std::endl;
						    
	    break;
	  }
	}
      }

      //Clear screen
      SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
      SDL_RenderClear( gRenderer );

      if (calibration){
	// CALIBRATION BOX
	SDL_SetRenderDrawColor( gRenderer, 0xAA, 0xAA, 0xAA, 0xFF );
	SDL_RenderClear( gRenderer );
		
	fillRect.x = 2;
	fillRect.y = 2;
	fillRect.w = screenInfo.w-4;
	fillRect.h = screenInfo.h-4;
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect( gRenderer, &fillRect );
		
      }
      else{
	int xPos = (screenInfo.w / 2) + round(sin(rad) * ((screenInfo.w - 20)/2));
	rad += speed;
	//Define Rectangle
	fillRect.x = xPos;
	fillRect.y = (screenInfo.h / 2) - 5;
	fillRect.w = 10;
	fillRect.h = 10;

	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
	SDL_RenderFillRect( gRenderer, &fillRect ); 
      }
      //Update screen
      SDL_RenderPresent( gRenderer );
    }
  }
  //Free resources and close SDL
  close();
  return 0;
}
