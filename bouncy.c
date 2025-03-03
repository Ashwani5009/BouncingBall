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
#define COLOR_TRAJECTORY 0xf59842
#define A_GRAVITY 0.2
#define DAMPENING 0.8
#define TRAJECTORY_LENGTH 100
#define TRAJECTORY_WIDTH 10

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
        SDL_FillRect(surface, &pixel,color);
      }
    }
  }
}

void step(struct Circle *circle) {
  circle->x += circle->x_v;
  circle->y += circle->y_v;

  if (circle->y + circle->r > HEIGHT) {
    circle->y = HEIGHT - circle->r;
    circle->y_v = -circle->y_v * DAMPENING;
  }
  if (circle->y - circle->r < 0) {
    circle->y = circle->r;
    circle->y_v = -circle->y_v * DAMPENING;
  }
  if (circle->x + circle->r > WIDTH) {
    circle->x = WIDTH - circle->r;
    circle->x_v = -circle->x_v * DAMPENING;
  }
  if (circle->x - circle->r < 0) {
    circle->x = circle->r;
    circle->x_v = -circle->x_v * DAMPENING;
  }

  circle->y_v += A_GRAVITY;
}

void FillTracjectory(SDL_Surface *surface,struct Circle trajectory[TRAJECTORY_LENGTH],int currIndex,Uint32 color){
  for(int i = 0 ; i < currIndex ; i++){
    trajectory[i].r = (double)TRAJECTORY_WIDTH * i / 100;
    FillCircle(surface,trajectory[i],color);
  }
}

void updateTrajectory(struct Circle trajectory[TRAJECTORY_LENGTH],struct Circle circle,int currIndex){
  if(currIndex >= TRAJECTORY_LENGTH){
    struct Circle shifted_trajectory[TRAJECTORY_LENGTH];
    for(int i = 0 ; i < currIndex ; i++){
      if(i < currIndex - 1 ) shifted_trajectory[i] = trajectory[i+1];
    }
    for(int i = 0 ; i < currIndex ; i++){
      trajectory[i] = shifted_trajectory[i];
    }
    trajectory[currIndex-1] = circle;
  }
  else trajectory[currIndex] = circle;
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Bouncy Ball", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                        SDL_WINDOW_BORDERLESS);

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  struct Circle circle = {200, 200, 100, 20, 10};
  SDL_Rect rect = (SDL_Rect){0, 0, WIDTH, HEIGHT};
  
  struct Circle trajectory[TRAJECTORY_LENGTH];
  int currIndex = 0;
  
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
    FillTracjectory(surface,trajectory,currIndex,COLOR_TRAJECTORY);
    FillCircle(surface, circle, COLOR_WHITE);
    step(&circle);
    updateTrajectory(trajectory,circle,currIndex);
    if(currIndex < TRAJECTORY_LENGTH) currIndex++;
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
  }
}
