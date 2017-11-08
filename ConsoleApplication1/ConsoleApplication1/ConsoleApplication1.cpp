// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <conio.h>

using namespace std;

const int MAPA_LINHA = 15;
const int MAPA_COLUNA = 15;
const int MATRIZ_BIDIMENSIONAL = 2;
const int QUANTIDADE_OPONENTES = 3;
const int QUANTIDADE_DE_LIDER_DE_GINASIO = 1;
const int QUANTIDADE_DE_MAPAS = 1;
const int QUANTIDADE_DE_JOGADORES = 1;
const int QUANTIDADE_DE_CERCAS = 10;
const char VAZIO = '\0';
const char CERCA = '#';
const char JOGADOR = 'P';
const char OPONENTE = 'E';
const char LIDER_DE_GINASIO = 'L';
const char CENTRO_POKEMON[2] = { 'C' , 'P' };
const int COR_MAPA = 240;
const int COR_PADRAO = 15;
const int CENTRALIZAR_MAPA[1] = { 50 };
const int LOCAL_INVALIDO = 1000;
const int TIPO_VOADOR = 5;
const int TIPO_ELETRICO = 4;
const int TIPO_GRAMA = 3;
const int TIPO_AGUA = 2;
const int TIPO_FOGO = 1;
const int QUANTIDADE_DE_PODER = 4;

string Nomes_dos_Pokemons[10] = { "Fogomon", "Firemon", "Aguamon", "Watermon", "Gramamon", "Grassmon",
"Eletricomon", "Eletricmon", "Voomon", "Fleemon" };

string Nomes_dos_Poderes[15] = { "Poder de fogo fraco", "Poder de fogo medio", "Poder de fogo forte",
"Poder de agua fraco", "Poder de agua medio", "Poder de agua forte",
"Poder de grama fraco", "Poder de grama medio", "Poder de grama forte",
"Poder eletrico fraco", "Poder eletrico medio", "Poder eletrico forte",
"Poder de voo fraco", "Poder de voo medio", "Poder de voo forte" };

struct Cerca {
	int linha[1];
	int coluna[1];
};

struct Oponente {
	int linha[1];
	int coluna[1];
	int linha_anterior[1];
	int coluna_anterior[1];
};

struct Lider_de_Ginasio {
	int linha[1];
	int coluna[1];
};

struct Jogador {
	int x_y_atual[MATRIZ_BIDIMENSIONAL];
	int x_y_anterior[MATRIZ_BIDIMENSIONAL];
};

struct Mapa {
	char x_y[MAPA_LINHA][MAPA_COLUNA];
	int centro_pokemon;
	Cerca cerca[MAPA_LINHA * 2 + MAPA_COLUNA * 2 + QUANTIDADE_DE_CERCAS - 3];
	Jogador jogador[QUANTIDADE_DE_JOGADORES];
	Oponente oponente[QUANTIDADE_OPONENTES];
	Lider_de_Ginasio lider_de_ginasio[QUANTIDADE_DE_LIDER_DE_GINASIO];
};

