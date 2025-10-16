#ifndef CENTER_H
#define CENTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"

static inline int RenderGameScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout, GameState *estadoJogo) {
    int LARGURA, ALTURA;
    int mouse_x, mouse_y;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&mouse_x, &mouse_y);

    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *centroIMG = IMG_LoadTexture(renderizador, "imgs/centro.png");
    SDL_Texture *textura_pinguim_0 = IMG_LoadTexture(renderizador, "imgs/pinguim/azul/000.png");
      
    int destino_x = floor(LARGURA/2), destino_y = floor(ALTURA/2);
    SDL_Rect pinguimRect = {floor(LARGURA/2), floor(ALTURA/2), 100, 160};
    SDL_Rect centroRect = {0, 0, LARGURA, ALTURA};

    while (true) {

        if (destino_x != (pinguimRect.x + pinguimRect.w/2)) {
            float diferenca = destino_x - (pinguimRect.x + pinguimRect.w/2);
            if (diferenca > 0) {
                pinguimRect.x +=  abs(diferenca) < 1 ? diferenca : 1;
            } else {
                pinguimRect.x -=  abs(diferenca) < 1 ? diferenca : 1;
            }
        }
        if (destino_y != pinguimRect.y - 2*pinguimRect.h/3) {
            float diferenca = destino_y - (pinguimRect.y + 2*pinguimRect.h/3);
            if (diferenca > 0) {
                pinguimRect.y +=  abs(diferenca) < 1 ? diferenca : 1;
            } else {
                pinguimRect.y -=  abs(diferenca) < 1 ? diferenca : 1;
            }
        }
        
        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, centroIMG, NULL, &centroRect);
        SDL_RenderCopy(renderizador, textura_pinguim_0, NULL, &pinguimRect);

        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_KEYDOWN) {
                if (evento->key.keysym.sym == SDLK_ESCAPE) {
                    *estadoJogo = STATE_MENU;

                    IMG_Quit();
                    return 1;
                }
            } else if(evento->type == SDL_QUIT) {
                *estadoJogo = STATE_SAIR;

                IMG_Quit();
                return 0;
            } else if(evento->type == SDL_MOUSEBUTTONDOWN) {
                destino_x = floor(evento->button.x);
                destino_y = floor(evento->button.y);
            }
        }

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif