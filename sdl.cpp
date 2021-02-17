#include <SDL2/SDL.h>
#include "phy_sim.h"

#define GridWidth 1  // line width is 1
#define PixelPerCell 22
#define CellSize GridWidth + PixelPerCell

const SDL_Color GRID_COLOR = {.r = 200, .g = 200, .b = 200, .a = 200};
const SDL_Color ARROW_COLOR = {.r = 255, .g = 255, .b = 255, .a = 200};

void DrawGrid(SDL_Renderer *renderer, const SDL_Color *color,
              ConstantSpace Space) {
  int xp,yp = 0;
  const int xmax = CellSize * Space.w;
  const int ymax = CellSize * Space.h;
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
  // Draw horizontal lines.
  for (int i = 0; i < Space.w+1; i++) {
    xp+= CellSize;
    SDL_RenderDrawLine(renderer, xp, 0, xp, ymax);
  };
  for (int i = 0; i < Space.h+1; i++) {
    yp += CellSize;
    SDL_RenderDrawLine(renderer, 0 , yp, xmax, yp);
  };
};

void DrawVector(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
};

void DrawVectors(SDL_Renderer *renderer,const SDL_Color *color, ConstantSpace Space) {
  VectorArea2D arr = Space.get_changefield();
  const int HalfCellSize = (PixelPerCell)/2; //round
  const int cs = CellSize;
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

  for (int i = 0; i < arr.w; i++) {
    for (int j = 0; j < arr.h; j++) {
      int x1 = i * cs + HalfCellSize;
      int y1 = j * cs + HalfCellSize;
      int x2 = x1 + (arr.field_arr[i][j][0] * cs);
      int y2 = y1 + (arr.field_arr[i][j][1] * cs);
      DrawVector(renderer, x1, y1, x2, y2);
    };
  };
};

int draw_space(ConstantSpace Space){
  //Initialize video
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  // create a window with size 
  SDL_Window *window = SDL_CreateWindow("Phy_sim v0.1", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, Space.h*(CellSize),
                                        Space.w*(CellSize), SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_Renderer *renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    SDL_DestroyWindow(window);
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  };
  SDL_Event e;
  bool quit_state = false;
  while(!quit_state){
    while (SDL_PollEvent(&e)){
      switch (e.type){
      case SDL_QUIT:
        quit_state = true;
        break;
      default: {};
      };
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    DrawGrid(renderer, &GRID_COLOR, Space);
    DrawVectors(renderer, &ARROW_COLOR, Space);
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
};

void test(){
  ConstantSpace Space(50, 50, 0, 0, 2.0, 2.0, 5);
  Space.add_div_source(20, 10, 20);
  Space.add_div_source(10, 10, 20);
  Space.add_curl_source(40, 30, 20);
  // Space.get_changefield().AreaPrint();
  draw_space(Space);
};

int main(){
  test();
  return EXIT_SUCCESS;
};
