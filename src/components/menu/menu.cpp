#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void RenderMenuScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 timeout){
  SDL_SetRenderDrawColor(renderizador, 200, 200, 255, 0);
  SDL_RenderClear(renderizador);
  SDL_RenderPresent(renderizador);
}
