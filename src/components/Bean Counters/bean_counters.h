#ifndef BEAN_COUNTERS_H
#define BEAN_COUNTERS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../personalizacao/personalizacao.h"

//Dimensões da Tela
int LARGURA, ALTURA;

// Gravidade da carga
double gravidade = 800.0;

// Marcadores de tempo
Uint32 tempo_anterior_carga = 0;

//limites de movimentao do pinguim
int limite_esq;
int limite_dir;

// Estados de movimento do pinguim
typedef enum 
{
    PARADO,
    MOVENDO
} Estados_pinguim_movimento;


typedef enum
{
    NAO_CARREGA,
    CARREGA1,
    CARREGA2,
    CARREGA3,
    CARREGA4,
    PESO_MAXIMO,
    DANO

} Estados_pinguim_carga;

// Tipos de carga
enum
{
    GRAOS,
    PEIXE,
    BIGORNA,
    VASO
} Enum_carga;


// -----------------------------
//        STRUCT CARGA
// -----------------------------
typedef struct 
{
    SDL_Rect rect;
    SDL_Texture * txt;
    int tipo;

    double pos_x;
    double pos_y;

    double velocidade_x;
    double velocidade_y;
    Uint32 tempo_anterior_carga;

} Carga;

//Vetor de cargas
typedef struct 
{
    Carga * cargas;
    int tamanho;
    int ultimo_index;
    int prob;


} Vetor_carga;

typedef struct 
{
    SDL_Texture  * txt;
    SDL_Rect rect;
    Estados_pinguim_carga estado_carga;
    Estados_pinguim_movimento estado_movimento;
    double velocidade;
    double posx;

} Pinguim;

void inicializa_pinguim(SDL_Renderer * renderizador, Pinguim * pinguim)
{
    int comprimento_andavel = limite_dir - limite_esq;
    int posicao_inicial = limite_esq + comprimento_andavel / 2 - 150;
    pinguim->txt = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin.webp");
    pinguim->rect = (SDL_Rect){ posicao_inicial, ALTURA - 200, 150, 150 };
    pinguim->estado_movimento = PARADO;
    pinguim->estado_carga = NAO_CARREGA;
    pinguim->velocidade;
    pinguim->posx = 0;

}

void atualiza_estado_pinguim()
{

}

void inicializa_vetor_carga(Vetor_carga * vetor,int tam)
{
    vetor->tamanho = tam;
    vetor->cargas = malloc(tam*sizeof(Carga));
    vetor->ultimo_index = -1;
}

void inicializa_carga(SDL_Renderer * renderizador, Carga * carga)
{
    carga->rect = (SDL_Rect){ LARGURA * 0.8, ALTURA - 400, 70, 60 };
    carga->pos_x = carga->rect.x;
    carga->pos_y = carga->rect.y;
    carga->velocidade_x = -200;
    carga->tempo_anterior_carga = 0;

    double space_x = limite_esq + rand() % (limite_dir - limite_esq + 1);
    double space_y = ALTURA - 100;

    double tempo = (space_x - carga->rect.x) / carga->velocidade_x;

    if (tempo <= 0.01) {
        printf("Movimento impossível! tempo=%f\n", tempo);
        return;
    }

    double dy = space_y - carga->pos_y;

    // 🔥 FÓRMULA CORRETA DE VELOCIDADE INICIAL PARA ALCANÇAR Y EM T TEMPO
    carga->velocidade_y = (2 * dy - gravidade * tempo * tempo) / (2 * tempo);

    carga->tipo = rand()%4;
    
    switch (carga->tipo)
    {
        case GRAOS:
            carga->txt = IMG_LoadTexture(renderizador, "imgs/bean_counters/Coffe_bag.webp");
            break;
        case PEIXE:
            carga->txt = IMG_LoadTexture(renderizador, "imgs/bean_counters/Fish_bean_counters.webp");
            break;
        case BIGORNA:
            carga->txt = IMG_LoadTexture(renderizador, "imgs/bean_counters/Anvil.webp");
            break;
        case VASO:
            carga->txt = IMG_LoadTexture(renderizador, "imgs/bean_counters/Flower_pot.webp");
            break;
        default:
            break;
    }



    //printf("%f,%d, %f, %f\n", space_x, carga->rect.x, tempo, carga->velocidade_y);
}


void sorteia_carga(SDL_Renderer *renderizador, Vetor_carga * vetor)
{
    vetor->prob = rand()%60;
    if (vetor->tamanho > vetor->ultimo_index)
    {
        if (vetor->prob == 13)
        {
            printf("%d\n",vetor->ultimo_index);
            inicializa_carga(renderizador,&vetor->cargas[++vetor->ultimo_index]);
        }
    }
}

void draw_cargas(SDL_Renderer * renderizador,Vetor_carga * vetor)
{
        for (int i = 0; i <= vetor->ultimo_index; i++)
        {
            SDL_RenderCopy(renderizador, vetor->cargas[i].txt, NULL, &vetor->cargas[i].rect);
            //SDL_RenderFillRect(renderizador, &vetor->cargas[i].rect);
        }
}

// ==================================================
//   FÍSICA DA CARGA — DT CORRETO, SEM MUDAR PINGUIM
// ==================================================
void calcula_movimento_carga(Carga *carga)
{
    Uint32 agora = SDL_GetTicks();

    if (carga->tempo_anterior_carga == 0)
        carga->tempo_anterior_carga = agora;

    double dt = (agora - carga->tempo_anterior_carga) / 1000.0;
    carga->tempo_anterior_carga = agora;

    // Física REAL
    carga->velocidade_y += gravidade * dt;
    carga->pos_x += carga->velocidade_x * dt;
    carga->pos_y += carga->velocidade_y * dt;

    carga->rect.x = (int)carga->pos_x;
    carga->rect.y = (int)carga->pos_y;
}


void calcula_movimento_cargas(Vetor_carga * vetor)
{
    for (int i=0;i<= vetor->ultimo_index; i++)
    {
        calcula_movimento_carga(&vetor->cargas[i]);
    }

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

    srand(time(NULL));

    SDL_Point inicial_mouse;
    SDL_Point atual_mouse;

    int estado_movimento = PARADO;
    double velocidade = 0.0;

    Uint32 last = SDL_GetTicks();   // <- Pinguim usa somente ESTE

    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&inicial_mouse.x, &inicial_mouse.y);

    limite_esq = LARGURA * 0.25;
    limite_dir = LARGURA * 0.75;

    int comprimento_andavel = limite_dir - limite_esq;
    int posicao_inicial = limite_esq + comprimento_andavel / 2;

    int incremento = 0;
    int destino = posicao_inicial;

    IMG_Init(IMG_INIT_PNG);

    // Pinguim
    SDL_Rect pinguim = { posicao_inicial, ALTURA - 100, 30, 30 };
    Pinguim p2;
    inicializa_pinguim(renderizador, &p2);

    // Caminhão
    SDL_Rect caminhao = { LARGURA * 0.80, ALTURA - 600, 300, 500 };

    // CARGA
    Vetor_carga cargas_jogadas;
    inicializa_vetor_carga(&cargas_jogadas,200);

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
                free(cargas_jogadas.cargas);
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

        sorteia_carga(renderizador, &cargas_jogadas);
        calcula_movimento_cargas(&cargas_jogadas);
       



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
        draw_cargas(renderizador, &cargas_jogadas);

        SDL_SetRenderDrawColor(renderizador, 0, 255, 255, 255);
        SDL_RenderFillRect(renderizador, &pinguim);

        SDL_SetRenderDrawColor(renderizador, 0, 255, 0, 255);
        SDL_RenderFillRect(renderizador, &destino_rect);

        SDL_SetRenderDrawColor(renderizador, 255, 255, 0, 255);
        SDL_RenderFillRect(renderizador, &caminhao);

        SDL_RenderDrawPoint(renderizador, esq.x, esq.y);
        SDL_RenderDrawPoint(renderizador, dir.x, dir.y);

         SDL_RenderCopy(renderizador, p2.txt, NULL, &p2.rect);

        SDL_RenderPresent(renderizador);
    }

    SDL_ShowCursor(SDL_ENABLE);
    IMG_Quit();
    return 1;
}

#endif
