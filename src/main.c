#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./consts/consts.h"
#include "./utils/Aux_Timeout.h"
#include "./components/loading/loading.h"
#include "./components/menu/menu.h"
#include "./components/personalizacao/personalizacao.h"

int main(int args, char* argc[]){
    
    SDL_Event evento;

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);

    int LARGURA = LARGURA_PADRAO, ALTURA = ALTURA_PADRAO;
    obterTamanhoMonitor(&LARGURA, &ALTURA);

    
    SDL_Window *janela = SDL_CreateWindow(
        "Ilha Geladeira", // Título
        SDL_WINDOWPOS_CENTERED, // Posição X
        SDL_WINDOWPOS_CENTERED, // Posição Y
        LARGURA, ALTURA, 
        SDL_WINDOW_SHOWN // Flag manter vísivel
    );

    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, 0);

    // Mostrar tela de carregamento
    //RenderLoadingScreen(janela, renderizador, &evento, &timeout);
    
    //RenderPersonalizacaoScreen(janela, renderizador, &evento, &timeout);
    
    
    RenderMenuScreen(janela, renderizador, &evento, &timeout);


    // Liberando recursos
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
