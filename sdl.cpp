#include <SDL2/SDL.h>
#include "phy_sim.h"

#define GridWidth 1  // line width is 1
#define PixelPerCell 15
#define CellSize GridWidth + PixelPerCell

const SDL_Color GRID_COLOR = {.r = 200, .g = 200, .b = 200, .a = 200};
const SDL_Color ARROW_COLOR = {.r = 255, .g = 100, .b = 120, .a = 100};

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

float logistic(float val) {return 1/(1+std::exp(-val));};


void DrawVector(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
  const int cs = CellSize;
  const int dx = abs(x2-x1) * .3;
  const int dy = abs(y2 - y1) * .1;
  SDL_RenderDrawLine (renderer, x1, y1, x2, y2);
  SDL_RenderDrawLine(renderer, x2, y2, x2 - dx, y2 - dy);
  SDL_RenderDrawLine(renderer, x2, y2, x2 - dx, y2 + dy);
};

void DrawVectors(SDL_Renderer *renderer,const SDL_Color *color, ConstantSpace Space) {
  VectorArea2D arr = Space.get_changefield();
  const std::valarray<double> len = arr.GetVectorLenght();
  const double max = len.max();
  const int HalfCellSize = (PixelPerCell)/2; //round
  const int cs = CellSize;

  for (int i = 0; i < arr.w; i++) {
    for (int j = 0; j < arr.h; j++) {
      const int x1 = i * cs + HalfCellSize;
      const int y1 = j * cs + HalfCellSize;
      const int x2 = x1 + (arr.field_arr[i][j][0] * HalfCellSize / len[i*arr.w+j] - 2);
      const int y2 = y1 + (arr.field_arr[i][j][1] * HalfCellSize / len[i*arr.w+j] - 2);
      const float ColorScale = logistic(len[i * arr.w + j] / max * 12-6)+1;
      const int RedScale = 255 -ColorScale * 255;
      const int BlueScale = ColorScale * 255;
      // const int GreenScale = 100 + ColorScale/2 ;
      SDL_SetRenderDrawColor(renderer, RedScale, BlueScale, BlueScale, 255);
      DrawVector(renderer, x1, y1, x2, y2);
    };
  };
};

// int draw_space_cts(ConstantSpace Space){
//   //Initialize video
//   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//     fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
//     return EXIT_FAILURE;
//   }
//   // create a window with size 
//   SDL_Window *window = SDL_CreateWindow("Phy_sim v0.1", SDL_WINDOWPOS_UNDEFINED,
//                                         SDL_WINDOWPOS_UNDEFINED, Space.h*(CellSize),
//                                         Space.w*(CellSize), SDL_WINDOW_SHOWN);
//   if (window == NULL) {
//     fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
//     return EXIT_FAILURE;
//   }
//   SDL_Renderer *renderer =
//     SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
//   if (renderer == NULL) {
//     SDL_DestroyWindow(window);
//     fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
//     return EXIT_FAILURE;
//   };
//   SDL_Event e;
//   bool quit_state = false;
//   while(!quit_state){
//     while (SDL_PollEvent(&e)){
//       switch (e.type){
//       case SDL_QUIT:
//         quit_state = true;
//         break;
//       default: {};
//       };
//     };
//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//     SDL_RenderClear(renderer);
//     DrawGrid(renderer, &GRID_COLOR, Space);
//     DrawVectors(renderer, &ARROW_COLOR, Space);
//     SDL_RenderPresent(renderer);
//     SDL_Delay(333); // 3fps
//   }
//   SDL_DestroyRenderer(renderer);
//   SDL_DestroyWindow(window);
//   SDL_Quit();
//   return EXIT_SUCCESS;
// };

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
    SDL_Delay(333); // 3fps
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
};
void test(){
  ConstantSpace Space(30, 30, 0, 0, 2.0, 2.0, 50);
  // Space.add_div_source(20, 10, 20);
  // Space.add_div_source(10, 10, 200);
  // Space.add_curl_source(15, 15, 20);
  Space.add_curl_source(10, 15, 15);
  Space.add_curl_source(20, 15, -15);
  // Space.add_div_source(10, 15, 10);
  // Space.get_changefield().AreaPrint();
  draw_space(Space);
};

int main(){
  test();
  return EXIT_SUCCESS;
};
