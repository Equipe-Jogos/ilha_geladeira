#ifndef PEGAPUFFLE_H
#define PEGAPUFFLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include "../../utils/Aux_monitor.h"
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_movimentacao.h"
#include "../../consts/consts.h"

typedef struct {
    float x;
    float y;
} Velocidade ;

static inline int RenderPegaPuffleScreen(
    SDL_Window *janela, 
    SDL_Renderer *renderizador, 
    SDL_Event *evento, 
    Uint32 *timeout, 
    GameState *estadoJogo
) {
    int LARGURA, ALTURA;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);

    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *fundo_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/fundo.png");
    SDL_Texture *arvores_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/arvores.png");

    int num_puffles = 3;
    int altura_puffle = ALTURA/18;
    int largura_puffle = ALTURA/18;

    const char *puffle_paths[] = {
        "imgs/puffle_roundup/puffle_amarelo.png",
        "imgs/puffle_roundup/puffle_azul.png",
        "imgs/puffle_roundup/puffle_branco.png",
        "imgs/puffle_roundup/puffle_vermelho.png",
        "imgs/puffle_roundup/puffle_verde.png",
        "imgs/puffle_roundup/puffle_laranja.png",
        "imgs/puffle_roundup/puffle_rosa.png",
        "imgs/puffle_roundup/puffle_roxo.png",
        "imgs/puffle_roundup/puffle_preto.png",
        "imgs/puffle_roundup/puffle_marrom.png"
    };
    SDL_Texture *puffle_textures[num_puffles];
    for (int i = 0; i < num_puffles; i++) {
        puffle_textures[i] = IMG_LoadTexture(renderizador, puffle_paths[i]);
    }

    SDL_Rect hitbox1 = {17*LARGURA/40, ALTURA/2, LARGURA/30, ALTURA/4};
    SDL_Rect hitbox2 = {10*LARGURA/24, 3*ALTURA/4, 2*LARGURA/11, ALTURA/15}; 
    SDL_Rect hitbox3 = {45*LARGURA/80, ALTURA/2, LARGURA/30, ALTURA/4};

    SDL_Rect cenario_rect = {0, 0, LARGURA, ALTURA};
    SDL_Point_Float posicoes_puffles[num_puffles];
    SDL_Point_Float destinos_puffles[num_puffles];
    SDL_Rect puffle_rects[num_puffles];
    float distancias[num_puffles], direcoes_rad[num_puffles];
    Velocidade velocidades[num_puffles];
    float escalar_velocidade = 1.0f;

    srand(time(NULL));
    for (int i = 0; i < num_puffles; i++) {     
        int range_x = (2 * LARGURA / 3) - (LARGURA / 3);
        int range_y = (2 * ALTURA / 3) - (ALTURA / 3);
        int x_random = (LARGURA / 3) + (rand() % (range_x + 1));
        int y_random = (ALTURA / 3) + (rand() % (range_y + 1));
        puffle_rects[i] = (SDL_Rect){x_random, y_random, altura_puffle, largura_puffle};
        posicoes_puffles[i] = (SDL_Point_Float){(float)puffle_rects[i].x, (float)puffle_rects[i].y};
        destinos_puffles[i] = (SDL_Point_Float){(float)puffle_rects[i].x, (float)puffle_rects[i].y};
        distancias[i] = 0;
        direcoes_rad[i] = 0;
        velocidades[i] = (Velocidade){0.0f, 0.0f};
    }
  
    while (true) {
        

        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
        SDL_RenderClear(renderizador);

        SDL_RenderCopy(renderizador, fundo_img, NULL, &cenario_rect);

        for (int i = 0; i < num_puffles; i++) {
            SDL_RenderCopy(renderizador, puffle_textures[i], NULL, &puffle_rects[i]);
        }
        SDL_RenderCopy(renderizador, arvores_img, NULL, &cenario_rect);


        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_KEYDOWN) {
                if (evento->key.keysym.sym == SDLK_ESCAPE) {
                    *estadoJogo = STATE_JOGANDO;

                    IMG_Quit();
                    return 1;
                }
            } else if (evento->type == SDL_MOUSEMOTION) {
                float ponto_inicial_x = evento->motion.x;
                float ponto_inicial_y = evento->motion.y;
                
                
                for (int i = 0; i < num_puffles; i++) {
                    //calcula ponto para inverter direcao do movimento do puffle
                    float dx = ponto_inicial_x - posicoes_puffles[i].x;
                    float dy = ponto_inicial_y - posicoes_puffles[i].y;
                    float ponto_invertido_x = ponto_inicial_x - (2 * dx);
                    float ponto_invertido_y = ponto_inicial_y - (2 * dy);
                    
                    destinos_puffles[i].x = ponto_invertido_x - largura_puffle/2;
                    destinos_puffles[i].y = ponto_invertido_y - altura_puffle/2;
                    
                    CalculaDistancia(
                        posicoes_puffles[i].x,
                        posicoes_puffles[i].y,
                        destinos_puffles[i].x,
                        destinos_puffles[i].y,
                        &distancias[i],
                        &direcoes_rad[i]
                    );
                    if (distancias[i] > 0.5f && distancias[i] <= 200) {
                        IniciaMovimentacao(
                            &posicoes_puffles[i].x,
                            &posicoes_puffles[i].y,
                            destinos_puffles[i].x,
                            destinos_puffles[i].y,
                            &distancias[i],
                            &direcoes_rad[i],
                            &velocidades[i].x,
                            &velocidades[i].y,
                            escalar_velocidade
                        );
                    }
                    
                }
            }
        }
        
        // Atualiza posições dos puffles continuamente
        for (int i = 0; i < num_puffles; i++) {
            if (distancias[i] > 0.5f && distancias[i] <= 200) {
                SDL_Point nova_posicao = AtualizaPosicao(
                    &posicoes_puffles[i].x, 
                    &posicoes_puffles[i].y, 
                    destinos_puffles[i].x, 
                    destinos_puffles[i].y, 
                    &velocidades[i].x, 
                    &velocidades[i].y, 
                    &distancias[i]
                );
    
                puffle_rects[i].x = nova_posicao.x;
                puffle_rects[i].y = nova_posicao.y;
            }
        }

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif