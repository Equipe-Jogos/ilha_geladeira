#ifndef CENTER_H
#define CENTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../../utils/Aux_Timeout.h"

static inline int RenderGameScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout, GameState *estadoJogo) {
    int LARGURA, ALTURA;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);

    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *centroIMG = IMG_LoadTexture(renderizador, "imgs/centro.png");
  
    SDL_Rect centroRect = {0, 0, LARGURA, ALTURA};

    while (true) {
        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, centroIMG, NULL, &centroRect);

        SDL_RenderPresent(renderizador);

    }

    IMG_Quit();
    return 1;
}

#endif