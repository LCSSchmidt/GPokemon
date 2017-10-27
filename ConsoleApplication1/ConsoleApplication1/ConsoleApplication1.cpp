// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>

#define QUANTIDADE_OPONENTES 4
#define MAPA_LINHA 15
#define MAPA_COLUNA 15

struct Mapa {
	char x_y[MAPA_LINHA][MAPA_COLUNA];
	int jogador;
	int oponentes[QUANTIDADE_OPONENTES];
	int lider_de_ginaso;
	int centro_pokemon;
	int cerca;
};

void tamanho_mapa(Mapa mapa, int linha, int coluna, int auxiliar, int chamadas) {

	if(chamadas == MAPA_LINHA * 2 + MAPA_COLUNA * 2){
		return;
	}

	mapa.x_y[linha][coluna] = '#';

	if (linha == MAPA_LINHA - 1 && coluna != 0) {
		tamanho_mapa(mapa, linha, --coluna, auxiliar, ++chamadas);
	}
	else if (coluna == MAPA_COLUNA - 1) {
		tamanho_mapa(mapa, ++linha, coluna, auxiliar, ++chamadas);
	}
	else if(linha == MAPA_LINHA - auxiliar && coluna == 0){
		tamanho_mapa(mapa, --linha, coluna, --auxiliar, ++chamadas);
	}
	else {
		tamanho_mapa(mapa, linha, ++coluna, auxiliar, ++chamadas);
	}
	
}

void cerca_padrao() {
	
}

int main(){
	Mapa mapa;
	int linha = 0;
	int coluna = 0;
	int auxiliar = 0;
	int chamadas = 0;

	tamanho_mapa(mapa, linha,  coluna, auxiliar, chamadas);
	system("pause");
    return 0;
}

