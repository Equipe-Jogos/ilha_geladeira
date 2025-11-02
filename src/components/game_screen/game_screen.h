#ifndef CENTER_H
#define CENTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../personalizacao/personalizacao.h" 

static inline int RenderGameScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout, GameState *estadoJogo) {
    int LARGURA, ALTURA;
    int mouse_x, mouse_y;
    char caminho[100];
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&mouse_x, &mouse_y);

    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *centroIMG = IMG_LoadTexture(renderizador, "imgs/centro.png");

    switch (corSelecionada) {
        case 0:
            sprintf(caminho, "imgs/pinguim/amarelo/000.png");
            break;
        case 1:
            sprintf(caminho, "imgs/pinguim/rosa/000.png");
            break;
        case 2:
            sprintf(caminho, "imgs/pinguim/azul/000.png");
            break;
        case 3:
            sprintf(caminho, "imgs/pinguim/azul_claro/000.png");
            break;
        case 5:
            sprintf(caminho, "imgs/pinguim/cinza/000.png");
            break;
        case 6:
            sprintf(caminho, "imgs/pinguim/laranja/000.png");
            break;
        case 7:
            sprintf(caminho, "imgs/pinguim/marrom/000.png");
            break;
        case 8:
            sprintf(caminho, "imgs/pinguim/preto/000.png");
            break;
        case 10:
            sprintf(caminho, "imgs/pinguim/roxo/000.png");
            break;
        case 11:
            sprintf(caminho, "imgs/pinguim/verdeMusgo/000.png");
            break;
        case 12:
            sprintf(caminho, "imgs/pinguim/verdeEscuro/000.png");
            break;
        case 13:
            sprintf(caminho, "imgs/pinguim/verde/000.png");
            break;
    }

    SDL_Texture *textura_pinguim_0 = IMG_LoadTexture(renderizador, caminho);
  
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