#pragma once

#include <fstream>
#include "../modelo/tablero.h"

using namespace std;


class tReglasSudoku{
    private:

    typedef struct
    {
        int fila;
        int columna;
    }tPosicion;
    

    typedef struct{
        tPosicion** lista;
        int cont;
        int cap;
    } tListaBloqueos;


    tTablero tablero;
    int cont;
    tListaBloqueos lista;

    typedef int arrValores[MAX];
    arrValores cuantas_celdas;

    bool coord_valid(const int& f, const int& c) const;
    void actualizar_bloqueos();

    void ini_matriz3D();

    void actualizar_posibles(const int&f, const int& c, const int& valor);
    void anadir_posibles(const int&f, const int& c, const int& valor);

    void quitar_posible(const int& f, const int& c, const int val);

    void recalcular_cuantas_celdas();

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

    void tReglasSudoku::restaurar_posible(const int& f, const int& c, const int val);
    void tReglasSudoku::modificar_afectados(const int& f, const int& c, const int& valor, bool quitar);

    public:
    tReglasSudoku();
    tReglasSudoku(const tReglasSudoku& otro);  // constructor por copia
    ~tReglasSudoku();
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
    bool carga_sudoku(ifstream&); // carga un Sudoku original de un archivo

    tReglasSudoku& operator=(const tReglasSudoku& otro);  // operador de asignación

    int cuantas_celdas_pueden_tener(int n_valores) const;

    bool operator<(const tReglasSudoku& s2) const;
    bool operator==(const tReglasSudoku& s2) const;

    /*Otros métdos públicos*/
}; 