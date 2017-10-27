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
	mapa.jogador = 0;
	tamanho_mapa(mapa, linha,  coluna, auxiliar, chamadas);
	system("pause");
    return 0;
}

#pragma region Leonardo
// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"[]
#include <iostream>
#include <string>
#include <time.h>

#define QUANTIDADE_DE_PODER 4
using namespace std;

struct Poder {
	string Nome;
	int Poder;
	int Tipo;
	int PP;
};

struct Pokemon {
	string Nome;
	int Vida;
	int Ataque;
	int Defesa;
	int Velocidade;
	int Tipo;
	Poder poder[QUANTIDADE_DE_PODER];
};

void Poderes_do_Pokemon(int indice, Pokemon pokemons[], int Indice_do_Poder, string Nomes_dos_Poderes) {
	pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 6];

}

void Gerador_de_Pokemon(int indice, Pokemon pokemons[], string Nomes_dos_Pokemons[], int Tipos[]) {
	if (indice == 12)
		return;
	pokemons[indice].Tipo = Tipos[rand() % 6];
	pokemons[indice].Nome = Nomes_dos_Pokemons[rand() % 13];
	pokemons[indice].Vida = rand() % 91 + 100;
	pokemons[indice].Ataque = rand() % 101;
	pokemons[indice].Defesa = rand() % 101;
	pokemons[indice].Velocidade = rand() % 101;

	if (pokemons[indice].Tipo == 0) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[rand() % 1];
		Poderes_do_Pokemon();
	}
	else if (pokemons[indice].Tipo == 1) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[rand() % 13];
		Poderes_do_Pokemon();
	}
	else if (pokemons[indice].Tipo == 2) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[rand() % 13];
		Poderes_do_Pokemon();
	}
	else if (pokemons[indice].Tipo == 3) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[rand() % 13];
		Poderes_do_Pokemon();
	}
	else {
		pokemons[indice].Nome = Nomes_dos_Pokemons[rand() % 13];
		Poderes_do_Pokemon();
	}


	Gerador_de_Pokemon(++indice, pokemons);
}

int main()
{
	srand(time(NULL));
	string Nomes_dos_Pokemons[12] = { "Fogomon", "Firemon", "Aguamon", "Watermon", "Gramamon", "Grassmon",
		"Eletricomon", "Eletricmon", "Voomon", "Fleemon" };

	string Nomes_dos_Poderes[15] = { "Poder de fogo fraco", "Poder de fogo medio", "Poder de fogo forte",
		"Poder de agua fraco", "Poder de agua medio", "Poder de agua forte",
		"Poder de grama fraco", "Poder de grama medio", "Poder de grama forte",
		"Poder eletrico fraco", "Poder eletrico medio", "Poder eletrico forte",
		"Poder de voo fraco", "Poder de voo medio", "Poder de voo forte" };

	string Tipos[5] = { "Fogo", "Agua", "Grama", "Eletrico", "Voador" };

	Pokemon pokemons[12];

	int indice = 0;
	Gerador_de_Pokemon(indice, pokemons, Nomes_dos_Pokemons, Tipos);
	return 0;
}

#pragma endregion
