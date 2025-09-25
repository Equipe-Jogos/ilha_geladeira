#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./components/loading/loading.cpp"
#include "./components/menu/menu.cpp"

int main(int args, char* argc[]){
    
    bool rodando = true;
    SDL_Event evento;
    Uint32 timeout = 200;

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    
    SDL_Window *janela = SDL_CreateWindow(
        "Ilha Geladeira", // Título
        SDL_WINDOWPOS_CENTERED, // Posição X
        SDL_WINDOWPOS_CENTERED, // Posição Y
        LARGURA, ALTURA, 
        SDL_WINDOW_SHOWN // Flag manter vísivel
    );

    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, 0);

    // Mostrar tela de carregamento
    RenderLoadingScreen(janela, renderizador, &evento, &timeout);
    RenderMenuScreen(janela, renderizador, &evento, &timeout);
    
    while(rodando){
        
        if(AUX_WaitEventTimeout(&evento, &timeout)){
            if(evento.type == SDL_QUIT){
                rodando = false;
            }
        }
    }

    // Liberando recursos
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
