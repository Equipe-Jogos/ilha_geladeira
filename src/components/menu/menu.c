#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

void RenderMenuScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout){

  IMG_Init(IMG_INIT_PNG);
  SDL_Rect vermelhoQ = {300, 200, 100, 100};
  SDL_Rect verdeQ = {500, 250, 100, 100}; 
  SDL_Rect azulQ = {50, 500, 100, 100};
  
  SDL_Texture* vermelho_img = IMG_LoadTexture(renderizador, "../src/imgs/cor_vermelho.png");
  SDL_Texture* verde_img = IMG_LoadTexture(renderizador, "../src/imgs/cor_verde_medio.png");
  SDL_Texture* azul_img = IMG_LoadTexture(renderizador, "../src/imgs/cor_azul_fraco.png");
  
  SDL_SetRenderDrawColor(renderizador, 36, 116, 255, 0);
  SDL_RenderClear(renderizador);
  SDL_RenderCopy(renderizador, vermelho_img, NULL, &vermelhoQ);
  SDL_RenderCopy(renderizador, verde_img, NULL, &verdeQ);
  SDL_RenderCopy(renderizador, azul_img, NULL, &azulQ);
  SDL_RenderPresent(renderizador);

  IMG_Quit();
}
