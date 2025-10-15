#include <SDL2/SDL.h>
#include "./consts/consts.h"
#include "./components/loading/loading.h"
#include "./components/menu/menu.h"
#include "./components/personalizacao/personalizacao.h"
#include "./components/sled_racing/sled_racing.h"


int main(int args, char* argc[]) {
    SDL_Event evento;

    // Inicialização SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Erro ao iniciar SDL: %s", SDL_GetError());
        return -1;
    }

    // Dimensões da tela
    int LARGURA = LARGURA_PADRAO;
    int ALTURA = ALTURA_PADRAO;
    obterTamanhoMonitor(&LARGURA, &ALTURA);

    // Criação da janela e renderizador
    SDL_Window *janela = SDL_CreateWindow(
        "Ilha Geladeira",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGURA,
        ALTURA,
        SDL_WINDOW_SHOWN
    );

    if (!janela) {
        SDL_Log("Erro ao criar janela: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
    if (!renderizador) {
        SDL_Log("Erro ao criar renderizador: %s", SDL_GetError());
        SDL_DestroyWindow(janela);
        SDL_Quit();
        return -1;
    }

    // Estado inicial
    GameState estadoJogo = STATE_LOADING;

    // Loop principal da FSM
    while (rodando) {
                
        switch (estadoJogo) {
            case STATE_LOADING:
                // Para começar, apenas renderiza e passa para MENU
                RenderLoadingScreen(janela, renderizador, &evento, &timeout, &estadoJogo);
                estadoJogo = STATE_MENU;
                break;

            case STATE_MENU:
                RenderMenuScreen(janela, renderizador, &evento, &timeout, &estadoJogo);
                // Exemplo de transição manual: tecla 1 para PERSONALIZACAO, ESC para sair
                while (SDL_PollEvent(&evento)) {
                    if (evento.type == SDL_QUIT) estadoJogo = STATE_SAIR;
                    if (evento.type == SDL_KEYDOWN) {
                        if (evento.key.keysym.sym == SDLK_1)
                            estadoJogo = STATE_PERSONALIZACAO;
                        else if (evento.key.keysym.sym == SDLK_ESCAPE)
                            estadoJogo = STATE_SAIR;
                    }
                }
                break;

            case STATE_PERSONALIZACAO:
                RenderPersonalizacaoScreen(janela, renderizador, &evento, &timeout, &estadoJogo);
                // Exemplo de retorno para menu
                while (SDL_PollEvent(&evento)) {
                    if (evento.type == SDL_QUIT) estadoJogo = STATE_SAIR;
                    if (evento.type == SDL_KEYDOWN) {
                        if (evento.key.keysym.sym == SDLK_ESCAPE)
                            estadoJogo = STATE_MENU;
                    }
                }
                break;

            case STATE_SAIR:
                rodando = false;
                break;

            case STATE_JOGANDO:
                RenderSledRacingScreen(janela, renderizador, &evento, &timeout, &estadoJogo);
                rodando = true;
                break;

            default:
                rodando = false;
                break;
        }
    }

    // Libera recursos
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    SDL_Quit();

    return 0;
}
