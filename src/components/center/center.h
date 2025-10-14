#ifndef CENTER_H
#define CENTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../../utils/Aux_Timeout.h"

inline int RenderCenterScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout){
  int LARGURA, ALTURA;
  IMG_Init(IMG_INIT_PNG);
  SDL_Texture *centroIMG = IMG_LoadTexture(renderizador, "../src/imgs/centro.png");
  obterTamanhoJanela(janela, &LARGURA, &ALTURA);
  SDL_Rect centroRect = {pos_botao_x, pos_botao_y, tam_botao_x, tam_botao_y};

  SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
  SDL_RenderClear(renderizador);
  SDL_RenderCopy(renderizador, centroIMG, NULL, &);
  
  IMG_Quit();
  return 1;
}

