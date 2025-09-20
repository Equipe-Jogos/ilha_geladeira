#include <SDL2/SDL.h>

#define LARGURA 800
#define ALTURA 600

int main(int args, char* argc[]){

    // Inicializando SDL
    SDL_Init(SDL_INIT_EVERYTHING);
     
    SDL_Window *janela = SDL_CreateWindow(
        "Ilha Geladeira", // Título
        SDL_WINDOWPOS_CENTERED, // Posição X
        SDL_WINDOWPOS_CENTERED, // Posição Y
        LARGURA, ALTURA, 
        SDL_WINDOW_SHOWN // Flag manter vísivel
    );

    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, 0);


    // Liberando recursos
    SDL_DestroyRenderer(renderizador)
    SDL_DestroyWindow(janela);
    SDL_Quit();
    return 0;
}