#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define LARGURA 800
#define ALTURA 600

int AUX_WaitEventTimeout(SDL_Event *evt, Uint32 *ms) {
    if (ms == NULL) {
        return 0; // segurança
    }

    Uint32 start = SDL_GetTicks();
    int result = SDL_WaitEventTimeout(evt, *ms);
    Uint32 end = SDL_GetTicks();
    Uint32 elapsed = end - start;

    if (elapsed >= *ms) {
        *ms = 0;
    } else {
        *ms -= elapsed;
    }

    return result; 
}

int main(int args, char* argc[]){
    
    bool rodando = true;
    SDL_Event evento;
    int timeout = 200;

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

    SDL_Rect img_capa = {0, 0, LARGURA, ALTURA};

    SDL_Rect carregamento = {0, 0, 1, 20};

    while(rodando){
        
        if(carregamento.w < 800){
            SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
            SDL_RenderClear(renderizador);
            SDL_RenderCopy(renderizador, capa, NULL, &img_capa);
            SDL_SetRenderDrawColor(renderizador, 0, 255, 0, 0);
            SDL_RenderFillRect(renderizador, &carregamento);
            
            // Evento fechar janela ALT + F4 ou [X]
            if(AUX_WaitEventTimeout(&evento, &timeout)){
                if(evento.type == SDL_QUIT){
                    rodando = false;
                }
            }
            carregamento.w += 1;
            SDL_RenderPresent(renderizador);
        }else{
            rodando = false;
        }
        
    }

    // Liberando recursos
    SDL_DestroyTexture(capa);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    IMG_Quit();
    SDL_Quit();
    return 0;
}