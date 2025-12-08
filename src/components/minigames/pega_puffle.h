#ifndef PEGAPUFFLE_H
#define PEGAPUFFLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

    SDL_Texture *puffle_amarelo_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_amarelo.png");
    SDL_Texture *puffle_azul_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_azul.png");
    SDL_Texture *puffle_branco_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_branco.png");
    SDL_Texture *puffle_vermelho_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_vermelho.png");
    SDL_Texture *puffle_verde_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_verde.png");
    SDL_Texture *puffle_laranja_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_laranja.png");
    SDL_Texture *puffle_rosa_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_rosa.png");
    SDL_Texture *puffle_roxo_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_roxo.png");
    SDL_Texture *puffle_preto_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_preto.png");
    SDL_Texture *puffle_marrom_img = IMG_LoadTexture(renderizador, "imgs/puffle_roundup/puffle_marrom.png");

    int num_puffles = 1;
    int altura_puffle = ALTURA/18;
    int largura_puffle = ALTURA/18;
    
    SDL_Rect cenario_rect = {0, 0, LARGURA, ALTURA};
    SDL_Point_Float mouse = {0, 0}, posicoes_puffles[num_puffles];
    SDL_Rect puffle_rects[num_puffles];
    bool puffle_andando[num_puffles];
    float distancias[num_puffles], direcoes_rad[num_puffles];
    Velocidade velocidades[num_puffles];
    float escalar_velocidade = 1.0f;

    for (int i = 0; i < num_puffles; i++) {
        puffle_rects[i] = (SDL_Rect){LARGURA/2, ALTURA/2, altura_puffle, largura_puffle};
        posicoes_puffles[i] = (SDL_Point_Float){puffle_rects[0].x, puffle_rects[0].y};	
        puffle_andando[i] = false;
        distancias[i] = 0;
        direcoes_rad[i] = 0;
        velocidades[i] = (Velocidade){0.0f, 0.0f};
    }
  
    while (true) {
        for (int i = 0; i < num_puffles; i++) {
            if (distancias[i] > 0.5f && distancias[i] < 100) {
                SDL_Point nova_posicao = AtualizaPosicao(
                    &posicoes_puffles[i].x, 
                    &posicoes_puffles[i].y, 
                    mouse.x - largura_puffle/2, 
                    mouse.y - altura_puffle/2, 
                    &velocidades[i].x, 
                    &velocidades[i].y, 
                    &distancias[i]
                );
    
                puffle_rects[i].x = nova_posicao.x;
                puffle_rects[i].y = nova_posicao.y;
                puffle_andando[i] = false;
            }
        }

        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
        SDL_RenderClear(renderizador);

        SDL_RenderCopy(renderizador, fundo_img, NULL, &cenario_rect);

        for (int i = 0; i < num_puffles; i++) {
            SDL_RenderCopy(renderizador, puffle_vermelho_img, NULL, &puffle_rects[i]);
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
                //printf("RECT XY: %i, %i\n", puffle_rects[0].x, puffle_rects[0].y);
                //printf("POSICAO XY: %f, %f\n", posicoes_puffles[0].x, posicoes_puffles[0].y);
                //printf("VELOCIDADE XY: %f, %f\n", velocidades[0].x, velocidades[0].y);
                //printf("DISTANCIA: %f\n", distancias[0]);
                //printf("DIRECAO: %f\n", direcoes_rad[0]);
                //printf("MOUSE XY: %f, %f\n", mouse.x, mouse.y);
                //printf("_____________________________________\n");
                mouse.x = evento->motion.x;
                mouse.y = evento->motion.y;
                for (int i = 0; i < num_puffles; i++) {
                    CalculaDistancia(
                        posicoes_puffles[i].x,
                        posicoes_puffles[i].y,
                        evento->motion.x - largura_puffle/2,
                        evento->motion.y - altura_puffle/2,
                        &distancias[i],
                        &direcoes_rad[i]
                    );
                    if (distancias[i] < 100) {
                        IniciaMovimentacao(
                            &posicoes_puffles[i].x,
                            &posicoes_puffles[i].y,
                            evento->motion.x - largura_puffle/2,
                            evento->motion.y - altura_puffle/2,
                            &distancias[i],
                            &direcoes_rad[i],
                            &velocidades[i].x,
                            &velocidades[i].y,
                            escalar_velocidade
                        );
                        puffle_andando[i] = true;
                        velocidades[i].x = 1;
                        velocidades[i].y = 1;
                    }
                    
                }
            }
        }

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif