#ifndef CENTER_H
#define CENTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../personalizacao/personalizacao.h" 

static inline int RenderGameScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout, GameState *estadoJogo) {
    int LARGURA, ALTURA;
    int mouse_x, mouse_y;
    char caminho[100];
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&mouse_x, &mouse_y);

    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *centroIMG = IMG_LoadTexture(renderizador, "imgs/centro.png");

    switch (corSelecionada) {
        case 0:
            sprintf(caminho, "imgs/pinguim/amarelo/000.png");
            break;
        case 1:
            sprintf(caminho, "imgs/pinguim/rosa/000.png");
            break;
        case 2:
            sprintf(caminho, "imgs/pinguim/azul/000.png");
            break;
        case 3:
            sprintf(caminho, "imgs/pinguim/azul_claro/000.png");
            break;
        case 5:
            sprintf(caminho, "imgs/pinguim/cinza/000.png");
            break;
        case 6:
            sprintf(caminho, "imgs/pinguim/laranja/000.png");
            break;
        case 7:
            sprintf(caminho, "imgs/pinguim/marrom/000.png");
            break;
        case 8:
            sprintf(caminho, "imgs/pinguim/preto/000.png");
            break;
        case 10:
            sprintf(caminho, "imgs/pinguim/roxo/000.png");
            break;
        case 11:
            sprintf(caminho, "imgs/pinguim/verdeMusgo/000.png");
            break;
        case 12:
            sprintf(caminho, "imgs/pinguim/verdeEscuro/000.png");
            break;
        case 13:
            sprintf(caminho, "imgs/pinguim/verde/000.png");
            break;
    }

    SDL_Texture *textura_pinguim_0 = IMG_LoadTexture(renderizador, caminho);
  
    
    SDL_Rect pinguimRect = {LARGURA/2, ALTURA/2, 100, 160};
    SDL_Rect centroRect = {0, 0, LARGURA, ALTURA};

    float escalar_velocidade = 1, velocidade_x = 0, velocidade_y = 0;
    float pinguim_x = (LARGURA/2.0f), pinguim_y = (ALTURA/2.0f);
    float destino_x = pinguim_x, destino_y = pinguim_y;
    float distancia = 0,dx = 0, dy = 0;

    while (true) {
        if (distancia > 0.5f) {
            // Atualiza posição usando variáveis float
            pinguim_x += velocidade_x;
            pinguim_y += velocidade_y;
            
            // Recalcula distância após movimento
            dx = destino_x - pinguim_x;
            dy = destino_y - pinguim_y;
            float nova_distancia = sqrt(dx*dx + dy*dy);
            
            // Se chegou perto o suficiente ou passou do destino, para no destino exato
            if (nova_distancia < 0.5f || nova_distancia >= distancia) {
                pinguim_x = destino_x;
                pinguim_y = destino_y;
                velocidade_x = 0;
                velocidade_y = 0;
            }
            
            // Converte para int ao atribuir ao SDL_Rect
            pinguimRect.x = (int)roundf(pinguim_x);
            pinguimRect.y = (int)roundf(pinguim_y);
        }

        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, centroIMG, NULL, &centroRect);
        SDL_RenderCopy(renderizador, textura_pinguim_0, NULL, &pinguimRect);

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
                destino_x = (float)evento->button.x - (pinguimRect.w/2);
                destino_y = (float)evento->button.y - (2*pinguimRect.h/3);

                //calcula distancia entre pinguim e destino e velocidade do pinguim
                dx = destino_x - pinguim_x;
                dy = destino_y - pinguim_y;
                distancia =  sqrt(dx*dx + dy*dy);
                
                if (distancia > 0) {

                    velocidade_x = (dx / distancia) * escalar_velocidade;
                    velocidade_y = (dy / distancia) * escalar_velocidade;
                }
            }
        }

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif