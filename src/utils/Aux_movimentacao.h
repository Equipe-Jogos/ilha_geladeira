#include <math.h>
#include "../consts/consts.h"

static inline void IniciaMovimentacao(
    float *origem_x, 
    float *origem_y, 
    float destino_x,
    float destino_y,
    float *distancia,
    float *direcao_rad,
    float *velocidade_x, 
    float *velocidade_y,
    float escalar_velocidade
) {
    float dx = destino_x - *origem_x;
    float dy = destino_y - *origem_y;
    *distancia = sqrt(dx*dx + dy*dy);
    *direcao_rad = atan2(dy, dx);

    if (*distancia > 0) {
        *velocidade_x = (dx / *distancia) * escalar_velocidade;
        *velocidade_y = (dy / *distancia) * escalar_velocidade;
    }
}

static inline SDL_Point AtualizaPosicao(
    float *origem_x, 
    float *origem_y, 
    float destino_x, 
    float destino_y, 
    float *velocidade_x, 
    float *velocidade_y, 
    float *distancia
) {
    // Atualiza posição usando variáveis float
    *origem_x += *velocidade_x;
    *origem_y += *velocidade_y;
        
    // Recalcula distância após movimento
    float dx = destino_x - *origem_x;
    float dy = destino_y - *origem_y;
    float nova_distancia = sqrt(dx*dx + dy*dy);
        
    // Se chegou perto o suficiente ou passou do destino, para no destino exato
    if (nova_distancia < 0.5f || nova_distancia >= *distancia) {
        *origem_x = destino_x;
        *origem_y = destino_y;
        *velocidade_x = 0;
        *velocidade_y = 0;
        *distancia = 0;
    }
        
    Coordenada p = { (int)roundf(*origem_x), (int)roundf(*origem_y) };
    return p;
}

static inline int DefineDirecaoCardinal(
    float direcao_rad
){
    if (direcao_rad < 5*M_PI/8 && direcao_rad >= 3*M_PI/8) {
        //sul
        return SUL;
    } else if (direcao_rad < 3*M_PI/8 && direcao_rad >= M_PI/8) {
        //sudeste
        return SUDESTE;
    } else if (direcao_rad < M_PI/8 && direcao_rad >= -M_PI/8) {
        //leste
        return LESTE;
    } else if (direcao_rad < -M_PI/8 && direcao_rad >= -3*M_PI/8) {
        //nordeste
        return NORDESTE;
    } else if (direcao_rad < -3*M_PI/8 && direcao_rad >= -5*M_PI/8) {
        //norte
        return NORTE;
    } else if (direcao_rad < -5*M_PI/8 && direcao_rad >= -7*M_PI/8) {
        //noroeste
        return NOROESTE;
    } else if (direcao_rad < -7*M_PI/8 || direcao_rad >= 7*M_PI/8) {
        //oeste
        return OESTE;
    } else if (direcao_rad < 7*M_PI/8 && direcao_rad >= 5*M_PI/8) {
        //sudoeste
        return SUDOESTE;
    }
    return SUL;
}