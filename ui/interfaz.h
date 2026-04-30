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
bool comenzar_partida(tReglasSudoku &reglas);
void leer_casillas_sudoku(const tReglasSudoku& reglas);
int menu_sudoku(const tReglasSudoku &reglas, const tError error);
void poner_valor(tReglasSudoku &reglas, tError &error);
void quitar_valor(tReglasSudoku &reglas, tError &error);
int posibles_valores(tReglasSudoku &reglas, tError &error);
void pausar();
void mostrar_sudoku(const tReglasSudoku& reglas);
int submenu_sudoku(ListaSudoku& listado);
bool resolver_sudoku(tReglasSudoku& sudoku, int fila, int columna);
void mostrar_lista(const ListaSudoku& lista);