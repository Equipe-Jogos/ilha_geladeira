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
#include "../../texturas/leitura_arquivos.c"
#include "../../texturas/globais.c"

static inline int RenderDojoScreen(
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
    SDL_Texture* background_textura = lista_txt.inicio[TEX_FUNDO_DOJO].txt;

    // Cartas azuis
    const int num_cartas = 5;
    SDL_Rect cartas_azuis[num_cartas];
    SDL_Texture* carta_azul_textura = lista_txt.inicio[TEX_CARTA_AZUL].txt;

    // Configurações das cartas
    int larguraCarta = 100;
    int alturaCarta = 100;
    int margem = 20;
    
    int larguraTotal = num_cartas * larguraCarta + (num_cartas - 1) * margem;
    int posInicialX = ((LARGURA - larguraTotal) / 2) - 300;  
    int posY = ALTURA - alturaCarta - 40;

    for(int i = 0; i < num_cartas; i++){
        cartas_azuis[i].x = posInicialX + i * (larguraCarta + margem);
        cartas_azuis[i].y = posY;
        cartas_azuis[i].w = larguraCarta;
        cartas_azuis[i].h = alturaCarta;
    }

    while(true){
    
        if (AUX_WaitEventTimeout(evento, timeout)){

            if(evento->type == SDL_KEYDOWN && evento->key.keysym.sym == SDLK_ESCAPE){
                *estadoJogo = STATE_MENU;
                SDL_DestroyTexture(background_textura);
                SDL_DestroyTexture(carta_azul_textura);
                IMG_Quit();
                return 1;
            }

            if(evento->type == SDL_QUIT){
                *estadoJogo = STATE_SAIR;
                SDL_DestroyTexture(background_textura);
                SDL_DestroyTexture(carta_azul_textura);
                IMG_Quit();
                return 0;
            }
        }

        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, background_textura, NULL, &background);

        // Renderiza as 5 cartas azuis
        for(int i = 0; i < num_cartas; i++){
            SDL_RenderCopy(renderizador, carta_azul_textura, NULL, &cartas_azuis[i]);
        }

        SDL_RenderPresent(renderizador);
    }

    SDL_DestroyTexture(background_textura);
    SDL_DestroyTexture(carta_azul_textura);
    IMG_Quit();
    return 1;
}
#endif
