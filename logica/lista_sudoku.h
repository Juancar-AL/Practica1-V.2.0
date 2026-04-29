#pragma once

#include "reglas_sudoku.h"

class ListaSudoku
{
private:
    tReglasSudoku* lista_sudokus;
    int size;
    int cap;

    void resize_(int newCap){
        tReglasSudoku* nuevo = new tReglasSudoku[newCap];
        for (int i = 0; i < size; i++)
        {
            nuevo[i] = lista_sudokus[i];
        }

        delete[] lista_sudokus;
        lista_sudokus = nuevo;
        cap = newCap;
    }

public:
    ListaSudoku();
    ~ListaSudoku();

    int dame_num_elems() const;
    const tReglasSudoku& dame_sudoku(const int& i);
    void insertar(const tReglasSudoku& sudoku);
    void eliminar(const int& pos); // elimina el elemento de la posición pos
    void mostrar_lista();


};

