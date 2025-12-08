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
    int mouse_x, mouse_y;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&mouse_x, &mouse_y);

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
    
    SDL_Rect cenario_rect = {0, 0, LARGURA, ALTURA};
    SDL_Point_Float mouse = {mouse_x, mouse_y}, posicoes_puffles[num_puffles];
    SDL_Rect puffle_rects[num_puffles];
    float distancias[num_puffles], direcoes_rad[num_puffles];
    Velocidade velocidades[num_puffles];
    float escalar_velocidade = 1.0f;

    
    
    
    for (int i = 0; i < num_puffles; i++) {
        puffle_rects[i] = (SDL_Rect){LARGURA/2, ALTURA/2, ALTURA/18, ALTURA/18};
        posicoes_puffles[i] = (SDL_Point_Float){puffle_rects[0].x, puffle_rects[0].y};	
        distancias[i] = 0;
        direcoes_rad[i] = 0;
        velocidades[i] = (Velocidade){0.0f, 0.0f};
    }

    while (true) {

        
        for (int i = 0; i < num_puffles; i++) {
            if (distancias[i] > 0.5f && distancias[i] < 10) {
                SDL_Point nova_posicao = AtualizaPosicao(
                    &posicoes_puffles[i].x, 
                    &posicoes_puffles[i].y, 
                    mouse.x, 
                    mouse.y, 
                    &velocidades[i].x, 
                    &velocidades[i].y, 
                    &distancias[i]
                );
    
                posicoes_puffles[i].x = nova_posicao.x;
                posicoes_puffles[i].y = nova_posicao.y;
                puffle_rects[i].x = nova_posicao.x;
                puffle_rects[i].y = nova_posicao.y;
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
                for (int i = 0; i < num_puffles; i++) {
                    CalculaDistancia(
                        posicoes_puffles[i].x,
                        posicoes_puffles[i].y,
                        mouse.x,
                        mouse.y,
                        &distancias[i],
                        &direcoes_rad[i]
                    );
                    if (distancias[i] < 10) {
                        IniciaMovimentacao(
                            &posicoes_puffles[i].x,
                            &posicoes_puffles[i].y,
                            mouse.x,
                            mouse.y,
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

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif