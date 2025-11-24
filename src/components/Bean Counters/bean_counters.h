#ifndef BEAN_COUNTERS_H
#define BEAN_COUNTERS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../personalizacao/personalizacao.h"

//Dimensões da Tela
int LARGURA, ALTURA;

// Gravidade da carga
double gravidade;
double k = 1.045;

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
    CARREGA5,
    CARREGA6

} Estados_pinguim_carga;

// Tipos de carga
typedef enum
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
    pinguim->velocidade_inicial = LARGURA/200;
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
    vetor->prob = rand()%100;
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

typedef struct {
    int limite;
    Carga * data;
    int topo;  // índice do elemento do topo
    int pos_x;
    int pos_y;
} Pilha;

void inicializa_pilha(Pilha *p) {
    p->topo = -1;
    p->limite = 10;
    p->data = malloc(10*sizeof(Carga));
}

// Verifica se a pilha está cheia
int pilha_cheia(Pilha *p) {
    return p->topo == p->limite - 1;
}

int push(SDL_Renderer * renderizador, Pilha *p) {
    if (pilha_cheia(p)) {
        return 0; // falhou
    }
    Carga carga;
    carga.rect = (SDL_Rect){p->pos_x, p->pos_y, LARGURA*0.07, LARGURA*0.07 };
    carga.tipo = GRAOS;
    carga.txt = IMG_LoadTexture(renderizador, "imgs/bean_counters/Coffe_bag.webp");
    p->pos_y -= LARGURA*0.02;
    p->data[++p->topo] = carga;
    return 1;
}

