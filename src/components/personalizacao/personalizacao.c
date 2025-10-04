#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "../../consts/consts.h"

typedef enum {
    AMARELO,
    AVERMELHADO,
    AZUL_FORTE,
    AZUL_FRACO,
    CIANO,
    CINZA,
    LARANJA,
    MARROM,
    PRETO,
    ROSA_MEDIO,
    ROXO,
    VERDE_FORTE,
    VERDE_MEDIO,
    VERDE_VOMITO,
    VERMELHO,
    TOTAL 
} Cor;

typedef enum {
    BALAO1,
    BALAO2,
    CONTINUAR,
    LOGO,
    TOTAL_ELEMENTOS
} Elementos;

SDL_Texture *pinguins[TOTAL];
SDL_Texture *cores[TOTAL];
SDL_Texture * elementos [TOTAL_ELEMENTOS];

void carregaImagens(SDL_Renderer *renderizador)
{
    pinguins[AMARELO]     = IMG_LoadTexture(renderizador, "imgs/pinguim_amarelo.png");
    pinguins[AVERMELHADO] = IMG_LoadTexture(renderizador, "imgs/pinguim_avermelhado.png");
    pinguins[AZUL_FORTE]  = IMG_LoadTexture(renderizador, "imgs/pinguim_azul_forte.png");
    pinguins[AZUL_FRACO]  = IMG_LoadTexture(renderizador, "imgs/pinguim_azul_fraco.png");
    pinguins[CIANO]       = IMG_LoadTexture(renderizador, "imgs/pinguim_ciano.png");
    pinguins[CINZA]       = IMG_LoadTexture(renderizador, "imgs/pinguim_cinza.png");
    pinguins[LARANJA]     = IMG_LoadTexture(renderizador, "imgs/pinguim_laranja.png");
    pinguins[MARROM]      = IMG_LoadTexture(renderizador, "imgs/pinguim_marrom.png");
    pinguins[PRETO]       = IMG_LoadTexture(renderizador, "imgs/pinguim_preto.png");
    pinguins[ROSA_MEDIO]  = IMG_LoadTexture(renderizador, "imgs/pinguim_rosa_medio.png");
    pinguins[ROXO]        = IMG_LoadTexture(renderizador, "imgs/pinguim_roxo.png");
    pinguins[VERDE_FORTE] = IMG_LoadTexture(renderizador, "imgs/pinguim_verde_forte.png");
    pinguins[VERDE_MEDIO] = IMG_LoadTexture(renderizador, "imgs/pinguim_verde_medio.png");
    pinguins[VERDE_VOMITO]= IMG_LoadTexture(renderizador, "imgs/pinguim_verde_vomito.png");
    pinguins[VERMELHO]    = IMG_LoadTexture(renderizador, "imgs/pinguim_vermelho.png");

    cores[AMARELO]        = IMG_LoadTexture(renderizador, "imgs/cor_amarelo.png");
    cores[AVERMELHADO]    = IMG_LoadTexture(renderizador, "imgs/cor_avermelhado.png");
    cores[AZUL_FORTE]     = IMG_LoadTexture(renderizador, "imgs/cor_azul_forte.png");
    cores[AZUL_FRACO]     = IMG_LoadTexture(renderizador, "imgs/cor_azul_fraco.png");
    cores[CIANO]          = IMG_LoadTexture(renderizador, "imgs/cor_ciano.png");
    cores[CINZA]          = IMG_LoadTexture(renderizador, "imgs/cor_cinza.png");
    cores[LARANJA]        = IMG_LoadTexture(renderizador, "imgs/cor_laranja.png");
    cores[MARROM]         = IMG_LoadTexture(renderizador, "imgs/cor_marrom.png");
    cores[PRETO]          = IMG_LoadTexture(renderizador, "imgs/cor_preto.png");
    cores[ROSA_MEDIO]     = IMG_LoadTexture(renderizador, "imgs/cor_rosa.png");
    cores[ROXO]           = IMG_LoadTexture(renderizador, "imgs/cor_roxo.png");
    cores[VERDE_FORTE]    = IMG_LoadTexture(renderizador, "imgs/cor_verde_forte.png");
    cores[VERDE_MEDIO]    = IMG_LoadTexture(renderizador, "imgs/cor_verde_medio.png");
    cores[VERDE_VOMITO]   = IMG_LoadTexture(renderizador, "imgs/cor_verde_vomito.png");
    cores[VERMELHO]       = IMG_LoadTexture(renderizador, "imgs/cor_vermelho.png");

    elementos[BALAO1]     = IMG_LoadTexture(renderizador, "imgs/balao-1.png");
    elementos[BALAO2]     = IMG_LoadTexture(renderizador, "imgs/balao-2.png");
    elementos[CONTINUAR]  = IMG_LoadTexture(renderizador, "imgs/memu_elementos.png");
    elementos[LOGO]       = IMG_LoadTexture(renderizador, "imgs/capa.png");

}

int RenderPersonalizacaoScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event *evento, Uint32 *timeout) {
    bool editando = true;
    SDL_SetWindowFullscreen(janela, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_SetWindowPosition(janela, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    carregaImagens(renderizador);
    SDL_Texture *pinguim_img = pinguins[AMARELO];
    

    SDL_Rect pinguim= {200, 100, 400, 400};
    SDL_Rect balao1= {150, 400, 200, 200};
    SDL_Rect balao2= {800, 100, 200, 200};
    SDL_Rect continuar= {800, 400, 200, 200};
    SDL_Rect corte = {30,200, 100,80 };

    SDL_Rect rectCores[TOTAL];

    int startX = 900;
    int startY = 250;
    int espaco = 20;   // sem espaço extra, 5 cabem certinho
    int largura = 100;
    int altura  = 100;


    for (int i = 0; i < TOTAL; i++) {
        rectCores[i].x = startX + (i % 5) * (largura + espaco);
        rectCores[i].y = startY + (i / 5) * (altura + espaco);
        rectCores[i].w = largura;
        rectCores[i].h = altura;
    }

    SDL_Point  mouse;

    while (rodando)
    {
        SDL_SetRenderDrawColor(renderizador,225, 225, 255, 0);
        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, elementos[BALAO1], NULL , &balao1);
        SDL_RenderCopy(renderizador, elementos[BALAO2], NULL , &balao2);
        SDL_RenderCopy(renderizador, elementos[CONTINUAR], &corte , &continuar);

        SDL_GetMouseState(&mouse.x, &mouse.y);

        if (AUX_WaitEventTimeout(evento, timeout))
        {
            if (evento->type == SDL_MOUSEBUTTONDOWN)
            {
                for (int i = 0; i< TOTAL; i++)
                {
                    if (SDL_PointInRect(&mouse, &rectCores[i]))
                    {
                        pinguim_img = pinguins[i];
                    }
                }
            }
            else if(evento->type == SDL_QUIT)
            {
                rodando = false;
            }
        }

        SDL_RenderCopy(renderizador, pinguim_img, NULL , &pinguim);
        SDL_SetRenderDrawColor(renderizador,0, 225, 255, 0);
         for (int i = 0; i < TOTAL; i++) 
         {
            SDL_RenderCopy(renderizador, cores[i], NULL , &rectCores[i]);
        }
        SDL_RenderPresent(renderizador);
    }
    // Liberando recursos
    SDL_DestroyTexture(pinguim_img);
    return 1;
}
