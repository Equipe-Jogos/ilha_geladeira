#ifndef BEAN_CARGA
#define BEAN_CARGA

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "globais.c"
#include "enums.c"

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
    bool ativo;
    int tempo;
    SDL_Rect teste;
    int destino;
    int pos_inicial;    
    Uint32 tempo_queda;
    bool fade;
    double alpha;


} Carga;


//Vetor de cargas
typedef struct 
{
    Carga * cargas;
    int tamanho;
    int ultimo_index;
    int prob;

} Vetor_carga;

void inicializa_vetor_carga(Vetor_carga * vetor,int tam)
{
    vetor->tamanho = tam;
    vetor->cargas = malloc(tam*sizeof(Carga));
    vetor->ultimo_index = -1;
}

void inicializa_carga(SDL_Renderer * renderizador, Carga * carga)
{
    carga->rect = (SDL_Rect){ LARGURA * 0.85, ALTURA*0.52, LARGURA*0.07, LARGURA*0.07 };
    carga->alpha = 255;
    carga->pos_x = carga->rect.x;
    carga->pos_y = carga->rect.y;
    carga->tempo_anterior_carga = 0;
    carga->fade = false;
    double space_x = limite_esq + rand() % (limite_dir - limite_esq + 1);
    carga->pos_inicial = carga->pos_y;
    carga->destino = (int)ALTURA*0.72;
    double space_y = ALTURA * 0.13;

    double dx = space_x - LARGURA * 0.85;
    double dy = space_y - carga->pos_y;

    // 1) escolha uma velocidade horizontal fixa (boa para não sair da tela)
    carga->velocidade_x = -300;   // você pode ajustar depois

    // 2) tempo necessário para atingir o X
    carga->tempo = fabs(dx) / fabs(carga->velocidade_x);

    // 3) velocidade Y correta para atingir o destino Y no mesmo tempo
    carga->velocidade_y = (dy - 0.5 * gravidade * carga->tempo * carga->tempo) / carga->tempo;

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

    carga->teste = (SDL_Rect){space_x,200,10,10};
    carga->ativo = true;
}


void sorteia_carga(SDL_Renderer *renderizador, Vetor_carga * vetor)
{
    vetor->prob = rand()%80;
    if (vetor->tamanho > vetor->ultimo_index)
    {
        if (vetor->prob == 13)
        {
            inicializa_carga(renderizador,&vetor->cargas[++vetor->ultimo_index]);
        }
    }
}

void draw_cargas(SDL_Renderer * renderizador,Vetor_carga * vetor)
{
    for (int i = 0; i <= vetor->ultimo_index; i++)
    {
        Carga * carga = &vetor->cargas[i]; 
        if (carga->ativo)
        {
            if (carga->fade)
            {
                carga->alpha -= 255 * (SDL_GetTicks() - carga->tempo_queda)/2000;
                if ( carga->alpha <= 0)
                { 
                     carga->alpha = 0;
                    carga->ativo = false;

                }
                SDL_SetTextureAlphaMod(carga->txt,  carga->alpha);
            }

            if(carga->tipo == GRAOS)
            {
                double angulo;
                double porc = ((abs(carga->rect.y-carga->pos_inicial)) /(carga->destino-carga->pos_inicial));

                if (porc <= 0.25)
                {
                    angulo = 0;
                }
                else if (porc <=0.50)
                {
                    angulo = 45;
                }
                else if (porc <= 0.75)
                {
                    angulo = 90;
                }
                else 
                {
                    angulo = 180;
                }
                
                SDL_RenderCopyEx(renderizador, carga->txt, NULL, &carga->rect, angulo, NULL, SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopy(renderizador, carga->txt,NULL, &carga->rect);
            }
        }
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
        if (!vetor->cargas[i].fade)
        calcula_movimento_carga(&vetor->cargas[i]);
    }

}
#endif