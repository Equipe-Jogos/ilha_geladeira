#ifndef LOADING_H
#define LOADING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../../consts/consts.h"
#include "../../utils/Aux_Timeout.h"

static inline int RenderLoadingScreen(
    SDL_Window *janela, 
    SDL_Renderer *renderizador, 
    SDL_Event *evento, 
    Uint32 *timeout,
    GameState *estadoJogo
) {
    
    SDL_Texture *capa = IMG_LoadTexture(renderizador, "imgs/capa.png");

    int LARGURA, ALTURA;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    
    SDL_Rect img_capa = {0, 0, LARGURA, ALTURA};
    SDL_Rect carregamento = {0, 0, 1, 20};
    bool carregando = true;

    while (carregando) {

        if (carregamento.w < LARGURA) {
            SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
            SDL_RenderClear(renderizador);
            SDL_RenderCopy(renderizador, capa, NULL, &img_capa);
            SDL_SetRenderDrawColor(renderizador, 0, 255, 0, 0);
            SDL_RenderFillRect(renderizador, &carregamento);
            carregamento.w += 1;
            SDL_RenderPresent(renderizador);
        }else {
            carregando = false;
        }
    }


    // Liberando recursos
    SDL_DestroyTexture(capa);
    return 1;
}

#endif
