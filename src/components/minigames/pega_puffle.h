#ifndef PEGAPUFFLE_H
#define PEGAPUFFLE_H_H

#include "../../utils/Aux_monitor.h"

static inline int RenderPegaPuffleScreen(
    SDL_Window *janela, 
    SDL_Renderer *renderizador, 
    SDL_Event *evento, 
    Uint32 *timeout, 
    GameState *estadoJogo
) {
    int LARGURA, ALTURA;
    int mouse_x, mouse_y;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&mouse_x, &mouse_y);


    while (true) {

        SDL_SetRenderDrawColor(renderizador, 6, 131, 211, 0);
        SDL_RenderClear(renderizador);

        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_KEYDOWN) {
                if (evento->key.keysym.sym == SDLK_ESCAPE) {
                    *estadoJogo = STATE_JOGANDO;

                    IMG_Quit();
                    return 1;
                }
            }
        }

        SDL_RenderPresent(renderizador);
    }

    return 1;
}

#endif