#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

inline void RenderMenuScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout){
  SDL_SetWindowFullscreen(janela, SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_SetWindowPosition(janela, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  IMG_Init(IMG_INIT_PNG);

  int LARGURA, ALTURA;
  obterTamanhoJanela(janela, &LARGURA, &ALTURA);

  int tam_logo_x = 300, tam_logo_y = 200;
  int tam_botao_x = 300, tam_botao_y = 40;
  int pos_botao_x = LARGURA / 2 - (tam_botao_x / 2), pos_botao_y = 2*ALTURA / 5;
  int pos_logo_x = LARGURA / 2 - (tam_logo_x / 2), pos_logo_y = ALTURA / 8;

  SDL_Rect logo = {pos_logo_x, pos_logo_y, tam_logo_x, tam_logo_y};
  SDL_Rect botao_inciar = {pos_botao_x, pos_botao_y, tam_botao_x, tam_botao_y};
  SDL_Rect botao_personalizar = {pos_botao_x, pos_botao_y + 100, tam_botao_x, tam_botao_y}; 
  SDL_Rect botao_sair = {pos_botao_x, pos_botao_y + 200, tam_botao_x, tam_botao_y};
  
  //Usar quando achar textura do botão
  //SDL_Texture* textura_botao = IMG_LoadTexture(renderizador, "../src/imgs/cor_vermelho.png");
  
  
  SDL_SetRenderDrawColor(renderizador, 36, 116, 255, 0);
  SDL_RenderClear(renderizador);

  //Usar quando achar textura do botão
  //SDL_RenderCopy(renderizador, vermelho_img, NULL, &vermelhoQ);


  SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 255);
  SDL_RenderFillRect(renderizador, &logo);
  SDL_RenderFillRect(renderizador, &botao_inciar);
  SDL_RenderFillRect(renderizador, &botao_personalizar);
  SDL_RenderFillRect(renderizador, &botao_sair);
  
  SDL_RenderPresent(renderizador);

  IMG_Quit();
}

#endif
