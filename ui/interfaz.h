#pragma once

#include <fstream>
#include "../logica/lista_sudoku.h"

using namespace std;

enum tError
{
    ninguno,
    opciones,
    valor,
    bloqueada,
    original,
    vacia,
    ocupada
};

char start_menu();
void comenzar_partida(const tReglasSudoku &reglas);
void leer_casillas_sudoku(const tReglasSudoku& reglas);
int menu_sudoku(const tReglasSudoku &reglas, const tError error);
void poner_valor(tReglasSudoku &reglas, tError &error);
void quitar_valor(tReglasSudoku &reglas, tError &error);
int posibles_valores(tReglasSudoku &reglas, tError &error);
void pausar();
void mostrar_sudoku(const tReglasSudoku& reglas);