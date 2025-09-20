#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define LARGURA 800
#define ALTURA 600

int main(int args, char* argc[]){

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

    SDL_Texture *capa = IMG_LoadTexture(renderizador, "imgs/capa.png");

    SDL_Rect quadrado = {0, 0, LARGURA, ALTURA};

    SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
    SDL_RenderClear(renderizador);
    SDL_RenderCopy(renderizador, capa, NULL, &quadrado);
    SDL_RenderPresent(renderizador);
    SDL_Delay(3000);

    // Liberando recursos
    SDL_DestroyTexture(capa);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    IMG_Quit();
    SDL_Quit();
    return 0;
}