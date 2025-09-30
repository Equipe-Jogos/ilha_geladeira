#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

void RenderMenuScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout){
  bool rodando = true;
  IMG_Init(IMG_INIT_PNG);
  SDL_Rect quadradoJogar = {300, 200, 200, 200};
  SDL_Texture *botaoJogar = IMG_LoadTexture(renderizador, "../src/imgs/jogar.png");
  SDL_Texture *botaoJogarSelecionado = IMG_LoadTexture(renderizador, "../src/imgs/jogar_selected.png");
  
  
  while(rodando){
    int x, y;
    SDL_SetRenderDrawColor(renderizador, 36, 116, 255, 0);
    SDL_RenderClear(renderizador);
    SDL_GetMouseState(&x, &y);
    SDL_Point *ponto = {x, y};
  
    if(SDL_PointInRect(&ponto, &quadradoJogar)){
      SDL_RenderCopy(renderizador, botaoJogarSelecionado, NULL, &quadradoJogar);  
    }else{
      SDL_RenderCopy(renderizador, botaoJogar, NULL, &quadradoJogar); 
    }
    
    SDL_RenderPresent(renderizador);
  }
  IMG_Quit();
}
