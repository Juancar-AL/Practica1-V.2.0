#pragma once

#include "reglas_sudoku.h"
#include <iostream>

class ListaSudoku {
public:
    // Constructores y destructores
    ListaSudoku();
    ~ListaSudoku();
    ListaSudoku(const ListaSudoku& lista);

    // Operadores
    ListaSudoku& operator=(const ListaSudoku& lista);
    tReglasSudoku& operator[](int indice);
    const tReglasSudoku& operator[](int indice) const;
    bool operator<(const ListaSudoku& otra) const;
    bool operator==(const ListaSudoku& otra) const;

    // Métodos consultores
    int dame_num_elems() const;
    const tReglasSudoku& dame_sudoku(const int& i);

    // Métodos modificadores
    void insertar(const tReglasSudoku& sudoku);
    void eliminar(const int& pos);

private:
    // Miembros de datos privados
    tReglasSudoku** lista_sudokus;
    int size;
    int cap;

    // Métodos privados - utilidades
    void resize_(int newCap) {
        tReglasSudoku** nuevo = new tReglasSudoku*[newCap];
        for (int i = 0; i < size; i++)
        {
            nuevo[i] = lista_sudokus[i];
        }

        for (int i = size; i < newCap; i++)
        {
            nuevo[i] = nullptr;
        }

        delete[] lista_sudokus;
        lista_sudokus = nuevo;
        cap = newCap;
    }
};

