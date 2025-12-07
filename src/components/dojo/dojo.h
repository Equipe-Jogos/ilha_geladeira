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

#define NUM_CARTAS_PLAYER 3
#define NUM_CARTAS_POOL 5

// === Função auxiliar para sortear cartas sem repetição ===
static void sortearCartas(SDL_Texture* destino[], SDL_Texture* pool[], int poolSize, int qtdCartas) {
    bool usados[NUM_CARTAS_POOL] = {false};

    for (int i = 0; i < qtdCartas; i++) {
        int idx;
        do {
            idx = rand() % poolSize;
        } while (usados[idx]);
        usados[idx] = true;
        destino[i] = pool[idx];
    }
}

static inline int RenderDojoScreen(
    SDL_Window *janela,
    SDL_Renderer *renderizador,
    SDL_Event *evento,
    Uint32 *timeout,
    GameState *estadoJogo
)
{
    srand(time(NULL));
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    IMG_Init(IMG_INIT_PNG);

    SDL_Rect background = {0,0,LARGURA, ALTURA};
    SDL_Texture* background_textura = lista_txt.inicio[TEX_FUNDO_DOJO].txt;

    // texturas cartas jogador
    SDL_Texture* cartas_pool[NUM_CARTAS_POOL] = {
        lista_txt.inicio[TEX_AGUA_3].txt,
        lista_txt.inicio[TEX_FOGO_4].txt,
        lista_txt.inicio[TEX_FOGO_7].txt,
        lista_txt.inicio[TEX_GELO_5].txt,
        lista_txt.inicio[TEX_GELO_6].txt
    };

    // Cartas sorteadas que serão exibidas
    SDL_Texture* cartas_texturas[NUM_CARTAS_PLAYER];

    // Sorteio sem repetição
    sortearCartas(cartas_texturas, cartas_pool, NUM_CARTAS_POOL, NUM_CARTAS_PLAYER);

    // Textura das cartas azuis (NPC)
    SDL_Texture* carta_azul_textura = lista_txt.inicio[TEX_CARTA_AZUL].txt;

    SDL_Rect cartas_player[NUM_CARTAS_PLAYER];
    SDL_Rect cartas_azuis[NUM_CARTAS_PLAYER];

    // === POSICIONAMENTO ===
    int card_w = 200;
    int card_h = 200;
    int pos_y = ALTURA - card_h;

    // Cartas azuis 
    for (int i = 0; i < NUM_CARTAS_PLAYER; i++) {
        cartas_azuis[i].x = i * (card_w / 2);
        cartas_azuis[i].y = pos_y;
        cartas_azuis[i].w = card_w;
        cartas_azuis[i].h = card_h;
    }

    // Cartas do jogador 
    int espacamento = card_w / 2;
    for (int i = NUM_CARTAS_PLAYER - 1; i >= 0; i--) {
        cartas_player[i].x = LARGURA - card_w - ((NUM_CARTAS_PLAYER - 1 - i) * espacamento);
        cartas_player[i].y = pos_y;
        cartas_player[i].w = card_w;
        cartas_player[i].h = card_h;
    }

    while (true) {

        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_KEYDOWN && evento->key.keysym.sym == SDLK_ESCAPE) {
                *estadoJogo = STATE_MENU;
                IMG_Quit();
                return 1;
            }
            if (evento->type == SDL_QUIT) {
                *estadoJogo = STATE_SAIR;
                IMG_Quit();
                return 0;
            }
        }

        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, background_textura, NULL, &background);

        // NPC
        for (int i = 0; i < NUM_CARTAS_PLAYER; i++)
            SDL_RenderCopy(renderizador, carta_azul_textura, NULL, &cartas_azuis[i]);

        // Jogador (sorteadas)
        for (int i = 0; i < NUM_CARTAS_PLAYER; i++)
            SDL_RenderCopy(renderizador, cartas_texturas[i], NULL, &cartas_player[i]);

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif
