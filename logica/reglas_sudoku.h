#pragma once

#include <fstream>

#include "../checkML.h"

#include "../modelo/tablero.h"

using namespace std;

class tReglasSudoku {
public:
    // Constructores y destructores
    tReglasSudoku();
    tReglasSudoku(const tReglasSudoku& otro);  // constructor por copia
    ~tReglasSudoku();

    // Operadores
    tReglasSudoku& operator=(const tReglasSudoku& otro);
    bool operator<(const tReglasSudoku& s2) const;
    bool operator==(const tReglasSudoku& s2) const;

    // Métodos consultores
    int dame_dimension() const;
    int dame_celda(int f, int c) const;
    bool terminado() const;
    bool bloqueo() const;
    int dame_num_celdas_bloqueadas() const;
    int dame_num_celdas_vacias() const;
    void dame_celda_bloqueada(int p, int& f, int& c) const;
    bool es_valor_posible(int f, int c, int v) const;
    int posibles_valores(int f, int c, int valores[] = nullptr) const;
    int cuantas_celdas_pueden_tener(int n_valores) const;

    // Métodos modificadores
    bool pon_valor(int f, int c, int v);
    bool quita_valor(int f, int c);
    void reset();
    void autocompletar();

    // Inicializadora
    bool carga_sudoku(ifstream&);

private:
    // Estructuras privadas
    typedef struct {
        int fila;
        int columna;
    } tPosicion;

    typedef struct {
        tPosicion** lista;
        int cont;
        int cap;
    } tListaBloqueos;

    typedef struct {
        bool posible;
        int celdas_que_afectan;
    } tValor;

    typedef struct {
        int nFilas;
        int nColumnas;
        tValor valores[MAX][MAX][MAX];
    } tValores;

    typedef int arrValores[MAX];

    // Miembros de datos privados
    tTablero tablero;
    int cont;
    tListaBloqueos lista;
    arrValores cuantas_celdas;
    tValores valores_celda;

    // Métodos privados - utilidades
    bool coord_valid(const int& f, const int& c) const;
    void ini_matriz3D();
    void recalcular_cuantas_celdas();

    // Métodos privados - gestión de bloqueos
    void actualizar_bloqueos();

    // Métodos privados - gestión de posibles valores
    void actualizar_posibles(const int& f, const int& c, const int& valor);
    void anadir_posibles(const int& f, const int& c, const int& valor);
    void quitar_posible(const int& f, const int& c, const int val);
    void restaurar_posible(const int& f, const int& c, const int val);
    void modificar_afectados(const int& f, const int& c, const int& valor, bool quitar);
}; 