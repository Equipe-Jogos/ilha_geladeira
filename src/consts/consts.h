#ifndef CONSTS_H
#define CONSTS_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../utils/Aux_motinor.h"

/*
Caso haja constantes de outro domínio, adicione um comentário 
para entitular o domínio e liste as constantes seguindo o padrão abaixo
*/

//CONSTANTES DE TELA
const int LARGURA_PADRAO = 800;
const int ALTURA_PADRAO = 600;


//CONSTANTES DE SISTEMA 
Uint32 timeout = 200;
bool rodando = true;

typedef enum {
    STATE_LOADING,
    STATE_MENU,
    STATE_JOGANDO,
    STATE_PERSONALIZACAO,
    STATE_SAIR
} GameState;


#endif