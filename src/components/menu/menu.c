#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void RenderMenuScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout){
  
  IMG_Init(IMG_INIT_PNG);
  SDL_Rect quadradoJogar = {0, 0, 200, 200};
  SDL_Texture *botaoJogar = IMG_LoadTexture(renderizador, "../src/imgs/jogar.png");
  
  SDL_SetRenderDrawColor(renderizador, 36, 116, 255, 0);
  SDL_RenderClear(renderizador);
  SDL_RenderCopy(renderizador, botaoJogar, NULL, &quadradoJogar);
  SDL_RenderPresent(renderizador);
  
  IMG_Quit();
}
