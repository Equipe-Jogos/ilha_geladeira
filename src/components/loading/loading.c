#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../../consts/consts.c"
#include "../../utils/Aux_Timeout.c"


int RenderLoadingScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event *evento, Uint32 *timeout) {
    SDL_Texture *capa = IMG_LoadTexture(renderizador, "imgs/capa.png");
    SDL_Rect img_capa = {0, 0, LARGURA, ALTURA};
    SDL_Rect carregamento = {0, 0, 1, 20};
    bool carregando = true;

    while (carregando) {

        if (carregamento.w < 800) {
            SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
            SDL_RenderClear(renderizador);
            SDL_RenderCopy(renderizador, capa, NULL, &img_capa);
            SDL_SetRenderDrawColor(renderizador, 0, 255, 0, 0);
            SDL_RenderFillRect(renderizador, &carregamento);

            //if (AUX_WaitEventTimeout(evento, timeout)) {
            //    if(evento.type == SDL_QUIT){
            //        carregando = false;
            //    }
            //} 
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