struct Poder {
	string Nome;
	int Forca;
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

void definir_resolucao() {
	HWND console = GetConsoleWindow();
	RECT resolution;
	MoveWindow(console, 0, 0, 1000, 550, TRUE);
}

void Poderes_do_Pokemon(Pokemon pokemons[], string Nomes_dos_Poderes[], int Tipos[], int indice, int Indice_do_Poder = 0) {
	if (Indice_do_Poder == 4)
		return;
	if (pokemons[indice].Tipo == TIPO_FOGO) {
		pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 3];
		pokemons[indice].poder[Indice_do_Poder].Tipo = Tipos[0];
		if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de fogo fraco") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 10;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 16 + 10;
		}
		else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de fogo medio") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 25;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 11 + 5;
		}
		else {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 16 + 35;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 6 + 3;
		}
	}

	else if (pokemons[indice].Tipo == TIPO_AGUA) {
		pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 3 + 3];
		pokemons[indice].poder[Indice_do_Poder].Tipo = Tipos[1];
		if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de agua fraco") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 10;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 16 + 10;
		}
		else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de agua medio") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 25;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 11 + 5;
		}
		else {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 16 + 35;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 6 + 3;
		}
	}

	else if (pokemons[indice].Tipo == TIPO_GRAMA) {
		pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 3 + 6];
		pokemons[indice].poder[Indice_do_Poder].Tipo = Tipos[2];
		if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de grama fraco") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 10;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 16 + 10;
		}
		else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de grama medio") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 25;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 11 + 5;
		}
		else {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 16 + 35;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 6 + 3;
		}
	}

	else if (pokemons[indice].Tipo == TIPO_ELETRICO) {
		pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 3 + 6];
		pokemons[indice].poder[Indice_do_Poder].Tipo = Tipos[3];
		if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder eletrico fraco") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 10;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 16 + 10;
		}
		else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder eletrico medio") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 25;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 11 + 5;
		}
		else {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 16 + 35;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 6 + 3;
		}
	}

	else {
		if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de voo fraco") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 10;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 16 + 10;
		}
		else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de voo medio") {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 25;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 11 + 5;
		}
		else {
			pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 16 + 35;
			pokemons[indice].poder[Indice_do_Poder].PP = rand() % 6 + 3;
		}
	}

	Poderes_do_Pokemon(pokemons, Nomes_dos_Poderes, Tipos, indice, ++Indice_do_Poder);
}

void Gerador_de_Pokemon(Pokemon pokemons[], string Nomes_dos_Pokemons[], int Tipos[], string Nomes_dos_Poderes[], int indice = 0) {
	if (indice == 12)
		return;
	pokemons[indice].Tipo = Tipos[rand() % 5];
	pokemons[indice].Vida = rand() % 91 + 100;
	pokemons[indice].Ataque = rand() % 101;
	pokemons[indice].Defesa = rand() % 101;
	pokemons[indice].Velocidade = rand() % 101;
	if (pokemons[indice].Tipo == TIPO_FOGO) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[rand() % 2];
		Poderes_do_Pokemon(pokemons, Nomes_dos_Poderes, Tipos, indice);
	}
	else if (pokemons[indice].Tipo == TIPO_AGUA) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[(rand() % 2) + 2];
		Poderes_do_Pokemon(pokemons, Nomes_dos_Poderes, Tipos, indice);
	}
	else if (pokemons[indice].Tipo == TIPO_GRAMA) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[(rand() % 2) + 4];
		Poderes_do_Pokemon(pokemons, Nomes_dos_Poderes, Tipos, indice);
	}
	else if (pokemons[indice].Tipo == TIPO_ELETRICO) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[(rand() % 2) + 6];
		Poderes_do_Pokemon(pokemons, Nomes_dos_Poderes, Tipos, indice);
	}
	else {
		pokemons[indice].Nome = Nomes_dos_Pokemons[(rand() % 2) + 8];
		Poderes_do_Pokemon(pokemons, Nomes_dos_Poderes, Tipos, indice);
	}
	Gerador_de_Pokemon(pokemons, Nomes_dos_Pokemons, Tipos, Nomes_dos_Poderes, ++indice);
}

void preencher_mapa(Mapa mapa[], int linha, int coluna) {
	if (linha == MAPA_LINHA - 1 && coluna == MAPA_COLUNA - 1) {
		return;
	}

	mapa[0].x_y[linha][coluna] = VAZIO;
	if (coluna != MAPA_COLUNA - 1)
		preencher_mapa(mapa, linha, ++coluna);
	else
		preencher_mapa(mapa, ++linha, 0);
}

void cerca_padrao(Mapa mapa[], int auxiliar, int chamadas, int linha = 0, int coluna = 0) {

	if (chamadas == MAPA_LINHA * 2 + MAPA_COLUNA * 2 - 3) {
		return;
	}

	mapa[0].x_y[linha][coluna] = CERCA;
	mapa[0].cerca[chamadas].linha[0] = linha;
	mapa[0].cerca[chamadas].coluna[0] = coluna;

	if (linha == MAPA_LINHA - 1 && coluna != 0) {
		cerca_padrao(mapa, auxiliar, ++chamadas, linha, --coluna);
	}
	else if (coluna == MAPA_COLUNA - 1) {
		cerca_padrao(mapa, auxiliar, ++chamadas, ++linha, coluna);
	}
	else if (coluna == 0 && linha == auxiliar) {
		cerca_padrao(mapa, --auxiliar, ++chamadas, --linha, coluna);
	}
	else {
		cerca_padrao(mapa, auxiliar, ++chamadas, linha, ++coluna);
	}

}

void desenhar_mapa(Mapa mapa[], int linha, int coluna) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	if (linha == MAPA_LINHA - 1 && coluna == MAPA_COLUNA - 1) {
		cout << mapa[0].x_y[linha][coluna];
		SetConsoleTextAttribute(hConsole, COR_PADRAO);
		return;
	}
	if (coluna == 0) {
		SetConsoleTextAttribute(hConsole, COR_PADRAO);
		cout << setw(CENTRALIZAR_MAPA[0]) << VAZIO;
		SetConsoleTextAttribute(hConsole, COR_MAPA);
		cout << mapa[0].x_y[linha][coluna];
	}else
		cout << mapa[0].x_y[linha][coluna];
	
	if(coluna != MAPA_COLUNA - 1)
		desenhar_mapa(mapa, linha, ++coluna);
	else{
		cout << endl;
		desenhar_mapa(mapa, ++linha, 0);
	}
}

void posicao_do_obstaculo(Mapa mapa[], int cordenadas[]) {

	cordenadas[0] = 1 + rand() % MAPA_LINHA - 1;
	cordenadas[1] = 1 + rand() % MAPA_COLUNA - 1;
	if (mapa[0].x_y[cordenadas[0]][cordenadas[1]] != VAZIO || cordenadas[0] == MAPA_LINHA / 2 && cordenadas[1] + 1 == MAPA_COLUNA / 2) {
		posicao_do_obstaculo(mapa, cordenadas);
	}
}

void criar_obstaculo(Mapa mapa[], int cordenadas[], int quantidade) {
		mapa[0].x_y[cordenadas[0]][cordenadas[1]] = CERCA;
		mapa[0].cerca[MAPA_LINHA * 2 + MAPA_COLUNA * 2 - 3 + quantidade].linha[0] = cordenadas[0];
		mapa[0].cerca[MAPA_LINHA * 2 + MAPA_COLUNA * 2 - 3 + quantidade].coluna[0] = cordenadas[1];
}

void obstaculo(Mapa mapa[], int cordenadas[], int quantidade_de_cercas = -1) {
	if (quantidade_de_cercas == QUANTIDADE_DE_CERCAS - 1) {
		return;
	}

	posicao_do_obstaculo(mapa, cordenadas);
	criar_obstaculo(mapa, cordenadas, ++quantidade_de_cercas);
	obstaculo(mapa, cordenadas, quantidade_de_cercas);
}

void atribuir_posicao_jogador(Mapa mapa_jogador[]) {
	mapa_jogador[0].jogador[0].x_y_atual[0] = MAPA_LINHA / 2;
	mapa_jogador[0].jogador[0].x_y_atual[1] = MAPA_COLUNA / 2;
	mapa_jogador[0].jogador[0].x_y_anterior[0] = MAPA_LINHA / 2;
	mapa_jogador[0].jogador[0].x_y_anterior[1] = MAPA_COLUNA / 2;
}

void posicionar_jogador(Mapa mapa[]) {
	mapa[0].x_y[mapa[0].jogador[0].x_y_atual[0]][mapa[0].jogador[0].x_y_atual[1]] = JOGADOR;
}

void posicionar_oponentes(Mapa mapa[], int auxiliar = 0) {
	if (auxiliar == QUANTIDADE_OPONENTES) {
		return;
	}
	int linha = 1 + rand() % (MAPA_LINHA - 1);
	int coluna = 1 + rand() % (MAPA_COLUNA - 1);

	if (mapa[0].x_y[linha][coluna] == VAZIO) {
		mapa[0].x_y[linha][coluna] = OPONENTE;
		mapa[0].oponente[auxiliar].linha[0] = linha;
		mapa[0].oponente[auxiliar].coluna[0] = coluna;
		posicionar_oponentes(mapa, ++auxiliar);
	}else
		posicionar_oponentes(mapa, auxiliar);
}

void posicionar_lider(Mapa mapa[]) {
	int linha = 1 + rand() % MAPA_LINHA - 1;
	int coluna = 1 + rand() % MAPA_COLUNA - 1;

	if (mapa[0].x_y[linha][coluna] == VAZIO) {
		mapa[0].x_y[linha][coluna] = LIDER_DE_GINASIO;
		mapa[0].lider_de_ginasio[0].linha[0] = linha;
		mapa[0].lider_de_ginasio[0].coluna[0] = coluna;
	}
	else
		posicionar_lider(mapa);
}

void posicionar_centro_pokemon(Mapa mapa[]) {
	int linha = 1 + rand() % MAPA_LINHA - 1;
	int coluna = 1 + rand() % MAPA_COLUNA - 1;

	if (mapa[0].x_y[linha][coluna] == VAZIO && mapa[0].x_y[linha][coluna + 1] == VAZIO) {
		mapa[0].x_y[linha][coluna] = CENTRO_POKEMON[0];
		mapa[0].x_y[linha][coluna + 1] = CENTRO_POKEMON[1];
	}
	else
		posicionar_centro_pokemon(mapa);
}

void posicionar_elementos(Mapa mapa[]) {
	posicionar_jogador(mapa);
	posicionar_oponentes(mapa);
	posicionar_lider(mapa);
	posicionar_centro_pokemon(mapa);
}

bool movimentacao_jogador(Mapa mapa[]) {

	switch (_getch()) {
		case 'w':
			if(mapa[0].x_y[mapa[0].jogador[0].x_y_atual[0] - 1][mapa[0].jogador[0].x_y_atual[1]] == VAZIO){
				mapa[0].jogador[0].x_y_anterior[0] = mapa[0].jogador[0].x_y_atual[0];
				mapa[0].jogador[0].x_y_anterior[1] = mapa[0].jogador[0].x_y_atual[1];
				mapa[0].jogador[0].x_y_atual[0]--;
				return true;
			}
			break;
		case 's':
			if(mapa[0].x_y[mapa[0].jogador[0].x_y_atual[0] + 1][mapa[0].jogador[0].x_y_atual[1]] == VAZIO){
				mapa[0].jogador[0].x_y_anterior[0] = mapa[0].jogador[0].x_y_atual[0];
				mapa[0].jogador[0].x_y_anterior[1] = mapa[0].jogador[0].x_y_atual[1];
				mapa[0].jogador[0].x_y_atual[0]++;
				return true;
			}
			break;
		case 'a':
			if(mapa[0].x_y[mapa[0].jogador[0].x_y_atual[0]][mapa[0].jogador[0].x_y_atual[1] - 1] == VAZIO){
				mapa[0].jogador[0].x_y_anterior[0] = mapa[0].jogador[0].x_y_atual[0];
				mapa[0].jogador[0].x_y_anterior[1] = mapa[0].jogador[0].x_y_atual[1];
				mapa[0].jogador[0].x_y_atual[1]--;
				return true;
			}
			break;
		case 'd':
			if(mapa[0].x_y[mapa[0].jogador[0].x_y_atual[0]][mapa[0].jogador[0].x_y_atual[1] + 1] == VAZIO){
				mapa[0].jogador[0].x_y_anterior[0] = mapa[0].jogador[0].x_y_atual[0];
				mapa[0].jogador[0].x_y_anterior[1] = mapa[0].jogador[0].x_y_atual[1];
				mapa[0].jogador[0].x_y_atual[1]++;
				return true;
			}
			break;
	}
	return false;
}

void movimentar_jogador(Mapa mapa[]) {
	int linha_anterior = mapa[0].jogador[0].x_y_anterior[0];
	int coluna_anterior = mapa[0].jogador[0].x_y_anterior[1];
	int linha_atual = mapa[0].jogador[0].x_y_atual[0];
	int coluna_atual = mapa[0].jogador[0].x_y_atual[1];

	mapa[0].x_y[linha_anterior][coluna_anterior] = VAZIO;
	mapa[0].x_y[linha_atual][coluna_atual] = JOGADOR;
}

void movimentar_oponentes(Mapa mapa[], int oponente = 0) {
	int linha_anterior = mapa[0].oponente[oponente].linha_anterior[0];
	int coluna_anterior = mapa[0].oponente[oponente].coluna_anterior[0];
	int linha_atual = mapa[0].oponente[oponente].linha[0];
	int coluna_atual = mapa[0].oponente[oponente].coluna[0];

	mapa[0].x_y[linha_anterior][coluna_anterior] = VAZIO;
	mapa[0].x_y[linha_atual][coluna_atual] = OPONENTE;
}

void busca_de_jogador(Mapa mapa[], int oponente = 0) {
	if (oponente == QUANTIDADE_OPONENTES) {
		return;
	}

	int linha = mapa[0].oponente[oponente].linha[0];
	int coluna = mapa[0].oponente[oponente].coluna[0];

	if (mapa[0].oponente[oponente].linha[0] < mapa[0].jogador[0].x_y_atual[0] && mapa[0].x_y[linha + 1][coluna] == VAZIO) {
		mapa[0].oponente[oponente].linha_anterior[0] = mapa[0].oponente[oponente].linha[0];
		mapa[0].oponente[oponente].coluna_anterior[0] = mapa[0].oponente[oponente].coluna[0];
		mapa[0].oponente[oponente].linha[0]++;
	}
	else if (mapa[0].oponente[oponente].coluna[0] < mapa[0].jogador[0].x_y_atual[1] && mapa[0].x_y[linha][coluna + 1] == VAZIO) {
		mapa[0].oponente[oponente].linha_anterior[0] = mapa[0].oponente[oponente].linha[0];
		mapa[0].oponente[oponente].coluna_anterior[0] = mapa[0].oponente[oponente].coluna[0];
		mapa[0].oponente[oponente].coluna[0]++;
	}
	else if (mapa[0].oponente[oponente].linha[0] > mapa[0].jogador[0].x_y_atual[0] && mapa[0].x_y[linha - 1][coluna] == VAZIO) {
		mapa[0].oponente[oponente].linha_anterior[0] = mapa[0].oponente[oponente].linha[0];
		mapa[0].oponente[oponente].coluna_anterior[0] = mapa[0].oponente[oponente].coluna[0];
		mapa[0].oponente[oponente].linha[0]--;
	}
	else if (mapa[0].oponente[oponente].coluna[0] > mapa[0].jogador[0].x_y_atual[1] && mapa[0].x_y[linha][coluna - 1] == VAZIO) {
		mapa[0].oponente[oponente].linha_anterior[0] = mapa[0].oponente[oponente].linha[0];
		mapa[0].oponente[oponente].coluna_anterior[0] = mapa[0].oponente[oponente].coluna[0];
		mapa[0].oponente[oponente].coluna[0]--;
	}
	movimentar_oponentes(mapa, oponente);
	busca_de_jogador(mapa, ++oponente);
}

void setCursorPosition_posicao_anterior(Mapa mapa[], string elemento_a_movimentar, int oponente = 100)	
{
	int x;
	int y;
	if (elemento_a_movimentar == "Jogador") {
		x = mapa[0].jogador[0].x_y_anterior[1];
		y = mapa[0].jogador[0].x_y_anterior[0];
	}
	else {
		x = mapa[0].oponente[oponente].coluna_anterior[0];
		y = mapa[0].oponente[oponente].linha_anterior[0];
	}
		
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COR_MAPA);
	COORD cordenadas_cursor = { (SHORT)x + 50, (SHORT)y };
	SetConsoleCursorPosition(hOut, cordenadas_cursor);
}

void setCursorPosition_posicao_atual(Mapa mapa[], string elemento_a_movimentar, int oponente = 100)
{
	int x;
	int y;
	if (elemento_a_movimentar == "Jogador") {
		x = mapa[0].jogador[0].x_y_atual[1];
		y = mapa[0].jogador[0].x_y_atual[0];
	}
	else {
		x = mapa[0].oponente[oponente].coluna[0];
		y = mapa[0].oponente[oponente].linha[0];
	}
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cordenadas_cursor = { (SHORT)x + 50, (SHORT)y };
	SetConsoleCursorPosition(hOut, cordenadas_cursor);
}

void iniciar_duelo(Mapa mapa, Pokemon pokemon) {

}

void verificar_se_a_duelo(Mapa mapa[], int oponente = 0) {
	if(oponente == QUANTIDADE_OPONENTES){
		return;
	}
	if (mapa[0].oponente[oponente].linha[0] == (mapa[0].jogador[0].x_y_atual[0] - 1) && mapa[0].oponente[oponente].coluna[0] == mapa[0].jogador[0].x_y_atual[1]) {
		//iniciar_duelo();
	}
	else if (mapa[0].oponente[oponente].linha[0] == mapa[0].jogador[0].x_y_atual[0] && mapa[0].oponente[oponente].coluna[0] == (mapa[0].jogador[0].x_y_atual[1] + 1)){
		//iniciar_duelo();
	}
	else if (mapa[0].oponente[oponente].linha[0] == (mapa[0].jogador[0].x_y_atual[0] + 1) && mapa[0].oponente[oponente].coluna[0] == mapa[0].jogador[0].x_y_atual[1]) {
		//iniciar_duelo();
	}
	else if (mapa[0].oponente[oponente].linha[0] == mapa[0].jogador[0].x_y_atual[0] && mapa[0].oponente[oponente].coluna[0] == (mapa[0].jogador[0].x_y_atual[1] - 1)) {
		//iniciar_duelo();
	}
	verificar_se_a_duelo(mapa, ++oponente);
}

void atualizar_mapa(Mapa mapa[]) {
	int linha_anterior_jogador = mapa[0].jogador[0].x_y_anterior[0];
	int coluna_anterior_jogador = mapa[0].jogador[0].x_y_anterior[1];
	int linha_atual_jogador = mapa[0].jogador[0].x_y_atual[0];
	int coluna_atual_jogador = mapa[0].jogador[0].x_y_atual[1];

	setCursorPosition_posicao_anterior(mapa, "Jogador");
	cout << mapa[0].x_y[linha_anterior_jogador][coluna_anterior_jogador];
	setCursorPosition_posicao_atual(mapa, "Jogador");
	cout << mapa[0].x_y[linha_atual_jogador][coluna_atual_jogador];
	for (int indice = 0; indice < QUANTIDADE_OPONENTES; indice++) {
		int linha_anterior = mapa[0].oponente[indice].linha_anterior[0];
		int coluna_anterior = mapa[0].oponente[indice].coluna_anterior[0];
		int linha = mapa[0].oponente[indice].linha[0];
		int coluna = mapa[0].oponente[indice].coluna[0];


		setCursorPosition_posicao_anterior(mapa, "Oponente", indice);
		cout << mapa[0].x_y[linha_anterior][coluna_anterior];
		setCursorPosition_posicao_atual(mapa, "Oponente", indice);
		cout << mapa[0].x_y[linha][coluna];
	}
}

void jogo(Mapa mapa[], int linha, int coluna) {
	if(movimentacao_jogador(mapa)){
		movimentar_jogador(mapa);
		busca_de_jogador(mapa);
		atualizar_mapa(mapa);
		verificar_se_a_duelo(mapa);
	}
	jogo(mapa, linha, coluna);
}

int main() {
	Mapa mapa[QUANTIDADE_DE_MAPAS];
	Pokemon pokemons[12];
	int cordenadas_spawn_obstaculos[2] = { 0 };
	int linha = 0;
	int coluna = 0;
	int auxiliar = MAPA_LINHA - 1;
	int chamadas = 0;
	int quantidade_de_oponentes = 4;
	int Tipos[5] = { TIPO_FOGO,TIPO_AGUA,TIPO_GRAMA, TIPO_ELETRICO, TIPO_VOADOR };
	

	srand(time(NULL));
	Gerador_de_Pokemon(pokemons, Nomes_dos_Pokemons, Tipos, Nomes_dos_Poderes);
	mapa[0].lider_de_ginasio[0].linha[0] = 0;
	definir_resolucao();
	preencher_mapa(mapa, linha, coluna);
	cerca_padrao(mapa, auxiliar, chamadas);
	obstaculo(mapa, cordenadas_spawn_obstaculos);
	atribuir_posicao_jogador(mapa);
	posicionar_elementos(mapa);
	desenhar_mapa(mapa, linha, coluna);
	jogo(mapa, linha, coluna);
	system("pause");
    return 0;
}