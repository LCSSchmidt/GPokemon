// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <conio.h>


#define MAPA_LINHA 15
#define MAPA_COLUNA 15
const int MATRIZ_BIDIMENSIONAL = 2;
const int QUANTIDADE_OPONENTES = 4;
const int QUANTIDADE_DE_MAPAS = 1;
const int QUANTIDADE_DE_JOGADORES = 1;
const char VAZIO = '\0';
const char CERCA = '#';
const char JOGADOR = 'P';
const char OPONENTE = 'E';
const char LIDER_DE_GINASIO = 'L';
const char CENTRO_POKEMON[2] = { 'C' , 'P' };
const int COR_MAPA = 240;
const int COR_PADRAO = 15;
const int CENTRALIZAR_MAPA[1] = { 50 };

using namespace std;
struct Jogador {
	int x_y_atual[MATRIZ_BIDIMENSIONAL];
	int x_y_anterior[MATRIZ_BIDIMENSIONAL];
};

struct Mapa {
	char x_y[MAPA_LINHA][MAPA_COLUNA];
	int jogador[MATRIZ_BIDIMENSIONAL];
	int oponentes[QUANTIDADE_OPONENTES];
	int lider_de_ginaso;
	int centro_pokemon;
	int cerca[MAPA_LINHA][MAPA_COLUNA];
};

void definir_resolucao() {
	HWND console = GetConsoleWindow();
	RECT resolution;
	GetWindowRect(console, &resolution);
	MoveWindow(console, resolution.left, resolution.top, 1000, 550, TRUE);
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

void cerca_padrao(Mapa mapa[], int linha, int coluna, int auxiliar, int chamadas) {

	if (chamadas == MAPA_LINHA * 2 + MAPA_COLUNA * 2) {
		return;
	}

	mapa[0].x_y[linha][coluna] = CERCA;

	if (linha == MAPA_LINHA - 1 && coluna != 0) {
		cerca_padrao(mapa, linha, --coluna, auxiliar, ++chamadas);
	}
	else if (coluna == MAPA_COLUNA - 1) {
		cerca_padrao(mapa, ++linha, coluna, auxiliar, ++chamadas);
	}
	else if (coluna == 0 && linha == auxiliar) {
		cerca_padrao(mapa, --linha, coluna, --auxiliar, ++chamadas);
	}
	else {
		cerca_padrao(mapa, linha, ++coluna, auxiliar, ++chamadas);
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

int tamanho_do_obstaculo() {
	return 2 + rand() % 4;
}

void posicao_do_obstaculo(Mapa mapa[], int cordenadas[]) {

	cordenadas[0] = 1 + rand() % MAPA_LINHA - 1;
	cordenadas[1] = 1 + rand() % MAPA_COLUNA - 1;
	if (mapa[0].x_y[cordenadas[0]][cordenadas[1]] != VAZIO || cordenadas[0] == MAPA_LINHA / 2 && cordenadas[1] + 1 == MAPA_COLUNA / 2) {
		posicao_do_obstaculo(mapa, cordenadas);
	}
}

void criar_obstaculos(Mapa mapa[], int cordenadas[], int tamanho_obstaculo) {
	if (tamanho_obstaculo == 0) {
		return;
	}

	if (mapa[0].x_y[cordenadas[0]][cordenadas[1] + 1] == VAZIO && !(cordenadas[0] == MAPA_LINHA / 2) && !(cordenadas[1] + 1 == MAPA_COLUNA / 2)) {
		mapa[0].x_y[cordenadas[0]][cordenadas[1] + 1] = CERCA;
		cordenadas[1]++;
	}
	else if (mapa[0].x_y[cordenadas[0] + 1][cordenadas[1]] == VAZIO && !(cordenadas[0] + 1 == MAPA_LINHA / 2) && !(cordenadas[1] == MAPA_COLUNA / 2)) {
		mapa[0].x_y[cordenadas[0] + 1][cordenadas[1]] = CERCA;
		cordenadas[0]++;
	}
	else if (mapa[0].x_y[cordenadas[0]][cordenadas[1] - 1] == VAZIO && !(cordenadas[0] == MAPA_LINHA / 2) && !(cordenadas[1] - 1 == MAPA_COLUNA / 2)) {
		mapa[0].x_y[cordenadas[0]][cordenadas[1] - 1] = CERCA;
		cordenadas[1]--;
	}
	else if (mapa[0].x_y[cordenadas[0] - 1][cordenadas[1]] == VAZIO && !(cordenadas[0] - 1 == MAPA_LINHA / 2) && !(cordenadas[1] == MAPA_COLUNA / 2)) {
		mapa[0].x_y[cordenadas[0] - 1][cordenadas[1]] = CERCA;
		cordenadas[0]--;
	}
	else if (mapa[0].x_y[cordenadas[0] + 1][cordenadas[1] + 1] == VAZIO && !(cordenadas[0]  + 1 == MAPA_LINHA / 2) && !(cordenadas[1] + 1 == MAPA_COLUNA / 2)) {
		mapa[0].x_y[cordenadas[0] + 1][cordenadas[1] + 1] = CERCA;
		cordenadas[0]++;
		cordenadas[1]++;
	}
	else if (mapa[0].x_y[cordenadas[0] - 1][cordenadas[1] - 1] == VAZIO && !(cordenadas[0] - 1 == MAPA_LINHA / 2) && !(cordenadas[1] - 1 == MAPA_COLUNA / 2)) {
		mapa[0].x_y[cordenadas[0] - 1][cordenadas[1] - 1] = CERCA;
		cordenadas[0]--;
		cordenadas[1]--;
	}
	else if (mapa[0].x_y[cordenadas[0] - 1][cordenadas[1] + 1] == VAZIO && !(cordenadas[0] - 1 == MAPA_LINHA / 2) && !(cordenadas[1] + 1 == MAPA_COLUNA / 2)) {
		mapa[0].x_y[cordenadas[0] - 1][cordenadas[1] + 1] = CERCA;
		cordenadas[0]--;
		cordenadas[1]++;
	}
	else if (mapa[0].x_y[cordenadas[0] + 1][cordenadas[1] - 1] == VAZIO && !(cordenadas[0] + 1 == MAPA_LINHA / 2) && !(cordenadas[1] - 1 == MAPA_COLUNA / 2)) {
		mapa[0].x_y[cordenadas[0] + 1][cordenadas[1] - 1] = CERCA;
		cordenadas[0]++;
		cordenadas[1]--;
	}
	criar_obstaculos(mapa, cordenadas, --tamanho_obstaculo);
}

void obstaculo(Mapa mapa[], int cordenadas[], int quantidade_de_cercas) {
	if (quantidade_de_cercas == 0) {
		return;
	}

	int tamanho = tamanho_do_obstaculo();

	posicao_do_obstaculo(mapa, cordenadas);
	criar_obstaculos(mapa, cordenadas, tamanho);
	obstaculo(mapa, cordenadas, --quantidade_de_cercas);
}

void atribuir_posicao_jogador(Jogador jogador[]) {
	jogador[0].x_y_atual[0] = MAPA_LINHA / 2;
	jogador[0].x_y_atual[1] = MAPA_COLUNA / 2;
	jogador[0].x_y_anterior[0] = MAPA_LINHA / 2;
	jogador[0].x_y_anterior[1] = MAPA_COLUNA / 2;
}

void posicionar_jogador(Mapa mapa[], Jogador jogador[]) {
	mapa[0].x_y[jogador[0].x_y_atual[0]][jogador[0].x_y_atual[1]] = JOGADOR;
}

void posicionar_oponentes(Mapa mapa[], int quantidade_de_oponentes) {
	if (quantidade_de_oponentes == 1) {
		return;
	}

	int linha = 1 + rand() % MAPA_LINHA - 1;
	int coluna = 1 + rand() % MAPA_COLUNA - 1;

	if (mapa[0].x_y[linha][coluna] == VAZIO) {
		mapa[0].x_y[linha][coluna] = OPONENTE;
		posicionar_oponentes(mapa, --quantidade_de_oponentes);
	}else
		posicionar_oponentes(mapa, quantidade_de_oponentes);
}

void posicionar_lider(Mapa mapa[]) {
	int linha = 1 + rand() % MAPA_LINHA - 1;
	int coluna = 1 + rand() % MAPA_COLUNA - 1;

	if (mapa[0].x_y[linha][coluna] == VAZIO) {
		mapa[0].x_y[linha][coluna] = LIDER_DE_GINASIO;
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

void posicionar_elementos(Mapa mapa[], int quantidade_de_oponentes, Jogador jogador[]) {
	posicionar_jogador(mapa, jogador);
	posicionar_oponentes(mapa, quantidade_de_oponentes);
	posicionar_lider(mapa);
	posicionar_centro_pokemon(mapa);
}

/*void inicializar_jogo(Mapa mapa[], int linha, int coluna, int auxiliar, int chamadas, int cordenadas_spawn_obstaculos[], int quantidade_de_cercas, int quantidade_de_oponentes) {
	
}*/

bool movimentacao_jogador(Mapa mapa[], Jogador jogador[]) {

	switch (_getch()) {
		case 'w':
			if(mapa[0].x_y[jogador[0].x_y_atual[0] - 1][jogador[0].x_y_atual[1]] == VAZIO){
				jogador[0].x_y_anterior[0] = jogador[0].x_y_atual[0];
				jogador[0].x_y_anterior[1] = jogador[0].x_y_atual[1];
				jogador[0].x_y_atual[0]--;
				return true;
			}
			break;
		case 's':
			if(mapa[0].x_y[jogador[0].x_y_atual[0] + 1][jogador[0].x_y_atual[1]] == VAZIO){
				jogador[0].x_y_anterior[0] = jogador[0].x_y_atual[0];
				jogador[0].x_y_anterior[1] = jogador[0].x_y_atual[1];
				jogador[0].x_y_atual[0]++;
				return true;
			}
			break;
		case 'a':
			if(mapa[0].x_y[jogador[0].x_y_atual[0]][jogador[0].x_y_atual[1] - 1] == VAZIO){
				jogador[0].x_y_anterior[0] = jogador[0].x_y_atual[0];
				jogador[0].x_y_anterior[1] = jogador[0].x_y_atual[1];
				jogador[0].x_y_atual[1]--;
				return true;
			}
			break;
		case 'd':
			if(mapa[0].x_y[jogador[0].x_y_atual[0]][jogador[0].x_y_atual[1] + 1] == VAZIO){
				jogador[0].x_y_anterior[0] = jogador[0].x_y_atual[0];
				jogador[0].x_y_anterior[1] = jogador[0].x_y_atual[1];
				jogador[0].x_y_atual[1]++;
				return true;
			}
			break;
	}
	return false;
}

void movimentar_jogador(Mapa mapa[], Jogador jogador[]) {
	mapa[0].x_y[jogador[0].x_y_anterior[0]][jogador[0].x_y_anterior[1]] = VAZIO;
	mapa[0].x_y[jogador[0].x_y_atual[0]][jogador[0].x_y_atual[1]] = JOGADOR;
}

bool setCursorPosition_posicao_anterior(Mapa mapa[], Jogador jogador[])
{
	int x = jogador[0].x_y_anterior[1];
	int y = jogador[0].x_y_anterior[0];
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COR_MAPA);
	COORD cordenadas_cursor = { (SHORT)x + 50, (SHORT)y };
	SetConsoleCursorPosition(hOut, cordenadas_cursor);
	return true;
}

bool setCursorPosition_posicao_atual(Mapa mapa[], Jogador jogador[])
{
	int x = jogador[0].x_y_atual[1];
	int y = jogador[0].x_y_atual[0];

	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COR_MAPA);
	COORD cordenadas_cursor = { (SHORT)x + 50, (SHORT)y };
	SetConsoleCursorPosition(hOut, cordenadas_cursor);
	return true;
}

void atualizar_mapa(Mapa mapa[], Jogador jogador[]) {
	if (setCursorPosition_posicao_anterior(mapa, jogador)) {
		cout << mapa[0].x_y[jogador[0].x_y_anterior[0]][jogador[0].x_y_anterior[1]];
	}
	if (setCursorPosition_posicao_atual(mapa, jogador)) {
		cout << mapa[0].x_y[jogador[0].x_y_atual[0]][jogador[0].x_y_atual[1]];
	}
}

void jogo(Mapa mapa[], int linha, int coluna, Jogador jogador[]) {
	if(movimentacao_jogador(mapa, jogador)){
		movimentar_jogador(mapa, jogador);
		atualizar_mapa(mapa, jogador);
	}
	jogo(mapa, linha, coluna, jogador);
	
}

int main() {
	Mapa mapa[QUANTIDADE_DE_MAPAS];
	Jogador jogador[QUANTIDADE_DE_JOGADORES];
	int cordenadas_spawn_obstaculos[2] = { 0 };
	int linha = 0;
	int coluna = 0;
	int auxiliar = MAPA_LINHA - 1;
	int chamadas = 0;
	int quantidade_de_oponentes = 4;
	int quantidade_de_cercas = 10;

	srand(time(NULL));
	mapa[0].jogador[0] = 0;
	definir_resolucao();
	preencher_mapa(mapa, linha, coluna);
	cerca_padrao(mapa, linha, coluna, auxiliar, chamadas);
	obstaculo(mapa, cordenadas_spawn_obstaculos, quantidade_de_cercas);
	atribuir_posicao_jogador(jogador);
	posicionar_elementos(mapa, quantidade_de_oponentes, jogador);
	//inicializar_jogo(mapa, linha, coluna, auxiliar, chamadas, cordenadas_spawn_obstaculos, quantidade_de_cercas, quantidade_de_oponentes);
	desenhar_mapa(mapa, linha, coluna);
	jogo(mapa, linha, coluna, jogador);
	system("pause");
    return 0;
}

/*#pragma region Leonardo
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

#pragma endregion*/
