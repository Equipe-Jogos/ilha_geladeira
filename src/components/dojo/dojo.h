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

// Armazenar índice da carta selecionada (-1 = nenhuma)
static int cartaSelecionada = -1;

// --- Função que sorteia as cartas do jogador sem repetição ---
static inline void sortearCartas(SDL_Texture** cartas_pool, SDL_Texture** cartas_destino)
{
    bool usados[NUM_CARTAS_POOL] = {false};
    for (int i = 0; i < NUM_CARTAS_PLAYER; i++) {
        int idx;
        do {
            idx = rand() % NUM_CARTAS_POOL;
        } while (usados[idx]);
        usados[idx] = true;
        cartas_destino[i] = cartas_pool[idx];
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

    // CHAMADA DA FUNÇÃO DE SORTEIO
    sortearCartas(cartas_pool, cartas_texturas);

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
    int espacamento = card_w; 
    for (int i = 0; i < NUM_CARTAS_PLAYER; i++) {
        cartas_player[i].x = LARGURA - card_w - i * espacamento;
        cartas_player[i].y = pos_y;
        cartas_player[i].w = card_w;
        cartas_player[i].h = card_h;
    }

    // Retângulo para carta selecionada grande
    SDL_Rect carta_grande = {0};
    carta_grande.w = 350;
    carta_grande.h = 350;
    carta_grande.x = LARGURA - carta_grande.w - 30;
    carta_grande.y = 40;

    while (true) {

        if (AUX_WaitEventTimeout(evento, timeout)) {

            // === Seleção de carta do jogador ===
            if (evento->type == SDL_MOUSEBUTTONDOWN) {
                int mx = evento->button.x;
                int my = evento->button.y;

                bool clicouNaCarta = false;
                for (int i = 0; i < NUM_CARTAS_PLAYER; i++) {
                    if(mx >= cartas_player[i].x && mx <= cartas_player[i].x + cartas_player[i].w &&
                       my >= cartas_player[i].y && my <= cartas_player[i].y + cartas_player[i].h)
                    {
                        cartaSelecionada = i; // registra a carta clicada
                        clicouNaCarta = true;
                        break;
                    }
                }
                if(!clicouNaCarta){
                    cartaSelecionada = -1; // clicou fora, desmarcar
                }
            }

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

        if (cartaSelecionada >= 0)
            SDL_RenderCopy(renderizador, cartas_texturas[cartaSelecionada], NULL, &carta_grande);

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif
