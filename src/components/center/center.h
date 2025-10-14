#ifndef CENTER_H
#define CENTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../../utils/Aux_Timeout.h"

inline int RenderCenterScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout){
  IMG_Init(IMG_INIT_PNG);
  SDL_Texture* textura_iniciar = IMG_LoadTexture(renderizador, "../src/imgs/centro.png");
  
  IMG_Quit();
  return 1;
}

