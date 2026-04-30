#pragma once

#include "reglas_sudoku.h"

#include <iostream>

class ListaSudoku
{
private:
    tReglasSudoku** lista_sudokus;
    int size;
    int cap;

    void resize_(int newCap){
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

public:
    ListaSudoku();
    ~ListaSudoku();

    ListaSudoku(const ListaSudoku& lista);

    int dame_num_elems() const;
    const tReglasSudoku& dame_sudoku(const int& i);
    void insertar(const tReglasSudoku& sudoku);
    void eliminar(const int& pos); // elimina el elemento de la posición pos

    tReglasSudoku& operator[](int indice);
    const tReglasSudoku& operator[](int indice) const;

    ListaSudoku& operator=(const ListaSudoku& lista);
    bool operator<(const ListaSudoku& otra) const;
    bool operator==(const ListaSudoku& otra) const;

};

