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
}
#endif