void draw_pilha(SDL_Renderer * renderizador,Pilha * p)
{
    for (int i =0; i<=p->topo; i++)
    {
        SDL_RenderCopy(renderizador, p->data[i].txt, NULL, &p->data[i].rect);
    }
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
    

    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&inicial_mouse.x, &inicial_mouse.y);

    gravidade = ALTURA * k;

    limite_esq = LARGURA * 0.25;
    limite_dir = LARGURA * 0.75;

    IMG_Init(IMG_INIT_PNG);


    SDL_Rect chao = {0,ALTURA*0.90, LARGURA,ALTURA*0.10};
    SDL_Rect background = {0,0,LARGURA, ALTURA};
    SDL_Texture * txt_background = IMG_LoadTexture(renderizador, "imgs/bean_counters/background.png");
    
        SDL_Rect neve = {0,ALTURA*0.37,LARGURA, ALTURA*0.70};
    SDL_Texture * txt_neve = IMG_LoadTexture(renderizador, "imgs/bean_counters/neve.png");
    
    SDL_Rect plataforma = {0,ALTURA*0.80,LARGURA*0.30,ALTURA*0.15};
    SDL_Texture * txt_plataforma = IMG_LoadTexture(renderizador, "imgs/bean_counters/plataforma.png");

    // Pinguim
    Pinguim pinguim;
    inicializa_pinguim(renderizador, &pinguim);

    // Caminhão
    SDL_Texture * caminhao_txt = IMG_LoadTexture(renderizador, "imgs/bean_counters/trucker.png");
    SDL_Rect caminhao = { LARGURA * 0.80, ALTURA*0.15, LARGURA*0.20, ALTURA*0.85};

    // CARGA
    Vetor_carga cargas_jogadas;
    inicializa_vetor_carga(&cargas_jogadas,200);

    // POSIÇÃO EM DOUBLE PARA PINGUIM

    Pilha pilha_1, pilha_2;
    inicializa_pilha(&pilha_1);
    inicializa_pilha(&pilha_2);
    pilha_1.pos_x = LARGURA*0.1;
    pilha_1.pos_y = ALTURA*0.80;

    pilha_2.pos_x = LARGURA*0.15;
    pilha_2.pos_y = ALTURA*0.80;


   TTF_Init();
   TTF_Font* fnt = TTF_OpenFont("fonts/Lovelo/lovelo_black.otf", 50);
   assert(fnt != NULL);
   SDL_Color clr = {0xFF,0xFF,0xFF,0xFF};

   SDL_Surface* sfc_life = TTF_RenderText_Blended(fnt,  "LIFE:  ", clr);
   assert(sfc_life != NULL);
   SDL_Texture* txt_life = SDL_CreateTextureFromSurface(renderizador, sfc_life);
   assert(txt_life != NULL);
   SDL_FreeSurface(sfc_life);

   SDL_Surface* sfc_score = TTF_RenderText_Blended(fnt, "SCORE: ", clr);
   assert(sfc_score != NULL);
   SDL_Texture* txt_score = SDL_CreateTextureFromSurface(renderizador, sfc_score);
   assert(txt_score != NULL);
   SDL_FreeSurface(sfc_score);

   SDL_Rect life = {LARGURA*0.07,ALTURA*0.025,LARGURA*0.05,ALTURA*0.04};
   SDL_Rect score = {LARGURA*0.15,ALTURA*0.025,LARGURA*0.05,ALTURA*0.04};


    SDL_ShowCursor(SDL_DISABLE);

    while (true)
    {
        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 255);
        SDL_RenderClear(renderizador);

        if(pinguim.atingido && (SDL_GetTicks() - pinguim.momento_queda) >= 3000)
        {
            pinguim.atingido = false;
            pinguim.rect.x = pinguim.posicao_inicial;
            pinguim.destino = pinguim.posicao_inicial;
            pinguim.incremento = 0;
            pinguim.txt = pinguim.texturas[0];
            pinguim.last = SDL_GetTicks();
        }

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
            
        }

        // ============================
        //  MOVIMENTO DA CARGA (FÍSICA)
        // ============================

        if (!pinguim.atingido)
        {
            sorteia_carga(renderizador, &cargas_jogadas);
            calcula_movimento_pinguim(&pinguim);
        }
        calcula_movimento_cargas(&cargas_jogadas);
       
   if (!pinguim.atingido)
        {
            for(int i = 0; i <= cargas_jogadas.ultimo_index; i++)
            {
                if (cargas_jogadas.cargas[i].ativo && !cargas_jogadas.cargas[i].fade)
                {
                    if (SDL_HasIntersection(&pinguim.rect, &cargas_jogadas.cargas[i].rect))
                    {
                        cargas_jogadas.cargas[i].ativo = false;

                        if (cargas_jogadas.cargas[i].tipo == PEIXE)
                        {
                            pinguim.txt = pinguim.texturas[8];
                            pinguim.vidas -=1;
                            pinguim.atingido = true;
                            pinguim.momento_queda = SDL_GetTicks();
                            pinguim.sacos = 0;
                            pinguim.porcentagem_peso = 1;
                        }
                        else if (cargas_jogadas.cargas[i].tipo == VASO)
                        {
                            pinguim.txt = pinguim.texturas[9];
                            pinguim.vidas -=1;
                            pinguim.atingido = true;
                            pinguim.momento_queda = SDL_GetTicks();
                            pinguim.sacos = 0;
                            pinguim.porcentagem_peso = 1;
                        }
                        else if (cargas_jogadas.cargas[i].tipo == BIGORNA)
                        {
                            pinguim.txt = pinguim.texturas[7];
                            pinguim.vidas -=1;
                            pinguim.atingido = true;
                            pinguim.momento_queda = SDL_GetTicks();
                            pinguim.sacos = 0;
                            pinguim.porcentagem_peso = 1;
                        }
                        else  if (cargas_jogadas.cargas[i].tipo == GRAOS)
                        {
                            if (pinguim.sacos <5)
                            {
                                pinguim.sacos +=1;
                                pinguim.score +=8;
                                pinguim.porcentagem_peso -= 0.10;
                                pinguim.ultima_entrega = SDL_GetTicks();
                                pinguim.estado_carga = (Estados_pinguim_carga)pinguim.sacos;
                                pinguim.txt = pinguim.texturas[pinguim.sacos];
                                cargas_jogadas.cargas[i].ativo = false;

                            }
                            else
                            {
                                pinguim.vidas -=1;
                                pinguim.atingido = true;
                                pinguim.momento_queda = SDL_GetTicks();
                                pinguim.sacos = 0;
                                pinguim.porcentagem_peso = 1;
                                pinguim.txt = pinguim.texturas[6];
                            }
                        }
                    }

                }

                if (pinguim.sacos >= 1 && SDL_HasIntersection(&pinguim.rect,&plataforma) && (SDL_GetTicks() - pinguim.ultima_entrega) >= pinguim.intervalo_entrega)
                {
                    pinguim.ultima_entrega = SDL_GetTicks();
                    pinguim.sacos--;
                    pinguim.txt = pinguim.texturas[pinguim.sacos];
                    pinguim.porcentagem_peso +=0.10;
                    pinguim.score +=12;
                    if(!pilha_cheia(&pilha_1))
                    {
                        push(renderizador,&pilha_1);
                    }
                    else if (!pilha_cheia(&pilha_2))
                    {
                        push(renderizador,&pilha_2);
                    }
                }
            }   
        }

        for (int i = 0; i <= cargas_jogadas.ultimo_index; i++)
        {
            Carga * carga = &cargas_jogadas.cargas[i];
            if (carga->ativo && !carga->fade)
            {            
                if (SDL_HasIntersection(&chao, &carga->rect))
                {
                    carga->fade = true;
                    carga->tempo_queda = SDL_GetTicks();
                }
            }
        }

        // ============================
        //        DESENHO
        // ============================
        char buffer[32];   // string que vamos montar
        sprintf(buffer, "LIFE: %d", pinguim.vidas);   // monta a string

        sfc_life = TTF_RenderText_Blended(fnt, buffer, clr);
        assert(sfc_life != NULL);
        txt_life = SDL_CreateTextureFromSurface(renderizador, sfc_life);
        assert(txt_life != NULL);
        
        sprintf(buffer, "SCORE: %d", pinguim.score);   // monta a string
        sfc_score = TTF_RenderText_Blended(fnt, buffer, clr);
        assert(sfc_score != NULL);
        txt_score = SDL_CreateTextureFromSurface(renderizador, sfc_score);
        assert(txt_score != NULL);
        
        SDL_RenderCopy(renderizador, txt_background, NULL, &background);

        SDL_RenderCopy(renderizador, txt_plataforma, NULL, &plataforma);
        draw_pilha(renderizador,&pilha_1);
        draw_pilha(renderizador,&pilha_2);

        draw_cargas(renderizador, &cargas_jogadas);

        SDL_RenderCopy(renderizador, caminhao_txt, NULL, &caminhao);

        SDL_RenderCopy(renderizador, pinguim.txt, NULL, &pinguim.rect);

        SDL_RenderCopy(renderizador, txt_neve, NULL, &neve);

        SDL_RenderCopy(renderizador, txt_life, NULL, &life);
        SDL_RenderCopy(renderizador, txt_score, NULL, &score);

        SDL_RenderPresent(renderizador);
    }

    SDL_ShowCursor(SDL_ENABLE);
    IMG_Quit();
    return 1;
}

#endif
