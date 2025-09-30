#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void RenderMenuScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout){
  char caminhoBotaoJogar[200];
  char caminhoBotaoSair[200];
  
  IMG_Init(IMG_INIT_PNG);
  //SDL_Texture *municao = IMG_LoadTexture(renderizador, caminhoBotaoJogar);
  //SDL_Texture *espada  = IMG_LoadTexture(renderizador, caminhoBotaoSair);
  
  SDL_SetRenderDrawColor(renderizador, 36, 116, 255, 0);
  SDL_RenderClear(renderizador);
  SDL_RenderPresent(renderizador);
  
  IMG_Quit();
}
