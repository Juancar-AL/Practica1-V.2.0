#pragma once

#include <fstream>
#include "../logica/lista_sudoku.h"

using namespace std;

// Estructura de selección
typedef struct tInfoSeleccion {
    tReglasSudoku sudoku;
    int indice;
    char tipo;
} tInfoSeleccion;

// Enumeración de errores
enum tError {
    ninguno,
    opciones,
    valor,
    bloqueada,
    original,
    vacia,
    no_resuelto,
    ocupada
};

// Funciones de menú principal
char start_menu();
tInfoSeleccion menu_inicio(const ListaSudoku& lista_nuevos, const ListaSudoku& lista_partidas);

// Funciones de juego
bool comenzar_partida(tReglasSudoku &reglas);
int menu_sudoku(const tReglasSudoku &reglas, const tError error);
bool resolver_sudoku(tReglasSudoku& sudoku, int fila, int columna);

// Funciones de selección
int submenu_sudoku(const ListaSudoku& listado);
bool gestionar_seleccion(const tReglasSudoku& sudoku);

// Funciones de visualización
void mostrar_sudoku(const tReglasSudoku& reglas);
void mostrar_lista(const ListaSudoku& lista);
void mostrar_bloqueos(const tReglasSudoku& reglas);

// Funciones de entrada/modificación
void poner_valor(tReglasSudoku &reglas, tError &error);
void quitar_valor(tReglasSudoku &reglas, tError &error);
int posibles_valores(tReglasSudoku &reglas, tError &error);

// Funciones auxiliares
string traducir_error(tError error);
void pausar();