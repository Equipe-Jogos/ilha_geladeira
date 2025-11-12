#ifndef CENTER_H
#define CENTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
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


    sprintf(caminho, "imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], "000");
    printf("caminho000: %s\n", caminho);
    SDL_Texture *textura_pinguim_000 = IMG_LoadTexture(renderizador, caminho);
    sprintf(caminho, "imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], "045");
    printf("caminho045: %s\n", caminho);
    SDL_Texture *textura_pinguim_045 = IMG_LoadTexture(renderizador, caminho);
    sprintf(caminho, "imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], "090");
    printf("caminho090: %s\n", caminho);
    SDL_Texture *textura_pinguim_090 = IMG_LoadTexture(renderizador, caminho);
    sprintf(caminho, "imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], "135");
    printf("caminho135: %s\n", caminho);
    SDL_Texture *textura_pinguim_135 = IMG_LoadTexture(renderizador, caminho);
    sprintf(caminho, "imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], "180");
    printf("caminho180: %s\n", caminho);
    SDL_Texture *textura_pinguim_180 = IMG_LoadTexture(renderizador, caminho);
    sprintf(caminho, "imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], "225");
    printf("caminho225: %s\n", caminho);
    SDL_Texture *textura_pinguim_225 = IMG_LoadTexture(renderizador, caminho);
    sprintf(caminho, "imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], "270");
    printf("caminho270: %s\n", caminho);
        SDL_Texture *textura_pinguim_270 = IMG_LoadTexture(renderizador, caminho);
    sprintf(caminho, "imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], "315");
    printf("caminho315: %s\n", caminho);
    SDL_Texture *textura_pinguim_315 = IMG_LoadTexture(renderizador, caminho);
    SDL_Texture *textura_atual = textura_pinguim_000;

    printf("textura_pinguim_000: %p\n", textura_pinguim_000);
    printf("textura_pinguim_045: %p\n", textura_pinguim_045);
    printf("textura_pinguim_090: %p\n", textura_pinguim_090);
    printf("textura_pinguim_135: %p\n", textura_pinguim_135);
    printf("textura_pinguim_180: %p\n", textura_pinguim_180);
    printf("textura_pinguim_225: %p\n", textura_pinguim_225);
    printf("textura_pinguim_270: %p\n", textura_pinguim_270);
    printf("textura_pinguim_315: %p\n", textura_pinguim_315);
    printf("______________________________________\n");
  
    
    SDL_Rect pinguimRect = {LARGURA/2, ALTURA/2, 100, 160};
    SDL_Rect centroRect = {0, 0, LARGURA, ALTURA};

    float escalar_velocidade = 1, velocidade_x = 0, velocidade_y = 0;
    float pinguim_x = (LARGURA/2.0f), pinguim_y = (ALTURA/2.0f);
    float destino_x = pinguim_x, destino_y = pinguim_y;
    float distancia = 0, direcao_rad = 0, dx = 0, dy = 0;

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
                destino_x = (float)evento->button.x - (pinguimRect.w/2);
                destino_y = (float)evento->button.y - (2*pinguimRect.h/3);

                //calcula distancia entre pinguim e destino e velocidade do pinguim
                dx = destino_x - pinguim_x;
                dy = destino_y - pinguim_y;
                distancia =  sqrt(dx*dx + dy*dy);
                direcao_rad = atan2(dy, dx);

                printf("direcao_rad: %f\n", direcao_rad);
                if (direcao_rad < 5*M_PI/8 && direcao_rad >= 3*M_PI/8) {
                    printf("DIRECAO: 000\n");
                    textura_atual = textura_pinguim_000;
                } else if (direcao_rad < 3*M_PI/8 && direcao_rad >= M_PI/8) {
                    printf("DIRECAO: 045\n");
                    textura_atual = textura_pinguim_045;
                } else if (direcao_rad < M_PI/8 && direcao_rad >= -M_PI/8) {
                    printf("DIRECAO: 090\n");
                    textura_atual = textura_pinguim_090;
                } else if (direcao_rad < -M_PI/8 && direcao_rad >= -3*M_PI/8) {
                    printf("DIRECAO: 135\n");
                    textura_atual = textura_pinguim_135;
                } else if (direcao_rad < -3*M_PI/8 && direcao_rad >= -5*M_PI/8) {
                    printf("DIRECAO: 180\n");
                    textura_atual = textura_pinguim_180;
                } else if (direcao_rad < -5*M_PI/8 && direcao_rad >= -7*M_PI/8) {
                    printf("DIRECAO: 225\n");
                    textura_atual = textura_pinguim_225;
                } else if (direcao_rad < -7*M_PI/8 || direcao_rad >= 7*M_PI/8) {
                    printf("DIRECAO: 270\n");
                    textura_atual = textura_pinguim_270;
                } else if (direcao_rad < 7*M_PI/8 && direcao_rad >= 5*M_PI/8) {
                    printf("DIRECAO: 315\n");
                    textura_atual = textura_pinguim_315;
                }
                printf("textura_atual: %p\n", textura_atual);
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