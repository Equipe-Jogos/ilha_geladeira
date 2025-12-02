#ifndef BEAN_PINGUIM
#define BEAN_PINGUIM

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "globais.c"
#include "enums.c"

typedef struct 
{
    SDL_Texture  * txt;
    SDL_Rect rect;
    Estados_pinguim_carga estado_carga;
    Estados_pinguim_movimento estado_movimento;
    double velocidade;
    double velocidade_inicial;
    double posx;
    int incremento;
    int destino;
    Uint32 now;
    Uint32 last;
    int sacos;
    SDL_Texture ** texturas;
    double porcentagem_peso;
    Uint32 ultima_entrega;
    int intervalo_entrega;
    int score;
    int vidas;
    bool atingido;
    Uint32 momento_queda;
    int posicao_inicial;
   

} Pinguim;


void inicializa_pinguim(SDL_Renderer * renderizador, Pinguim * pinguim)
{
    int comprimento_andavel = limite_dir - limite_esq;
    pinguim->posicao_inicial = limite_esq + comprimento_andavel / 2;
    printf("%d\n",ALTURA);
    printf("%d\n",LARGURA);

    pinguim->score = 0;
    pinguim->vidas = 3;
    pinguim->atingido = false;
    
    pinguim->rect = (SDL_Rect){ pinguim->posicao_inicial, ALTURA*0.72, LARGURA*0.12, LARGURA*0.12};
    pinguim->estado_movimento = PARADO;
    pinguim->estado_carga = NAO_CARREGA;
    pinguim->velocidade_inicial = LARGURA/50;
    pinguim->velocidade = 0;
    pinguim->posx = pinguim->rect.x;
    pinguim->destino = pinguim->posx;
    pinguim->incremento = 0;
    pinguim->last = SDL_GetTicks();
    pinguim->sacos = 0;
    pinguim->porcentagem_peso = 1;
    pinguim->ultima_entrega = 0;
    pinguim->intervalo_entrega = 250;

    pinguim->texturas = malloc(10*sizeof(SDL_Texture *));
    pinguim->texturas[0] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin0.webp");
    pinguim->texturas[1] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin1.webp");
    pinguim->texturas[2] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin2.webp");
    pinguim->texturas[3] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin3.webp");
    pinguim->texturas[4] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin4.webp");
    pinguim->texturas[5] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin5.webp");
    pinguim->texturas[6] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin6.webp");
    pinguim->texturas[7] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin_anvil.webp");
    pinguim->texturas[8] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin_fish.webp");
    pinguim->texturas[9] = IMG_LoadTexture(renderizador, "imgs/bean_counters/penguin_pot.webp");


    pinguim->txt = pinguim->texturas[0];

}

void calcula_movimento_pinguim(Pinguim *pinguim)
{
    if (pinguim->atingido)
        return;

    const Uint8* teclas = SDL_GetKeyboardState(NULL);

    // MOVER ESQUERDA
    if (teclas[SDL_SCANCODE_LEFT])
    {
        int destino_aux = (int)pinguim->rect.x - 
                          (pinguim->velocidade_inicial * pinguim->porcentagem_peso);

        if (destino_aux > limite_esq)
        {
            pinguim->destino = destino_aux;
            pinguim->estado_movimento = MOVENDO;
            pinguim->rect.x = pinguim->destino;
        }
    }

    // MOVER DIREITA
    if (teclas[SDL_SCANCODE_RIGHT])
    {
        int destino_aux = (int)pinguim->rect.x + 
                          (pinguim->velocidade_inicial * pinguim->porcentagem_peso);

        if (destino_aux + pinguim->rect.w < limite_dir)
        {
            pinguim->destino = destino_aux;
            pinguim->estado_movimento = MOVENDO;
            pinguim->rect.x = pinguim->destino;
        }
    }
}

#endif