# Basic Makefile
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
SDL_TTF_LDFLAGS = -lSDL2_ttf
all: Pi4eye

clean:
	rm Pi4eye

Pi4eye: app.cpp
	$(CXX) $(SDL_CFLAGS) $(SDL_LDFLAGS) $(SDL_TTF_LDFLAGS) $? -o $@
	@echo "### Application ready in $@ ###"

