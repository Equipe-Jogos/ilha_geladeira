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
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    IMG_Init(IMG_INIT_PNG);

    SDL_Rect background = {0,0,LARGURA, ALTURA};
    SDL_Texture* background_textura = lista_txt.inicio[TEX_FUNDO_DOJO].txt;

    const int num_cartas = 5;

    // Cartas jogador
    SDL_Texture* cartas_texturas[num_cartas];
    cartas_texturas[0] = lista_txt.inicio[TEX_AGUA_3].txt;
    cartas_texturas[1] = lista_txt.inicio[TEX_FOGO_4].txt;
    cartas_texturas[2] = lista_txt.inicio[TEX_FOGO_7].txt;
    cartas_texturas[3] = lista_txt.inicio[TEX_GELO_5].txt;
    cartas_texturas[4] = lista_txt.inicio[TEX_GELO_6].txt;

    // Cartas azuis
    SDL_Texture* carta_azul_textura = lista_txt.inicio[TEX_CARTA_AZUL].txt;

    SDL_Rect cartas_player[num_cartas];
    SDL_Rect cartas_azuis[num_cartas];

    // === POSICIONAMENTO DAS CARTAS ===
    int card_w = 200;
    int card_h = 200;
    int margem_inferior = 25;

    // Cartas azuis (seu código original permanece!)
    int pos_y = ALTURA - card_h;
    for(int i = 0; i < num_cartas; i++) {
        cartas_azuis[i].x = i * (card_w / 2);
        cartas_azuis[i].y = pos_y;
        cartas_azuis[i].w = card_w;
        cartas_azuis[i].h = card_h;
    }

    // Cartas do jogador (apenas o X foi corrigido — desenhar da direita para a esquerda)
    int espacamento = (card_w / 2);
    for(int i = num_cartas - 1; i >= 0; i--) {
        cartas_player[i].x = LARGURA - card_w - ( (num_cartas - 1 - i) * espacamento );
        cartas_player[i].y = pos_y;
        cartas_player[i].w = card_w;
        cartas_player[i].h = card_h;
    }

    // ================================

    while(true){

        if (AUX_WaitEventTimeout(evento, timeout)){
            if(evento->type == SDL_KEYDOWN && evento->key.keysym.sym == SDLK_ESCAPE){
                *estadoJogo = STATE_MENU;
                IMG_Quit();
                return 1;
            }
            if(evento->type == SDL_QUIT){
                *estadoJogo = STATE_SAIR;
                IMG_Quit();
                return 0;
            }
        }

        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, background_textura, NULL, &background);

        // Cartas azuis (NPC)
        for(int i = 0; i < num_cartas; i++)
            SDL_RenderCopy(renderizador, carta_azul_textura, NULL, &cartas_azuis[i]);

        // Cartas do jogador
        for(int i = 0; i < num_cartas; i++)
            SDL_RenderCopy(renderizador, cartas_texturas[i], NULL, &cartas_player[i]);

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif
