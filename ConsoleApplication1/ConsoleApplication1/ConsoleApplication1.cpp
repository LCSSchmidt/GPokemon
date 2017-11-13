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
const int QUANTIDADE_OPONENTES = 4;
const int QUANTIDADE_DE_LIDER_DE_GINASIO = 1;
const int QUANTIDADE_DE_MAPAS = 1;
const int QUANTIDADE_DE_JOGADORES = 1;
const int QUANTIDADE_DE_CERCAS = 10;
const int QUANTIDADE_DE_POKEMONS = 12;
const char VAZIO = '\0';
const char CERCA = '#';
const char JOGADOR = 'P';
const char OPONENTE = 'E';
const char LIDER_DE_GINASIO = 'L';
const char CENTRO_POKEMON[2] = { 'C' , 'P' };
const int COR_MAPA = 240;
const int COR_PADRAO = 15;
const int CENTRALIZAR_MAPA = 50;
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
	int linha;
	int coluna;
};

struct Centro_pokemon {
	int linha[2];
	int coluna[2];
};

struct Oponente {
	int linha;
	int coluna;
	int linha_anterior;
	int coluna_anterior;
	int quantidade_de_oponentes = QUANTIDADE_OPONENTES;
};

struct Lider_de_Ginasio {
	int linha;
	int coluna;
};

struct Jogador {
	int x_y_atual[MATRIZ_BIDIMENSIONAL];
	int x_y_anterior[MATRIZ_BIDIMENSIONAL];
};

struct Poder {
	string Nome;
	int Forca;
	int Tipo;
	int PP;
	int PP_MAX;
};

struct Pokemon {
	string Nome;
	int Vida;
	int Vida_MAX;
	int Ataque;
	int Defesa;
	int Velocidade;
	int Tipo;
	int lvl;
	Poder poder[QUANTIDADE_DE_PODER];
};

struct Duelo {
	int quantidade_pokemon_inimigos;
	int quantidade_pokemon_jogador;
	int duelo;
	int turno;
	int localizacao_oponente_quantidade_pokemons;
	int localizacao_jogador_quantidade_pokemons;
	int localizacao_turno;
	int localizacao_opcao_escolhas;
	int localizacao_escolhas[2];
	int pokemons_do_jogador_atual;
	int pokemons_do_oponente_atual;
};

struct Mapa {
	char x_y[MAPA_LINHA][MAPA_COLUNA];
	Centro_pokemon centro_pokemon;
	Cerca cerca[MAPA_LINHA * 2 + MAPA_COLUNA * 2 + QUANTIDADE_DE_CERCAS - 3];
	Jogador jogador[QUANTIDADE_DE_JOGADORES];
	Oponente oponente[QUANTIDADE_OPONENTES];
	Lider_de_Ginasio lider_de_ginasio;
};

void usuario_configuracao_resolucao(int &a, int &b) {
	cout << setw(CENTRALIZAR_MAPA + MAPA_COLUNA) << VAZIO;
	for (int indice = 0; indice < 22; indice++) {
		cout << indice << " ";
	}
	
	cout << endl << "Por favor configure a resolucao antes de jogar." << endl << "Deve aparecer um sequencia de numeros, e para a resolucao estar correta a sequencia deve ir ate o Vinte e um." << endl;
	cout << "Caso a resolucao esteja correta apenas redigite o X" << endl;
	cout << "Resolucao Atual: X =" << a << " por Y =" << b << endl;
	cout << "X: ";
	cin >> a;
}

bool verificar_se_pode_proseguir_com_resolucao() {
	int condicao;

	cout << setw(CENTRALIZAR_MAPA + MAPA_COLUNA) << VAZIO;
	for (int indice = 0; indice < 22; indice++) {
		cout << indice << " ";
	}
	cout << endl << "A resolucao esta correta?" << endl;
	cout << "Digite " << endl <<"1. Sim" << endl << "2. Nao" << endl;
	cout << "Opcao: ";
	cin >> condicao;
	if (condicao == 1) {
		return false;
	}
	else {
		return true;
	}
}

void definir_resolucao() {
	bool prosseguir = true;
	int a = 1000;
	int b = 550;

	HWND console = GetConsoleWindow();
	RECT resolution;
	GetWindowRect(console, &resolution);
	MoveWindow(console, 0, 0, a, b, TRUE);

	do {
		usuario_configuracao_resolucao(a, b);

		HWND console = GetConsoleWindow();
		RECT resolution;
		GetWindowRect(console, &resolution);
		MoveWindow(console, 0, 0, a, b, TRUE);
		prosseguir = verificar_se_pode_proseguir_com_resolucao();
		system("cls");
	} while (prosseguir);
}

void setCursorPosition_posicao_anterior(Mapa &mapa, string elemento, int oponente = 100)
{
	int x;
	int y;
	if (elemento == "Jogador") {
		x = mapa.jogador[0].x_y_anterior[1];
		y = mapa.jogador[0].x_y_anterior[0];
	}
	else {
		x = mapa.oponente[oponente].coluna_anterior;
		y = mapa.oponente[oponente].linha_anterior;
	}

	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COR_MAPA);
	COORD cordenadas_cursor = { (SHORT)x + CENTRALIZAR_MAPA, (SHORT)y };
	SetConsoleCursorPosition(hOut, cordenadas_cursor);
}

void setCursorPosition_posicao_atual(Mapa &mapa, string elemento, int oponente = 100)
{
	int x;
	int y;
	if (elemento == "Jogador") {
		x = mapa.jogador[0].x_y_atual[1];
		y = mapa.jogador[0].x_y_atual[0];
	}
	else if(elemento == "Lider_de_ginasio"){
		x = mapa.lider_de_ginasio.coluna;
		y = mapa.lider_de_ginasio.linha;
	}
	else {
		x = mapa.oponente[oponente].coluna;
		y = mapa.oponente[oponente].linha;
	}
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cordenadas_cursor = { (SHORT)x + CENTRALIZAR_MAPA, (SHORT)y };
	SetConsoleCursorPosition(hOut, cordenadas_cursor);
}

void setCursorPosition_duelo(int adicional, int adicional_a_linha)
{
	int x = 0;
	int y = MAPA_LINHA + adicional_a_linha;

	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cordenadas_cursor = { (SHORT)x + adicional, (SHORT)y };
	SetConsoleCursorPosition(hOut, cordenadas_cursor);
}

bool verificar_condicao_de_gave_over(Duelo duelo) {
	if (duelo.quantidade_pokemon_inimigos == 0) {
		return true;
	}
	else {
		return false;
	}
}

int game_over(Duelo duelo) {
	int escolha;
	bool situacao;

	do {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, COR_PADRAO);
		system("cls");
		if (verificar_condicao_de_gave_over(duelo)) {
		cout << "Parabens, voce derrotou o lider de ginasio e terminou a fase" << endl << "1. Jogar novamente" << endl << "2. Sair";
		}else
		cout << "Infelismente nao foi hoje, voce foi derrotado, mas nao desista, amanha é um novo dia :)" << endl << "1. Jogar novamente" << endl << "2. Sair";
		
		cin >> escolha;
		if (escolha != 1 && escolha != 2) {
			cout << "Opcao invalida, escolha novamente.";
			situacao = true;
		}
		else {
			situacao = false;
		}
	} while (situacao);
	system("cls");

	return escolha;
}

void forca_de_ataque_fraco(int indice, int Indice_do_Poder, Pokemon pokemons[]) {
	pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 11 + 10;
	pokemons[indice].poder[Indice_do_Poder].PP = rand() % 4 + 7;
}

void forca_de_ataque_medio(int indice, int Indice_do_Poder, Pokemon pokemons[]) {
	pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 16 + 20;
	pokemons[indice].poder[Indice_do_Poder].PP = rand() % 4 + 5;
}

void forca_de_ataque_forte(int indice, int Indice_do_Poder, Pokemon pokemons[]) {
	pokemons[indice].poder[Indice_do_Poder].Forca = rand() % 16 + 35;
	pokemons[indice].poder[Indice_do_Poder].PP = rand() % 4 + 3;
}

void gerador_ataque_de_fogo(int indice, int Indice_do_Poder, int Tipos[], Pokemon pokemons[]) {
	pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 3];
	pokemons[indice].poder[Indice_do_Poder].Tipo = Tipos[0];

	if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de fogo fraco")
		forca_de_ataque_fraco(indice, Indice_do_Poder, pokemons);
	else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de fogo medio")
		forca_de_ataque_medio(indice, Indice_do_Poder, pokemons);
	else
		forca_de_ataque_forte(indice, Indice_do_Poder, pokemons);
}

void gerador_ataque_de_agua(int indice, int Indice_do_Poder, int Tipos[], Pokemon pokemons[]) {
	pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 3 + 3];
	pokemons[indice].poder[Indice_do_Poder].Tipo = Tipos[1];

	if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de agua fraco")
		forca_de_ataque_fraco(indice, Indice_do_Poder, pokemons);
	else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de agua medio")
		forca_de_ataque_medio(indice, Indice_do_Poder, pokemons);
	else
		forca_de_ataque_forte(indice, Indice_do_Poder, pokemons);
}

void gerador_ataque_de_grama(int indice, int Indice_do_Poder, int Tipos[], Pokemon pokemons[]) {
	pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 3 + 6];
	pokemons[indice].poder[Indice_do_Poder].Tipo = Tipos[2];

	if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de grama fraco")
		forca_de_ataque_fraco(indice, Indice_do_Poder, pokemons);
	else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de grama medio")
		forca_de_ataque_medio(indice, Indice_do_Poder, pokemons);
	else
		forca_de_ataque_forte(indice, Indice_do_Poder, pokemons);
}

void gerador_ataque_eletrico(int indice, int Indice_do_Poder, int Tipos[], Pokemon pokemons[]) {
	pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 3 + 9];
	pokemons[indice].poder[Indice_do_Poder].Tipo = Tipos[3];

	if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder eletrico fraco")
		forca_de_ataque_fraco(indice, Indice_do_Poder, pokemons);
	else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder eletrico medio")
		forca_de_ataque_medio(indice, Indice_do_Poder, pokemons);
	else
		forca_de_ataque_forte(indice, Indice_do_Poder, pokemons);
}

void gerador_ataque_de_voo(int indice, int Indice_do_Poder, int Tipos[], Pokemon pokemons[]) {
	pokemons[indice].poder[Indice_do_Poder].Nome = Nomes_dos_Poderes[rand() % 3 + 12];
	pokemons[indice].poder[Indice_do_Poder].Tipo = Tipos[4];

	if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de voo fraco")
		forca_de_ataque_fraco(indice, Indice_do_Poder, pokemons);
	else if (pokemons[indice].poder[Indice_do_Poder].Nome == "Poder de voo medio")
		forca_de_ataque_medio(indice, Indice_do_Poder, pokemons);
	else
		forca_de_ataque_forte(indice, Indice_do_Poder, pokemons);
}

void Poderes_do_Pokemon(Pokemon pokemons[], int Tipos[], int indice, int Indice_do_Poder = 0) {
	if (Indice_do_Poder == 4)
		return;
	int Tipo_do_poder = 0;
	Tipo_do_poder = rand() % 3;

	switch (pokemons[indice].Tipo) {
	case TIPO_FOGO:
		if (Tipo_do_poder == 1) {
			gerador_ataque_de_fogo(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else if (Tipo_do_poder == 2) {
			gerador_ataque_eletrico(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else {
			gerador_ataque_de_voo(indice, Indice_do_Poder, Tipos, pokemons);
		}
		break;
	case TIPO_AGUA:
		if (Tipo_do_poder == 1) {
			gerador_ataque_de_agua(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else if (Tipo_do_poder == 2) {
			gerador_ataque_de_grama(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else {
			gerador_ataque_de_voo(indice, Indice_do_Poder, Tipos, pokemons);
		}
		break;
	case TIPO_GRAMA:
		if (Tipo_do_poder == 1) {
			gerador_ataque_de_grama(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else if (Tipo_do_poder == 2) {
			gerador_ataque_de_agua(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else {
			gerador_ataque_eletrico(indice, Indice_do_Poder, Tipos, pokemons);
		}
		break;
	case TIPO_ELETRICO:
		if (Tipo_do_poder == 1) {
			gerador_ataque_eletrico(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else if (Tipo_do_poder == 2) {
			gerador_ataque_de_voo(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else {
			gerador_ataque_de_fogo(indice, Indice_do_Poder, Tipos, pokemons);
		}
		break;
	default:
		if (Tipo_do_poder == 1) {
			gerador_ataque_de_voo(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else if (Tipo_do_poder == 2) {
			gerador_ataque_de_fogo(indice, Indice_do_Poder, Tipos, pokemons);
		}
		else {
			gerador_ataque_eletrico(indice, Indice_do_Poder, Tipos, pokemons);
		}
	}
	pokemons[indice].poder[Indice_do_Poder].PP_MAX = pokemons[indice].poder[Indice_do_Poder].PP;
	Poderes_do_Pokemon(pokemons, Tipos, indice, ++Indice_do_Poder);
}

void Gerador_de_Pokemon(Pokemon pokemons[], int Tipos[], int indice = 0) {
	if (indice == 12)
		return;
	if (indice == 10 && indice == 11) {
		pokemons[indice].lvl = 75;
	}
	else {
		pokemons[indice].lvl = 50;
	}
	pokemons[indice].Vida = rand() % 101 + 100;
	pokemons[indice].Tipo = Tipos[rand() % 5];
	pokemons[indice].Vida_MAX = pokemons[indice].Vida;
	pokemons[indice].Ataque = rand() % 26 + 75;
	pokemons[indice].Defesa = rand() % 26 + 75;
	pokemons[indice].Velocidade = rand() % 26 + 75;

	if (pokemons[indice].Tipo == TIPO_FOGO) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[rand() % 2];
		Poderes_do_Pokemon(pokemons, Tipos, indice);
	}
	else if (pokemons[indice].Tipo == TIPO_AGUA) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[(rand() % 2) + 2];
		Poderes_do_Pokemon(pokemons, Tipos, indice);
	}
	else if (pokemons[indice].Tipo == TIPO_GRAMA) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[(rand() % 2) + 4];
		Poderes_do_Pokemon(pokemons, Tipos, indice);
	}
	else if (pokemons[indice].Tipo == TIPO_ELETRICO) {
		pokemons[indice].Nome = Nomes_dos_Pokemons[(rand() % 2) + 6];
		Poderes_do_Pokemon(pokemons, Tipos, indice);
	}
	else {
		pokemons[indice].Nome = Nomes_dos_Pokemons[(rand() % 2) + 8];
		Poderes_do_Pokemon(pokemons, Tipos, indice);
	}
	Gerador_de_Pokemon(pokemons, Tipos, ++indice);
}

void preencher_mapa(Mapa &mapa, int linha, int coluna) {
	if (linha == MAPA_LINHA - 1 && coluna == MAPA_COLUNA - 1) {
		return;
	}

	mapa.x_y[linha][coluna] = VAZIO;
	if (coluna != MAPA_COLUNA - 1)
		preencher_mapa(mapa, linha, ++coluna);
	else
		preencher_mapa(mapa, ++linha, 0);
}

void cerca_padrao(Mapa &mapa, int auxiliar, int chamadas, int linha = 0, int coluna = 0) {

	if (chamadas == MAPA_LINHA * 2 + MAPA_COLUNA * 2 - 3) {
		return;
	}

	mapa.x_y[linha][coluna] = CERCA;
	mapa.cerca[chamadas].linha = linha;
	mapa.cerca[chamadas].coluna = coluna;

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

void desenhar_mapa(Mapa &mapa, int linha, int coluna) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (linha == MAPA_LINHA - 1 && coluna == MAPA_COLUNA - 1) {
		cout << mapa.x_y[linha][coluna];
		SetConsoleTextAttribute(hConsole, COR_PADRAO);
		return;
	}
	if (coluna == 0) {
		SetConsoleTextAttribute(hConsole, COR_PADRAO);
		cout << setw(CENTRALIZAR_MAPA) << VAZIO;
		SetConsoleTextAttribute(hConsole, COR_MAPA);
		cout << mapa.x_y[linha][coluna];
	}
	else
		cout << mapa.x_y[linha][coluna];

	if (coluna != MAPA_COLUNA - 1)
		desenhar_mapa(mapa, linha, ++coluna);
	else {
		cout << endl;
		desenhar_mapa(mapa, ++linha, 0);
	}
}

void posicao_do_obstaculo(Mapa &mapa, int cordenadas[]) {

	cordenadas[0] = 1 + rand() % MAPA_LINHA - 1;
	cordenadas[1] = 1 + rand() % MAPA_COLUNA - 1;
	if (mapa.x_y[cordenadas[0]][cordenadas[1]] != VAZIO || cordenadas[0] == MAPA_LINHA / 2 && cordenadas[1] + 1 == MAPA_COLUNA / 2) {
		posicao_do_obstaculo(mapa, cordenadas);
	}
}

void criar_obstaculo(Mapa &mapa, int cordenadas[], int quantidade) {
	mapa.x_y[cordenadas[0]][cordenadas[1]] = CERCA;
	mapa.cerca[MAPA_LINHA * 2 + MAPA_COLUNA * 2 - 3 + quantidade].linha = cordenadas[0];
	mapa.cerca[MAPA_LINHA * 2 + MAPA_COLUNA * 2 - 3 + quantidade].coluna = cordenadas[1];
}

void obstaculo(Mapa &mapa, int cordenadas[], int quantidade_de_cercas = -1) {
	if (quantidade_de_cercas == QUANTIDADE_DE_CERCAS - 1) {
		return;
	}

	posicao_do_obstaculo(mapa, cordenadas);
	criar_obstaculo(mapa, cordenadas, ++quantidade_de_cercas);
	obstaculo(mapa, cordenadas, quantidade_de_cercas);
}

void atribuir_posicao_jogador(Mapa &mapa_jogador) {
	mapa_jogador.jogador[0].x_y_atual[0] = MAPA_LINHA / 2;
	mapa_jogador.jogador[0].x_y_atual[1] = MAPA_COLUNA / 2;
	mapa_jogador.jogador[0].x_y_anterior[0] = MAPA_LINHA / 2;
	mapa_jogador.jogador[0].x_y_anterior[1] = MAPA_COLUNA / 2;
}

void posicionar_jogador(Mapa &mapa) {
	mapa.x_y[mapa.jogador[0].x_y_atual[0]][mapa.jogador[0].x_y_atual[1]] = JOGADOR;
}

void posicionar_oponentes(Mapa &mapa, int auxiliar = 0) {
	if (auxiliar == QUANTIDADE_OPONENTES) {
		return;
	}
	int linha = 1 + rand() % (MAPA_LINHA - 1);
	int coluna = 1 + rand() % (MAPA_COLUNA - 1);

	if (mapa.x_y[linha][coluna] == VAZIO) {
		mapa.x_y[linha][coluna] = OPONENTE;
		mapa.oponente[auxiliar].linha = linha;
		mapa.oponente[auxiliar].coluna = coluna;
		posicionar_oponentes(mapa, ++auxiliar);
	}
	else
		posicionar_oponentes(mapa, auxiliar);
}

void posicionar_lider(Mapa &mapa) {
	int linha = 1 + rand() % (MAPA_LINHA - 1);
	int coluna = 1 + rand() % (MAPA_COLUNA - 1);

	if (mapa.x_y[linha][coluna] == VAZIO) {
		mapa.lider_de_ginasio.linha = linha;
		mapa.lider_de_ginasio.coluna = coluna;
	}
	else
		posicionar_lider(mapa);
}

void posicionar_centro_pokemon(Mapa &mapa) {
	int linha = 1 + rand() % MAPA_LINHA - 1;
	int coluna = 1 + rand() % MAPA_COLUNA - 1;

	if (mapa.x_y[linha][coluna] == VAZIO && mapa.x_y[linha][coluna + 1] == VAZIO) {
		mapa.centro_pokemon.linha[0] = linha;
		mapa.centro_pokemon.coluna[0] = coluna;
		mapa.x_y[linha][coluna] = CENTRO_POKEMON[0];
		mapa.centro_pokemon.linha[1] = linha;
		mapa.centro_pokemon.coluna[1] = coluna + 1;
		mapa.x_y[linha][coluna + 1] = CENTRO_POKEMON[1];
	}
	else
		posicionar_centro_pokemon(mapa);
}

void posicionar_elementos(Mapa &mapa) {
	posicionar_jogador(mapa);
	posicionar_oponentes(mapa);
	posicionar_centro_pokemon(mapa);
}

void apagar_oponente(Mapa &mapa, int oponente) {
	int linha_anterior = mapa.oponente[oponente].linha_anterior;
	int coluna_anterior = mapa.oponente[oponente].coluna_anterior;

	mapa.x_y[linha_anterior][coluna_anterior] = VAZIO;
	setCursorPosition_posicao_anterior(mapa, "Oponente", oponente);
	cout << mapa.x_y[linha_anterior][coluna_anterior];
}

void restaurar_pokemons(Pokemon pokemons[]) {
	for (int indice = 0; indice < 2; indice++) {
		pokemons[indice].Vida = pokemons[indice].Vida_MAX;
		for (int indice_auxiliar = 0; indice_auxiliar < 4; indice_auxiliar++) {
			pokemons[indice].poder[indice_auxiliar].PP = pokemons[indice].poder[indice_auxiliar].PP_MAX;
		}
	}
	setCursorPosition_duelo(24, 4);
	cout << "Pokemons restaurados, pressione algum botao para para continuar";
	_getch();
	setCursorPosition_duelo(24, 4);
	cout << setw << VAZIO;
}

bool acao_do_jogador(Mapa &mapa, Pokemon pokemons[]) {
	int linha = mapa.jogador[0].x_y_atual[0];
	int coluna = mapa.jogador[0].x_y_atual[1];

	switch (_getch()) {
	case 'w':
		if (mapa.x_y[linha - 1][coluna] == VAZIO) {
			mapa.jogador[0].x_y_anterior[0] = linha;
			mapa.jogador[0].x_y_anterior[1] = mapa.jogador[0].x_y_atual[1];
			mapa.jogador[0].x_y_atual[0]--;
			return true;
		}
		break;
	case 's':
		if (mapa.x_y[linha + 1][coluna] == VAZIO) {
			mapa.jogador[0].x_y_anterior[0] = linha;
			mapa.jogador[0].x_y_anterior[1] = coluna;
			mapa.jogador[0].x_y_atual[0]++;
			return true;
		}
		break;
	case 'a':
		if (mapa.x_y[linha][coluna - 1] == VAZIO) {
			mapa.jogador[0].x_y_anterior[0] = linha;
			mapa.jogador[0].x_y_anterior[1] = coluna;
			mapa.jogador[0].x_y_atual[1]--;
			return true;
		}
		break;
	case 'd':
		if (mapa.x_y[linha][coluna + 1] == VAZIO) {
			mapa.jogador[0].x_y_anterior[0] = linha;
			mapa.jogador[0].x_y_anterior[1] = coluna;
			mapa.jogador[0].x_y_atual[1]++;
			return true;
		}
		break;
	case 'j':
		if (mapa.x_y[linha][coluna + 1] == CENTRO_POKEMON[0] ||
			mapa.x_y[linha + 1][coluna] == CENTRO_POKEMON[0] ||
			mapa.x_y[linha - 1][coluna] == CENTRO_POKEMON[0] ||
			mapa.x_y[linha][coluna - 1] == CENTRO_POKEMON[1] ||
			mapa.x_y[linha + 1][coluna] == CENTRO_POKEMON[1] ||
			mapa.x_y[linha - 1][coluna] == CENTRO_POKEMON[1]) {
			restaurar_pokemons(pokemons);
			return true;
		}
		else {
			return false;
		}
			break;
	}
	return false;
}

void movimentar_jogador(Mapa &mapa) {
	int linha_anterior = mapa.jogador[0].x_y_anterior[0];
	int coluna_anterior = mapa.jogador[0].x_y_anterior[1];
	int linha_atual = mapa.jogador[0].x_y_atual[0];
	int coluna_atual = mapa.jogador[0].x_y_atual[1];

	mapa.x_y[linha_anterior][coluna_anterior] = VAZIO;
	mapa.x_y[linha_atual][coluna_atual] = JOGADOR;
}

void movimentar_oponentes(Mapa &mapa, int oponente = 0) {
	int linha_anterior = mapa.oponente[oponente].linha_anterior;
	int coluna_anterior = mapa.oponente[oponente].coluna_anterior;
	int linha_atual = mapa.oponente[oponente].linha;
	int coluna_atual = mapa.oponente[oponente].coluna;

	mapa.x_y[linha_anterior][coluna_anterior] = VAZIO;
	mapa.x_y[linha_atual][coluna_atual] = OPONENTE;
}

void busca_de_jogador(Mapa &mapa, int oponente = 0) {
	if (oponente == QUANTIDADE_OPONENTES) {
		return;
	}

	if (mapa.oponente[oponente].linha != 0 && mapa.oponente[oponente].coluna != 0) {
		int linha = mapa.oponente[oponente].linha;
		int coluna = mapa.oponente[oponente].coluna;

		if (mapa.oponente[oponente].linha < mapa.jogador[0].x_y_atual[0] && mapa.x_y[linha + 1][coluna] == VAZIO) {
			mapa.oponente[oponente].linha_anterior = mapa.oponente[oponente].linha;
			mapa.oponente[oponente].coluna_anterior = mapa.oponente[oponente].coluna;
			mapa.oponente[oponente].linha++;
		}
		else if (mapa.oponente[oponente].coluna < mapa.jogador[0].x_y_atual[1] && mapa.x_y[linha][coluna + 1] == VAZIO) {
			mapa.oponente[oponente].linha_anterior = mapa.oponente[oponente].linha;
			mapa.oponente[oponente].coluna_anterior = mapa.oponente[oponente].coluna;
			mapa.oponente[oponente].coluna++;
		}
		else if (mapa.oponente[oponente].linha > mapa.jogador[0].x_y_atual[0] && mapa.x_y[linha - 1][coluna] == VAZIO) {
			mapa.oponente[oponente].linha_anterior = mapa.oponente[oponente].linha;
			mapa.oponente[oponente].coluna_anterior = mapa.oponente[oponente].coluna;
			mapa.oponente[oponente].linha--;
		}
		else if (mapa.oponente[oponente].coluna > mapa.jogador[0].x_y_atual[1] && mapa.x_y[linha][coluna - 1] == VAZIO) {
			mapa.oponente[oponente].linha_anterior = mapa.oponente[oponente].linha;
			mapa.oponente[oponente].coluna_anterior = mapa.oponente[oponente].coluna;
			mapa.oponente[oponente].coluna--;
		}
		movimentar_oponentes(mapa, oponente);	
	}
	busca_de_jogador(mapa, ++oponente);
}

void atribuir_valores_a_batalha(Duelo &duelo) {
	duelo.duelo = 1;
	duelo.quantidade_pokemon_inimigos = 2;
	duelo.quantidade_pokemon_jogador = 2;
	duelo.turno = 1;
	duelo.localizacao_oponente_quantidade_pokemons = 65;
	duelo.localizacao_jogador_quantidade_pokemons = 22;
	duelo.localizacao_turno = 54;
	duelo.localizacao_opcao_escolhas = 37;
	duelo.localizacao_escolhas[0] = duelo.localizacao_jogador_quantidade_pokemons;
	duelo.localizacao_escolhas[1] = duelo.localizacao_oponente_quantidade_pokemons;
	duelo.pokemons_do_oponente_atual = 0;
	duelo.pokemons_do_jogador_atual = 0;
}

void resetar_informacoes_do_oponente(Duelo &duelo) {
	duelo.quantidade_pokemon_inimigos = 2;
	duelo.pokemons_do_oponente_atual = 0;
}

void informacoes_da_batalha(Pokemon pokemons[], Duelo duelo, int pokemons_do_oponente[]) {
	int indice_pokemons_oponente = pokemons_do_oponente[duelo.pokemons_do_oponente_atual];
	int indice_pokemons_jogador = duelo.pokemons_do_jogador_atual;

	setCursorPosition_duelo(duelo.localizacao_oponente_quantidade_pokemons, 4);
	cout << "Oponente possui " << duelo.quantidade_pokemon_inimigos << " Monstro(os)";

	setCursorPosition_duelo(duelo.localizacao_turno, 4);
	cout << "Turno " << duelo.turno;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 4);
	cout << "Jogador possui " << duelo.quantidade_pokemon_jogador << " Monstro(os)";

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 5);
	cout << pokemons[indice_pokemons_jogador].Nome << " (LV " << pokemons[indice_pokemons_jogador].lvl << ") - " << pokemons[indice_pokemons_jogador].Vida << "/" << pokemons[indice_pokemons_jogador].Vida_MAX;

	setCursorPosition_duelo(duelo.localizacao_oponente_quantidade_pokemons, 5);
	cout << pokemons[indice_pokemons_oponente].Nome << " (LV " << pokemons[indice_pokemons_oponente].lvl << ") - " << pokemons[indice_pokemons_oponente].Vida << "/" << pokemons[indice_pokemons_oponente].Vida_MAX;	;
}

void mostrar_escolhas_da_batalha(Pokemon pokemons[], Duelo duelo){

	setCursorPosition_duelo(duelo.localizacao_opcao_escolhas, 6);
	cout << "Qual ataque seu " << pokemons[duelo.pokemons_do_jogador_atual].Nome << " deve utilizar?";

	setCursorPosition_duelo(duelo.localizacao_escolhas[0], 7);
	cout << "1. " << pokemons[duelo.pokemons_do_jogador_atual].poder[0].Nome << " (PP/: " << pokemons[duelo.pokemons_do_jogador_atual].poder[0].PP << "/ " << pokemons[duelo.pokemons_do_jogador_atual].poder[0].PP_MAX << ") ";

	setCursorPosition_duelo( duelo.localizacao_escolhas[0], 8);
	cout << "2. " << pokemons[duelo.pokemons_do_jogador_atual].poder[1].Nome << " (PP/: " << pokemons[duelo.pokemons_do_jogador_atual].poder[1].PP << "/ " << pokemons[duelo.pokemons_do_jogador_atual].poder[1].PP_MAX << ") ";

	setCursorPosition_duelo(duelo.localizacao_escolhas[1], 7);
	cout << "3. " << pokemons[duelo.pokemons_do_jogador_atual].poder[2].Nome << " (PP/: " << pokemons[duelo.pokemons_do_jogador_atual].poder[2].PP << "/ " << pokemons[duelo.pokemons_do_jogador_atual].poder[2].PP_MAX << ") ";

	setCursorPosition_duelo(duelo.localizacao_escolhas[1], 8);
	cout << "4." << pokemons[duelo.pokemons_do_jogador_atual].poder[3].Nome << " (PP/: " << pokemons[duelo.pokemons_do_jogador_atual].poder[3].PP << "/" << pokemons[duelo.pokemons_do_jogador_atual].poder[3].PP_MAX << ") ";
}

void mostrar_detalhes_da_batalha(Pokemon pokemons[], Duelo duelo, int pokemons_do_oponente[], int ataque_escolhido, int ataque_aleatorio) {
	int indice_pokemons_jogador = duelo.pokemons_do_jogador_atual;
	int indice_pokemons_oponente = pokemons_do_oponente[duelo.pokemons_do_oponente_atual];

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 10);
	cout << "Seu ataque deu: " << pokemons[indice_pokemons_jogador].poder[ataque_escolhido - 1].Forca << " de dano!" << endl;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 11);
	cout << "Seu adversario usou o ataque: " << pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Nome << endl;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 12);
	cout << "O ataque de seu adversario deu: " << pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Forca << " de dano!";
}

bool verificar_desvantagem_de_ataque_do_jogador(Pokemon pokemons[], int indice_pokemons_jogador, int ataque_escolhido, int indice_pokemons_oponente) {
	if ((pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_FOGO && pokemons[indice_pokemons_oponente].Tipo == TIPO_AGUA) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_FOGO && pokemons[indice_pokemons_oponente].Tipo == TIPO_FOGO) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_AGUA && pokemons[indice_pokemons_oponente].Tipo == TIPO_AGUA) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_AGUA && pokemons[indice_pokemons_oponente].Tipo == TIPO_GRAMA) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_GRAMA && pokemons[indice_pokemons_oponente].Tipo == TIPO_FOGO) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_GRAMA && pokemons[indice_pokemons_oponente].Tipo == TIPO_GRAMA) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_GRAMA && pokemons[indice_pokemons_oponente].Tipo == TIPO_VOADOR) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_ELETRICO && pokemons[indice_pokemons_oponente].Tipo == TIPO_ELETRICO) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_ELETRICO && pokemons[indice_pokemons_oponente].Tipo == TIPO_GRAMA) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_VOADOR && pokemons[indice_pokemons_oponente].Tipo == TIPO_ELETRICO)) {

		cout << "Ataque pouco efetivo";
		return true;
	}
	else
		return false;
}

bool verificar_vantagem_de_ataque_do_jogador(Pokemon pokemons[], int indice_pokemons_jogador, int ataque_escolhido, int indice_pokemons_oponente) {
	if ((pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_FOGO && pokemons[indice_pokemons_oponente].Tipo == TIPO_GRAMA) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_AGUA && pokemons[indice_pokemons_oponente].Tipo == TIPO_FOGO) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_GRAMA && pokemons[indice_pokemons_oponente].Tipo == TIPO_AGUA) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_ELETRICO && pokemons[indice_pokemons_oponente].Tipo == TIPO_AGUA) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_ELETRICO && pokemons[indice_pokemons_oponente].Tipo == TIPO_VOADOR) ||
		(pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Tipo == TIPO_VOADOR && pokemons[indice_pokemons_oponente].Tipo == TIPO_GRAMA)) {

		cout << "Ataque super efetivo";
		return true;
	}
	else
		return false;
}

bool verificar_desvantagem_de_ataque_do_oponente(Pokemon pokemons[], Duelo duelo, int indice_pokemons_jogador, int ataque_aleatorio, int indice_pokemons_oponente) {
	if ((pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_FOGO && pokemons[indice_pokemons_jogador].Tipo == TIPO_AGUA) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_FOGO && pokemons[indice_pokemons_jogador].Tipo == TIPO_FOGO) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_AGUA && pokemons[indice_pokemons_jogador].Tipo == TIPO_AGUA) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_AGUA && pokemons[indice_pokemons_jogador].Tipo == TIPO_GRAMA) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_GRAMA && pokemons[indice_pokemons_jogador].Tipo == TIPO_FOGO) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_GRAMA && pokemons[indice_pokemons_jogador].Tipo == TIPO_GRAMA) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_GRAMA && pokemons[indice_pokemons_jogador].Tipo == TIPO_VOADOR) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_ELETRICO && pokemons[indice_pokemons_jogador].Tipo == TIPO_ELETRICO) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_ELETRICO && pokemons[indice_pokemons_jogador].Tipo == TIPO_GRAMA) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_VOADOR && pokemons[indice_pokemons_jogador].Tipo == TIPO_ELETRICO)) {
		setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 13);
		cout << "Ataque pouco efetivo";
		return true;
	}
	else
		return false;
}

bool verificar_vantagem_de_ataque_do_oponente(Pokemon pokemons[], Duelo duelo, int indice_pokemons_jogador, int ataque_aleatorio, int indice_pokemons_oponente) {
	if ((pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_FOGO && pokemons[indice_pokemons_jogador].Tipo == TIPO_GRAMA) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_AGUA && pokemons[indice_pokemons_jogador].Tipo == TIPO_FOGO) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_GRAMA && pokemons[indice_pokemons_jogador].Tipo == TIPO_AGUA) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_ELETRICO && pokemons[indice_pokemons_jogador].Tipo == TIPO_AGUA) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_ELETRICO && pokemons[indice_pokemons_jogador].Tipo == TIPO_VOADOR) ||
		(pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Tipo == TIPO_VOADOR && pokemons[indice_pokemons_jogador].Tipo == TIPO_GRAMA)) {
		setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 13);
		cout << "Ataque super efetivo";
		return true;
	}
	else
		return false;
}

void ataque_do_oponente(Pokemon pokemons[], Duelo duelo, int pokemons_do_oponente[], int &ataque_aleatorio) {
	int indice_pokemons_oponente = pokemons_do_oponente[duelo.pokemons_do_oponente_atual];
	int indice_pokemons_jogador = duelo.pokemons_do_jogador_atual;
	int dano_aleatorio = rand() % 16;
	int dano_final = 0;
	ataque_aleatorio = rand() % 4;

	dano_final = dano_aleatorio + pokemons[indice_pokemons_oponente].Ataque + pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Forca - pokemons[indice_pokemons_jogador].Defesa;

	if (verificar_vantagem_de_ataque_do_oponente(pokemons, duelo, indice_pokemons_jogador, ataque_aleatorio, indice_pokemons_oponente))
		dano_final = dano_final * 1.5;
	if (verificar_desvantagem_de_ataque_do_oponente(pokemons, duelo, indice_pokemons_jogador, ataque_aleatorio, indice_pokemons_oponente))
		dano_final = dano_final *  0.5;
	if (dano_final < 0)
		dano_final = 0;

	pokemons[indice_pokemons_jogador].Vida -= dano_final;
	pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].PP--;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 11);
	cout << "Seu adversario usou o ataque: " << pokemons[indice_pokemons_oponente].poder[ataque_aleatorio].Nome << endl;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 12);
	cout << "O ataque de seu adversario deu: " << dano_final << " de dano!";

	return;
}

void ataque_do_jogador(Pokemon pokemons[], Duelo duelo, int pokemons_do_oponente[], int ataque_escolhido) {
	int indice_pokemons_oponente = pokemons_do_oponente[duelo.pokemons_do_oponente_atual];
	int indice_pokemons_jogador = duelo.pokemons_do_jogador_atual;
	int dano_aleatorio = rand() % 16;
	int dano_final = 0;

	pokemons[indice_pokemons_oponente].Vida -= pokemons[indice_pokemons_jogador].poder[ataque_escolhido - 1].Forca;
	pokemons[indice_pokemons_jogador].poder[ataque_escolhido - 1].PP--;

	dano_final = dano_aleatorio + pokemons[indice_pokemons_jogador].Ataque + pokemons[indice_pokemons_jogador].poder[ataque_escolhido].Forca - pokemons[indice_pokemons_oponente].Defesa;

	if (verificar_vantagem_de_ataque_do_jogador(pokemons, indice_pokemons_jogador, ataque_escolhido, indice_pokemons_oponente))
		dano_final = dano_final * 1.5;
	if (verificar_desvantagem_de_ataque_do_jogador(pokemons, indice_pokemons_jogador, ataque_escolhido, indice_pokemons_oponente))
		dano_final = dano_final *  0.5;
	if (dano_final < 0)
		dano_final = 0;

	pokemons[indice_pokemons_oponente].Vida -= dano_final;
	pokemons[indice_pokemons_jogador].poder[ataque_escolhido - 1].PP--;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 10);
	cout << "Seu ataque deu: " << dano_final << " de dano!" << endl;

	return;
}

bool verificar_se_a_pp(Pokemon pokemons[], Duelo duelo, int ataque_escolhido) {
	if (pokemons[duelo.pokemons_do_jogador_atual].poder[ataque_escolhido - 1].PP <= 0) {
		return true;
	}
	else {
		return false;
	}
}

void converter_ataque_escolhido(int &ataque_escolhido) {
	if (ataque_escolhido == 49) {
		ataque_escolhido = 1;
	}
	else if (ataque_escolhido == 50) {
		ataque_escolhido = 2;
	}
	else if (ataque_escolhido == 51) {
		ataque_escolhido = 3;
	}
	else if (ataque_escolhido == 52) {
		ataque_escolhido = 4;
	}
}

int escolhas_do_jogador(Pokemon pokemons[], Duelo duelo, int ataque_escolhido) {
	ataque_escolhido = _getch();

	if (ataque_escolhido != 49 && ataque_escolhido != 50 && ataque_escolhido != 51 && ataque_escolhido != 52) {
		setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 9);
		cout << "Escolha invalida!";
		ataque_escolhido = escolhas_do_jogador(pokemons, duelo, ataque_escolhido);
	}
	else{
		converter_ataque_escolhido(ataque_escolhido);

		if (verificar_se_a_pp(pokemons, duelo, ataque_escolhido)) {

			setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 13);
			cout << "PP insuficiente, escolha outro habilidade.";
			ataque_escolhido = escolhas_do_jogador(pokemons, duelo, ataque_escolhido);

			setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 13);
			cout << setw(100) << VAZIO;
		}

	}
	return ataque_escolhido;
}

void checar_vida_do_pokemon_do_jogador(Pokemon pokemons[], Duelo &duelo) {

	if (pokemons[duelo.pokemons_do_jogador_atual].Vida <= 0){
		duelo.pokemons_do_jogador_atual++;
		duelo.quantidade_pokemon_jogador--;
	}
}

void checar_vida_do_pokemon_do_oponente(Pokemon pokemons[], Duelo &duelo, int pokemons_do_oponente[]) {
	if (pokemons[pokemons_do_oponente[duelo.pokemons_do_oponente_atual]].Vida <= 0){
		duelo.pokemons_do_oponente_atual++;
		duelo.quantidade_pokemon_inimigos--;
	}
}

void limpar_informacoes_da_batalha(Duelo duelo) {
	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 4);
	cout << setw(100) << VAZIO;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 5);
	cout << setw(100) << VAZIO;
}

void limpar_detalhes_da_batalha(Duelo duelo){
	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 6);
	cout << setw(100) << VAZIO;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 7);
	cout << setw(100) << VAZIO;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 8);
	cout << setw(100) << VAZIO;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 9);
	cout << setw(100) << VAZIO;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 10);
	cout << setw(100) << VAZIO;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 11);
	cout << setw(100) << VAZIO;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 12);
	cout << setw(100) << VAZIO;

	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 13);
	cout << setw(100) << VAZIO;
}

void informacoes_apos_batalha(Duelo duelo) {
	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 4);
	if (duelo.quantidade_pokemon_jogador == 0) {
		cout << "Perdeu preiba";
	}
	else {
		cout << "Oponente Derrotado";
	}
	_getch();
	setCursorPosition_duelo(duelo.localizacao_jogador_quantidade_pokemons, 4);
	cout << setw(100) << VAZIO;
}

bool batalha(Pokemon pokemons[], Duelo &duelo, int pokemons_do_oponente[], int &quantidade_de_oponentes) {
	int ataque_escolhido = 0;
	int ataque_aleatorio = 0;

	if (duelo.quantidade_pokemon_jogador == 0) {
		informacoes_apos_batalha(duelo);
		return false;
	}
	if (duelo.quantidade_pokemon_inimigos == 0) {

		if(pokemons_do_oponente[0] != 10)
			resetar_informacoes_do_oponente(duelo);

		if (pokemons_do_oponente[0] != 10) {
			quantidade_de_oponentes--;
		}
		duelo.duelo++;
		informacoes_apos_batalha(duelo);
		return true;
	}
	
	mostrar_escolhas_da_batalha(pokemons, duelo);
	ataque_escolhido = escolhas_do_jogador(pokemons, duelo, ataque_escolhido);

	if (pokemons[duelo.pokemons_do_jogador_atual].Velocidade > pokemons[duelo.pokemons_do_oponente_atual].Velocidade) {
		ataque_do_jogador(pokemons, duelo, pokemons_do_oponente, ataque_escolhido);
		checar_vida_do_pokemon_do_oponente(pokemons, duelo, pokemons_do_oponente);
		ataque_do_oponente(pokemons, duelo, pokemons_do_oponente, ataque_aleatorio);
		checar_vida_do_pokemon_do_jogador(pokemons, duelo);
	}
	else {
		ataque_do_oponente(pokemons, duelo, pokemons_do_oponente, ataque_aleatorio);
		checar_vida_do_pokemon_do_jogador(pokemons, duelo);
		ataque_do_jogador(pokemons, duelo, pokemons_do_oponente, ataque_escolhido);
		checar_vida_do_pokemon_do_oponente(pokemons, duelo, pokemons_do_oponente);
	}

	limpar_informacoes_da_batalha(duelo);

	if (duelo.quantidade_pokemon_inimigos != 0 && duelo.quantidade_pokemon_jogador != 0) {
		duelo.turno++;
		informacoes_da_batalha(pokemons, duelo, pokemons_do_oponente);
		_getch();
	}
	limpar_detalhes_da_batalha(duelo);
	batalha(pokemons, duelo, pokemons_do_oponente, quantidade_de_oponentes);
}

bool batalha_screen(Pokemon pokemons[], Duelo &duelo, int &quantidade_de_oponentes) {
	int pokemons_do_oponente[2];

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COR_PADRAO);

	if (duelo.duelo == 1) {
		pokemons_do_oponente[0] = 2;
		pokemons_do_oponente[1] = 3;
	}
	else if (duelo.duelo == 2) {
		pokemons_do_oponente[0] = 4;
		pokemons_do_oponente[1] = 5;
	}
	else if (duelo.duelo == 3) {
		pokemons_do_oponente[0] = 6;
		pokemons_do_oponente[1] = 7;
	}
	else if (duelo.duelo == 4) {
		pokemons_do_oponente[0] = 8;
		pokemons_do_oponente[1] = 9;
	}
	else if (duelo.duelo == 5) {
		pokemons_do_oponente[0] = 10;
		pokemons_do_oponente[1] = 11;
	}

	informacoes_da_batalha(pokemons, duelo, pokemons_do_oponente);
	return batalha(pokemons, duelo, pokemons_do_oponente, quantidade_de_oponentes);
}

bool iniciar_batalha(Pokemon pokemons[], Duelo &duelo, int &quantidade_de_oponentes) {
	return batalha_screen(pokemons, duelo, quantidade_de_oponentes);
}

void atualizar_mapa(Mapa &mapa) {
	int linha_anterior_jogador = mapa.jogador[0].x_y_anterior[0];
	int coluna_anterior_jogador = mapa.jogador[0].x_y_anterior[1];
	int linha_atual_jogador = mapa.jogador[0].x_y_atual[0];
	int coluna_atual_jogador = mapa.jogador[0].x_y_atual[1];

	setCursorPosition_posicao_anterior(mapa, "Jogador");
	cout << mapa.x_y[linha_anterior_jogador][coluna_anterior_jogador];

	setCursorPosition_posicao_atual(mapa, "Jogador");
	cout << mapa.x_y[linha_atual_jogador][coluna_atual_jogador];

	for (int indice = 0; indice < QUANTIDADE_OPONENTES; indice++) {
		int linha_anterior = mapa.oponente[indice].linha_anterior;
		int coluna_anterior = mapa.oponente[indice].coluna_anterior;
		int linha = mapa.oponente[indice].linha;
		int coluna = mapa.oponente[indice].coluna;

		if (mapa.oponente[indice].linha != 0 && mapa.oponente[indice].coluna != 0) {

			setCursorPosition_posicao_anterior(mapa, "Oponente", indice);
			cout << mapa.x_y[linha_anterior][coluna_anterior];
			setCursorPosition_posicao_atual(mapa, "Oponente", indice);
			cout << mapa.x_y[linha][coluna];
		}
	}
}

void atualizar_mapa_com_lider(Mapa &mapa) {
	int linha = mapa.lider_de_ginasio.linha;
	int coluna = mapa.lider_de_ginasio.coluna;

	setCursorPosition_posicao_atual(mapa, "Lider_de_ginasio");
	mapa.x_y[linha][coluna] = LIDER_DE_GINASIO;
	cout << mapa.x_y[linha][coluna];
}

bool verificar_se_a_duelo(Mapa &mapa, Pokemon pokemons[], Duelo &duelo, int &quantidade_de_oponentes, int oponente = 0) {
	bool oponente_derrotado = false;
	
	if(quantidade_de_oponentes != 0){
		if (oponente == QUANTIDADE_OPONENTES)
			return false;

		if (mapa.oponente[oponente].linha == (mapa.jogador[0].x_y_atual[0] - 1) && mapa.oponente[oponente].coluna == mapa.jogador[0].x_y_atual[1] && 
			mapa.oponente[oponente].linha != 0) {
			oponente_derrotado = iniciar_batalha(pokemons, duelo, quantidade_de_oponentes);
			if (!oponente_derrotado) {
				return true;
			}
				
		}
		else if (mapa.oponente[oponente].linha == mapa.jogador[0].x_y_atual[0] && mapa.oponente[oponente].coluna == (mapa.jogador[0].x_y_atual[1] + 1) &&
			mapa.oponente[oponente].linha != 0) {
			oponente_derrotado = iniciar_batalha(pokemons, duelo, quantidade_de_oponentes);
			if (!oponente_derrotado) {
				return true;
			} 	
		}
		else if (mapa.oponente[oponente].linha == (mapa.jogador[0].x_y_atual[0] + 1) && mapa.oponente[oponente].coluna == mapa.jogador[0].x_y_atual[1]
			&& mapa.oponente[oponente].linha != 0) {
			oponente_derrotado = iniciar_batalha(pokemons, duelo, quantidade_de_oponentes);
			if (!oponente_derrotado) {
				return true;
			}
		}
		else if (mapa.oponente[oponente].linha == mapa.jogador[0].x_y_atual[0] && mapa.oponente[oponente].coluna == (mapa.jogador[0].x_y_atual[1] - 1)
			&& mapa.oponente[oponente].linha != 0) {
			oponente_derrotado = iniciar_batalha(pokemons, duelo, quantidade_de_oponentes);
			if (!oponente_derrotado) {
				return true;
			}
		}
		if (oponente_derrotado) {
			mapa.oponente[oponente].linha_anterior = mapa.oponente[oponente].linha;
			mapa.oponente[oponente].coluna_anterior = mapa.oponente[oponente].coluna;
			mapa.oponente[oponente].linha = 0;
			mapa.oponente[oponente].coluna = 0;
			apagar_oponente(mapa, oponente);
		}
	}
	else {
		int linha = mapa.jogador[0].x_y_atual[0];
		int coluna = mapa.jogador[0].x_y_atual[1];

		if (mapa.x_y[linha][coluna - 1] == LIDER_DE_GINASIO || mapa.x_y[linha + 1][coluna] == LIDER_DE_GINASIO ||
			mapa.x_y[linha][coluna + 1] == LIDER_DE_GINASIO || mapa.x_y[linha - 1][coluna] == LIDER_DE_GINASIO) {

			oponente_derrotado = iniciar_batalha(pokemons, duelo, quantidade_de_oponentes);
			if (oponente_derrotado) {
				mapa.oponente[oponente].linha_anterior = mapa.oponente[oponente].linha;
				mapa.oponente[oponente].coluna_anterior = mapa.oponente[oponente].coluna;
				mapa.oponente[oponente].linha = 0;
				mapa.oponente[oponente].coluna = 0;
				apagar_oponente(mapa, oponente);
				return true;
			}
		}
		return false;
	}
	verificar_se_a_duelo(mapa, pokemons, duelo, quantidade_de_oponentes, ++oponente);
}

void jogo(Mapa &mapa, Pokemon pokemons[], Duelo &duelo, int linha, int coluna) {
	bool game_over = false;
	int quantidade_de_oponentes = QUANTIDADE_OPONENTES;
	
	do {
		if (quantidade_de_oponentes == 0) {
			atualizar_mapa_com_lider(mapa);
		}
		if (acao_do_jogador(mapa, pokemons)) {
			movimentar_jogador(mapa);
			busca_de_jogador(mapa);
			atualizar_mapa(mapa);
			game_over = verificar_se_a_duelo(mapa, pokemons, duelo, quantidade_de_oponentes, game_over);
		}
	} while (!game_over);
}

void reinicializar_variaveis(int &chamadas, int &quantidade_de_oponentes, int &auxiliar, int &linha, int &coluna) {
	chamadas = 0;
	quantidade_de_oponentes = 4;
	auxiliar = MAPA_LINHA - 1;
	linha = 0;
	coluna = 0;
	quantidade_de_oponentes = QUANTIDADE_OPONENTES;
}

int main() {
	Mapa mapa;
	Pokemon pokemons[12];
	Duelo duelo;
	int cordenadas_spawn_obstaculos[2] = { 0 };
	int linha = 0;
	int coluna = 0;
	int auxiliar = MAPA_LINHA - 1;
	int chamadas = 0;
	int quantidade_de_oponentes = 4;
	int escolha_game_over = 0;
	int Tipos[5] = { TIPO_FOGO,TIPO_AGUA,TIPO_GRAMA, TIPO_ELETRICO, TIPO_VOADOR };


	srand(time(NULL));

	do{
		mapa.lider_de_ginasio.linha = 0;
		duelo.turno = 0;
		Gerador_de_Pokemon(pokemons, Tipos);
		definir_resolucao();
		preencher_mapa(mapa, linha, coluna);
		cerca_padrao(mapa, auxiliar, chamadas);
		obstaculo(mapa, cordenadas_spawn_obstaculos);
		atribuir_posicao_jogador(mapa);
		posicionar_lider(mapa);
		atribuir_valores_a_batalha(duelo);
		posicionar_elementos(mapa);
		desenhar_mapa(mapa, linha, coluna);
		jogo(mapa, pokemons, duelo, linha, coluna);
		escolha_game_over = game_over(duelo);
		if (escolha_game_over == 1) {
			reinicializar_variaveis(chamadas, quantidade_de_oponentes, auxiliar, linha, coluna);
		}
	} while (escolha_game_over != 2);
	return 0;
}