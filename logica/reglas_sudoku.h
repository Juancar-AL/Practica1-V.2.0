#pragma once

#include <fstream>
#include "../modelo/tablero.h"

using namespace std;


class tReglasSudoku{
    private:

    typedef struct{
        int lista[MAX*MAX][2];
        int cont;
    } tListaBloqueos;


    tTablero tablero;
    int cont;
    tListaBloqueos lista;

    bool coord_valid(const int& f, const int& c) const;
    void actualizar_bloqueos();

    void ini_matriz3D();

    void actualizar_posibles(const int&f, const int& c, const int& valor);
    void anadir_posibles(const int&f, const int& c, const int& valor);

    void quitar_posible(const int& f, const int& c, const int val);



    //Actualización

    typedef struct {
        bool posible;
        int celdas_que_afectan;
    } tValor;
    typedef struct {
        int nFilas;
        int nColumnas;
        tValor valores[MAX][MAX][MAX];
    } tValores;
    tValores valores_celda;

    public:
    tReglasSudoku();
    int dame_dimension() const; // devuelve la dimensión del tablero
    int dame_celda(int f, int c) const; // devuelve la celda en la posición (f,c)
    bool terminado() const; // true si y sólo si el Sudoku está resuelto
    bool bloqueo() const; // true si el Sudoku tiene celdas bloqueadas
    int dame_num_celdas_bloqueadas() const; // devuelve el número de celdas bloqueadas
    int dame_num_celdas_vacias() const; // devuelve el número de celdas vacías
    void dame_celda_bloqueada(int p, int& f, int& c) const; // devuelve en (f,c) la celda bloqueada en la posición p
    bool es_valor_posible(int f, int c, int v) const; // true si y sólo si v se puede colocar en (f,c)
    int posibles_valores(int f, int c, int valores[] = nullptr) const; // devuelve el número de posibles valores para (f,c) y opcionalmente los escribe en valores[]
    /* modificadoras */
    bool pon_valor(int f, int c, int v); // pone v en (f,c)
    bool quita_valor(int f, int c); // pone la celda (f,c) a VACIA
    void reset(); // recupera el Sudoku original
    void autocompletar(); // rellena todas las celdas con un único valor posible
    /* inicializadora */
    bool carga_sudoku(ifstream&); // carga un Sudoku original de un archiv

    /*Otros métdos privados*/
}; 