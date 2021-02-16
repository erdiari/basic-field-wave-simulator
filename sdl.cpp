#include <SDL2/SDL.h>
#include "phy_sim.h"
void draw_space(ConstantSpace Space){
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("Phy_sim v0.1", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, Space.h,
                                        Space.w, SDL_WINDOW_SHOWN);
  // Create a renderer for the window created above, with the first display
  // driver present
  // and with no additional settings
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  // Set the draw color of renderer to green
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

  // Clear the renderer with the draw color
  SDL_RenderClear(renderer);

  // Update the renderer which will show the renderer cleared by the draw color
  // which is green
  SDL_RenderPresent(renderer);

  // Pause for 15 seconds (or 15000 milliseconds)
  SDL_Delay(15000);

  // Destroy the renderer created above
  SDL_DestroyRenderer(renderer);

  // Destroy the window created above
  SDL_DestroyWindow(window);

  // Close all the systems of SDL initialized at the top
  SDL_Quit();
}

void test(){
  ConstantSpace Space(10, 10, 0, 0, 2.0, 2.0, 5);
  Space.add_div_source(2, 5, 2);
  Space.add_div_source(8, 5, 2);
  draw_space(Space);
}
