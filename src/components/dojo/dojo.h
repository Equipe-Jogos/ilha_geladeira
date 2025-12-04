#ifndef DOJO_H
#define DOJO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../personalizacao/personalizacao.h"

static inline int RenderDojo(
    SDL_Window *janela, 
    SDL_Renderer *renderizador, 
    SDL_Event * evento, 
    Uint32 *timeout,
    GameState *estadoJogo  
    )
{
    
    srand(time(NULL));

    SDL_Point inicial_mouse;
    SDL_Point atual_mouse;

    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&inicial_mouse.x, &inicial_mouse.y);

    IMG_Init(IMG_INIT_PNG);

    // Fundo de tela
    SDL_Rect background = {0,0,LARGURA, ALTURA};
    SDL_Texture* background_textura = IMG_LoadTexture(renderizador, "imgs/dojo/background.png");

    
}
#endif
