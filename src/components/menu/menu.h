#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../../utils/Aux_Timeout.h"

inline void RenderMenuScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout){
  SDL_SetWindowFullscreen(janela, SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_SetWindowPosition(janela, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  IMG_Init(IMG_INIT_PNG);

  int LARGURA, ALTURA;
  obterTamanhoJanela(janela, &LARGURA, &ALTURA);

  int tam_logo_x = 300, tam_logo_y = 200;
  int tam_botao_x = 400, tam_botao_y = 120;
  int pos_botao_x = LARGURA / 2 - (tam_botao_x / 2), pos_botao_y = 2*ALTURA / 5;
  int pos_logo_x = LARGURA / 2 - (tam_logo_x / 2), pos_logo_y = ALTURA / 8;

  SDL_Rect logo = {pos_logo_x, pos_logo_y, tam_logo_x, tam_logo_y};
  SDL_Rect botao_inciar = {pos_botao_x, pos_botao_y, tam_botao_x, tam_botao_y};
  SDL_Rect botao_personalizar = {pos_botao_x, pos_botao_y + 150, tam_botao_x, tam_botao_y}; 
  SDL_Rect botao_sair = {pos_botao_x, pos_botao_y + 300, tam_botao_x, tam_botao_y};
  
  //carrega texturas normais
  SDL_Texture* textura_iniciar = IMG_LoadTexture(renderizador, "../src/imgs/botoes/jogar.png");
  SDL_Texture* textura_personalizar = IMG_LoadTexture(renderizador, "../src/imgs/botoes/customizar.png");
  SDL_Texture* textura_sair = IMG_LoadTexture(renderizador, "../src/imgs/botoes/sair.png");
  
  //carrega texturas hover
  SDL_Texture* textura_iniciar_hover = IMG_LoadTexture(renderizador, "../src/imgs/botoes/jogar_hover.png");
  SDL_Texture* textura_personalizar_hover = IMG_LoadTexture(renderizador, "../src/imgs/botoes/customizar_hover.png");
  SDL_Texture* textura_sair_hover = IMG_LoadTexture(renderizador, "../src/imgs/botoes/sair_hover.png");
  
  // Texturas atuais (que serão renderizadas)
  SDL_Texture* textura_atual_iniciar = textura_iniciar;
  SDL_Texture* textura_atual_personalizar = textura_personalizar;
  SDL_Texture* textura_atual_sair = textura_sair;
  
  
  while(true){
    // Captura posição do mouse (funciona mesmo no timeout)
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    
    // Verifica hover nos botões
    bool mouse_in_iniciar = (
      mouse_x >= botao_inciar.x && mouse_x <= botao_inciar.x + botao_inciar.w &&
      mouse_y >= botao_inciar.y && mouse_y <= botao_inciar.y + botao_inciar.h
    );
    
    bool mouse_in_personalizar = (
      mouse_x >= botao_personalizar.x && mouse_x <= botao_personalizar.x + botao_personalizar.w &&
      mouse_y >= botao_personalizar.y && mouse_y <= botao_personalizar.y + botao_personalizar.h
    );
    
    bool mouse_in_sair = (
      mouse_x >= botao_sair.x && mouse_x <= botao_sair.x + botao_sair.w &&
      mouse_y >= botao_sair.y && mouse_y <= botao_sair.y + botao_sair.h
    );
    
    // Atualiza texturas baseado no hover
    textura_atual_iniciar = mouse_in_iniciar ? textura_iniciar_hover : textura_iniciar;
    textura_atual_personalizar = mouse_in_personalizar ? textura_personalizar_hover : textura_personalizar;
    textura_atual_sair = mouse_in_sair ? textura_sair_hover : textura_sair;
    
    SDL_SetRenderDrawColor(renderizador, 36, 116, 255, 0);
    SDL_RenderClear(renderizador);
  
    //Renderiza botões com texturas atualizadas
    SDL_RenderCopy(renderizador, textura_atual_iniciar, NULL, &botao_inciar);
    SDL_RenderCopy(renderizador, textura_atual_personalizar, NULL, &botao_personalizar);
    SDL_RenderCopy(renderizador, textura_atual_sair, NULL, &botao_sair);
  
  
    SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 255);
    SDL_RenderFillRect(renderizador, &logo);
    
    
    SDL_RenderPresent(renderizador);
    
    if(AUX_WaitEventTimeout(evento, timeout)){
      switch(evento->type) {
        case SDL_MOUSEBUTTONDOWN:
          if(evento->button.button == SDL_BUTTON_LEFT){
            int click_x = evento->button.x;
            int click_y = evento->button.y;
            
            // Verifica se clicou no botão iniciar
            if(mouse_in_iniciar){
              //iniciar jogo
            }
            // Verifica se clicou no botão personalizar
            else if(mouse_in_personalizar){
              //abrir tela de personalizar personagem

            }
            // Verifica se clicou no botão sair
            else if(mouse_in_sair){
              //sair do jogo
              return;
            }
          }
          break;
        case SDL_QUIT:
          return;
          break;
      }
    }
  }
  
  

  IMG_Quit();
}

#endif
