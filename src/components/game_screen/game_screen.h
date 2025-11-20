#ifndef CENTER_H
#define CENTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../../utils/Aux_movimentacao.h"
#include "../../consts/consts.h"
#include "../personalizacao/personalizacao.h" 

static inline int RenderGameScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout, GameState *estadoJogo) {
    int LARGURA, ALTURA;
    int mouse_x, mouse_y;
    char caminho[100];
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&mouse_x, &mouse_y);

    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *centroIMG = IMG_LoadTexture(renderizador, "imgs/centro.png");

    
    SDL_Texture *textura_pinguim[TOTAL_DIRECOES];
    SDL_Texture *textura_atual;

    for (int i = 0; i < TOTAL_DIRECOES; i++) {
        sprintf(caminho, "imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], nomes_direcoes[i]);
        textura_pinguim[i] = IMG_LoadTexture(renderizador, caminho);
    }
    textura_atual = textura_pinguim[SUL];
  
    
    SDL_Rect pinguimRect = {LARGURA/2, ALTURA/2, 100, 160};
    SDL_Rect centroRect = {0, 0, LARGURA, ALTURA};
    SDL_Rect sled_racing = {(LARGURA/5)-150, (ALTURA/2)-75, 300, 150}, pega_puffle = {(LARGURA/2)-150, (4*ALTURA/5)-75, 300, 150};

    float escalar_velocidade = 1, velocidade_x = 0, velocidade_y = 0;
    SDL_Point_Float pinguim_posicao = {(LARGURA/2.0f), (ALTURA/2.0f)};
    SDL_Point_Float destino = {(LARGURA/2.0f), (ALTURA/2.0f)};
    float distancia = 0, direcao_rad = 0, dx = 0, dy = 0;

    while (true) {
        if (
            SDL_PointInRect(
                &(SDL_Point){(int)pinguim_posicao.x + (pinguimRect.w/2), (int)pinguim_posicao.y + (2*pinguimRect.h/3)}, 
                &sled_racing
            )
        ) {
            *estadoJogo = STATE_BEANS_COUNTERS;
            return 1;
        }    

        if (
            SDL_PointInRect(
                &(SDL_Point){(int)pinguim_posicao.x + (pinguimRect.w/2), (int)pinguim_posicao.y + (2*pinguimRect.h/3)}, 
                &pega_puffle
            )
        ) {
            *estadoJogo = STATE_PEGA_PUFFLE;
            return 1;
        }

        if (distancia > 0.5f) {
            SDL_Point nova_posicao = AtualizaPosicao(
                &pinguim_posicao.x, 
                &pinguim_posicao.y, 
                destino.x, 
                destino.y, 
                &velocidade_x, 
                &velocidade_y, 
                &distancia
            );

            pinguimRect.x = nova_posicao.x;
            pinguimRect.y = nova_posicao.y;
        }

        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, centroIMG, NULL, &centroRect);

        SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 100);
        SDL_RenderFillRect(renderizador, &sled_racing);
        SDL_SetRenderDrawColor(renderizador, 0, 0, 255, 100);
        SDL_RenderFillRect(renderizador, &pega_puffle);
        SDL_RenderCopy(renderizador, textura_atual, NULL, &pinguimRect);

        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_KEYDOWN) {
                if (evento->key.keysym.sym == SDLK_ESCAPE) {
                    *estadoJogo = STATE_MENU;

                    IMG_Quit();
                    return 1;
                }
            } else if(evento->type == SDL_QUIT) {
                *estadoJogo = STATE_SAIR;

                IMG_Quit();
                return 0;
            } else if(evento->type == SDL_MOUSEBUTTONDOWN) {
                //define destino do pinguim
                destino.x = (float)evento->button.x - (pinguimRect.w/2);
                destino.y = (float)evento->button.y - (2*pinguimRect.h/3);

                IniciaMovimentacao(
                    &pinguim_posicao.x, 
                    &pinguim_posicao.y, 
                    destino.x, 
                    destino.y, 
                    &distancia,
                    &direcao_rad,
                    &velocidade_x, 
                    &velocidade_y,
                    escalar_velocidade
                );
                int direcao = DefineDirecaoCardinal(direcao_rad);

                textura_atual = textura_pinguim[direcao];
            }
        }

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif