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

    SDL_Rect hitbox1 = {17*LARGURA/40, ALTURA/2, LARGURA/30, ALTURA/4};
    SDL_Rect hitbox2 = {10*LARGURA/24, 3*ALTURA/4, 2*LARGURA/11, ALTURA/15}; 
    SDL_Rect hitbox3 = {45*LARGURA/80, ALTURA/2, LARGURA/30, ALTURA/4};

    SDL_Rect cenario_rect = {0, 0, LARGURA, ALTURA};
    SDL_Point_Float mouse = {0, 0}, posicoes_puffles[num_puffles];
    SDL_Rect puffle_rects[num_puffles];
    float distancias[num_puffles], direcoes_rad[num_puffles];
    Velocidade velocidades[num_puffles];
    float escalar_velocidade = 1.0f;

    for (int i = 0; i < num_puffles; i++) {
        puffle_rects[i] = (SDL_Rect){LARGURA/2, ALTURA/2, altura_puffle, largura_puffle};
        posicoes_puffles[i] = (SDL_Point_Float){puffle_rects[0].x, puffle_rects[0].y};	
        distancias[i] = 0;
        direcoes_rad[i] = 0;
        velocidades[i] = (Velocidade){0.0f, 0.0f};
    }
  
    while (true) {
        

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
                float ponto_inicial_x = evento->motion.x;
                float ponto_inicial_y = evento->motion.y;
                
                
                for (int i = 0; i < num_puffles; i++) {
                    //calcula ponto para inverter direcao do movimento do puffle
                    float dx = ponto_inicial_x - posicoes_puffles[i].x - largura_puffle/2;
                    float dy = ponto_inicial_y - posicoes_puffles[i].y - altura_puffle/2;
                    float ponto_invertido_x = ponto_inicial_x - (2 * dx);
                    float ponto_invertido_y = ponto_inicial_y - (2 * dy);
                    mouse.x = ponto_invertido_x;
                    mouse.y = ponto_invertido_y;
                    
                    CalculaDistancia(
                        posicoes_puffles[i].x,
                        posicoes_puffles[i].y,
                        ponto_invertido_x ,
                        ponto_invertido_y,
                        &distancias[i],
                        &direcoes_rad[i]
                    );
                    if (distancias[i] > 0.5f && distancias[i] <= 200) {
                        IniciaMovimentacao(
                            &posicoes_puffles[i].x,
                            &posicoes_puffles[i].y,
                            ponto_invertido_x,
                            ponto_invertido_y - altura_puffle/2,
                            &distancias[i],
                            &direcoes_rad[i],
                            &velocidades[i].x,
                            &velocidades[i].y,
                            escalar_velocidade
                        );
                    }
                    
                }
            }
        } else {
            for (int i = 0; i < num_puffles; i++) {
                if (distancias[i] > 0.5f && distancias[i] <= 200) {
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
                }
            }
        }

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif