#ifndef BEAN_COUNTERS_H
#define BEAN_COUNTERS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../personalizacao/personalizacao.h" 

// Gravidade da carga
double gravidade = 800.0;

// Marcadores de tempo
Uint32 tempo_anterior_carga = 0;

enum 
{
    PARADO,
    MOVENDO
} Estados_movimento;

enum
{
    GRAOS,
    PEIXE,
    BIGORNA
} Enum_carga;


// -----------------------------
//        STRUCT CARGA
// -----------------------------
typedef struct 
{
    SDL_Rect rect;
    char * txt;
    int peso;

    double pos_x;
    double pos_y;

    double velocidade_x;
    double velocidade_y;

} Carga;


// ==================================================
//   FÍSICA DA CARGA — DT CORRETO, SEM MUDAR PINGUIM
// ==================================================
void calcula_movimento_carga(Carga *carga)
{
    Uint32 agora = SDL_GetTicks();

    if (tempo_anterior_carga == 0)
        tempo_anterior_carga = agora;

    double dt = (agora - tempo_anterior_carga) / 1000.0;
    tempo_anterior_carga = agora;

    // Física REAL
    carga->velocidade_y += gravidade * dt;
    carga->pos_x += carga->velocidade_x * dt;
    carga->pos_y += carga->velocidade_y * dt;

    carga->rect.x = (int)carga->pos_x;
    carga->rect.y = (int)carga->pos_y;
}



// ==================================================
// TELA PRINCIPAL — PINGUIM MANTIDO, CARGA CORRIGIDA
// ==================================================
static inline int RenderBeanCountersScreen(
    SDL_Window *janela,
    SDL_Renderer *renderizador,
    SDL_Event *evento,
    Uint32 *timeout,
    GameState *estadoJogo)
{
    int LARGURA, ALTURA;
    SDL_Point inicial_mouse;
    SDL_Point atual_mouse;

    int estado_movimento = PARADO;
    double velocidade = 0.0;

    Uint32 last = SDL_GetTicks();   // <- Pinguim usa somente ESTE

    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&inicial_mouse.x, &inicial_mouse.y);

    int limite_esq = LARGURA * 0.25;
    int limite_dir = LARGURA * 0.75;

    int comprimento_andavel = limite_dir - limite_esq;
    int posicao_inicial = limite_esq + comprimento_andavel / 2;

    int incremento = 0;
    int destino = posicao_inicial;

    IMG_Init(IMG_INIT_PNG);

    // Pinguim
    SDL_Rect pinguim = { posicao_inicial, ALTURA - 100, 30, 30 };

    // Caminhão
    SDL_Rect caminhao = { LARGURA * 0.80, ALTURA - 600, 300, 500 };

    // CARGA
    Carga carga;
    carga.rect = (SDL_Rect){ LARGURA * 0.85, ALTURA - 400, 30, 30 };
    carga.pos_x = carga.rect.x;
    carga.pos_y = carga.rect.y;
    carga.velocidade_x = -200;
    carga.velocidade_y = -500;

    SDL_Point esq = { limite_esq, ALTURA - 100 };
    SDL_Point dir = { limite_dir, ALTURA - 100 };

    // POSIÇÃO EM DOUBLE PARA PINGUIM
    double pos_x = pinguim.x;

    SDL_ShowCursor(SDL_DISABLE);

    while (true)
    {
        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 255);
        SDL_RenderClear(renderizador);

        // =====================
        //    ENTRADAS
        // =====================
        if (AUX_WaitEventTimeout(evento, timeout))
        {
            if (evento->type == SDL_KEYDOWN && evento->key.keysym.sym == SDLK_ESCAPE)
            {
                *estadoJogo = STATE_MENU;
                SDL_ShowCursor(SDL_ENABLE);
                IMG_Quit();
                return 1;
            }
            if (evento->type == SDL_QUIT)
            {
                *estadoJogo = STATE_SAIR;
                SDL_ShowCursor(SDL_ENABLE);
                IMG_Quit();
                return 0;
            }

            if (evento->type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&atual_mouse.x, &atual_mouse.y);

                if (inicial_mouse.x != atual_mouse.x)
                {
                    incremento = atual_mouse.x - inicial_mouse.x;
                    int destino_aux;

                    if (estado_movimento == PARADO)
                        destino_aux = (int)pos_x + incremento;
                    else
                        destino_aux = (int)destino + incremento;

                    if (destino_aux + pinguim.w < limite_dir && destino_aux > limite_esq)
                    {
                        destino = destino_aux;
                        estado_movimento = MOVENDO;

                        velocidade = (destino > pos_x) ? 200.0 : -200.0;
                    }
                }

                inicial_mouse.x = atual_mouse.x;
            }
        }

        // ============================
        //  MOVIMENTO DA CARGA (FÍSICA)
        // ============================
        calcula_movimento_carga(&carga);


        // ============================
        //    MOVIMENTO DO PINGUIM
        // ============================
        Uint32 agora = SDL_GetTicks();
        double dt = (agora - last) / 1000.0;

        if (estado_movimento == MOVENDO)
        {
            pos_x += velocidade * dt;

            if ((velocidade > 0 && pos_x >= destino) ||
                (velocidade < 0 && pos_x <= destino))
            {
                pos_x = destino;
                estado_movimento = PARADO;
            }

            pinguim.x = (int)pos_x;
        }

        last = agora;


        // ============================
        //        DESENHO
        // ============================
        SDL_Rect destino_rect = { destino, ALTURA - 100, 10, 10 };

        SDL_SetRenderDrawColor(renderizador, 0, 0, 255, 255);
        SDL_RenderFillRect(renderizador, &carga.rect);

        SDL_SetRenderDrawColor(renderizador, 0, 255, 255, 255);
        SDL_RenderFillRect(renderizador, &pinguim);

        SDL_SetRenderDrawColor(renderizador, 0, 255, 0, 255);
        SDL_RenderFillRect(renderizador, &destino_rect);

        SDL_SetRenderDrawColor(renderizador, 255, 255, 0, 255);
        SDL_RenderFillRect(renderizador, &caminhao);

        SDL_RenderDrawPoint(renderizador, esq.x, esq.y);
        SDL_RenderDrawPoint(renderizador, dir.x, dir.y);

        SDL_RenderPresent(renderizador);
    }

    SDL_ShowCursor(SDL_ENABLE);
    IMG_Quit();
    return 1;
}

#endif
