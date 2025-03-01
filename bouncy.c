#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#define WIDTH 900
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define COLOR_BACKGROUND 0x0f0f0f0f
#define A_GRAVITY 0.2

struct Circle {
  double x;
  double y;
  double r;
  double x_v;
  double y_v;
};

void FillCircle(SDL_Surface *surface, struct Circle circle, Uint32 color) {
  double radiusSquared = pow(circle.r, 2);
  for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
    for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
      double distSquared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
      if (distSquared < radiusSquared) {
        SDL_Rect pixel = (SDL_Rect){x, y, 1, 1};
        SDL_FillRect(surface, &pixel, COLOR_WHITE);
      }
    }
  }
}

void step(struct Circle *circle) {
  circle->x += circle->x_v;
  circle->y += circle->y_v;

  if (circle->y + circle->r > HEIGHT) {
    circle->y = HEIGHT - circle->r;
    circle->y_v = -circle->y_v;
  }
  if (circle->y - circle->r < 0) {
    circle->y = circle->r;
    circle->y_v = -circle->y_v;
  }
  if (circle->x + circle->r > WIDTH) {
    circle->x = WIDTH - circle->r;
    circle->x_v = -circle->x_v;
  }
  if (circle->x - circle->r < 0) {
    circle->x = circle->r;
    circle->x_v = -circle->x_v;
  }

  circle->y_v += A_GRAVITY;
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Bouncy Ball", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                        SDL_WINDOW_BORDERLESS);

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  struct Circle circle = {200, 200, 100, 5, 0};
  SDL_Rect rect = (SDL_Rect){0, 0, WIDTH, HEIGHT};

  int simulation_running = 1;
  SDL_Event event;
  while (simulation_running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        simulation_running = 0;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        simulation_running = 0;
      }
    }

    SDL_FillRect(surface, &rect, COLOR_BACKGROUND);
    FillCircle(surface, circle, COLOR_WHITE);
    step(&circle);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
  }
}
