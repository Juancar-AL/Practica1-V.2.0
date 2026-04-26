#include "lista_sudoku.h"


ListaSudoku::ListaSudoku() : size(0) , cap(0), lista_sudokus(nullptr) {}

ListaSudoku::~ListaSudoku()
{
    delete[] lista_sudokus;
    lista_sudokus = nullptr;
}


int ListaSudoku::dame_num_elems(){return size;}

const tReglasSudoku& ListaSudoku::dame_sudoku(const int& i){
    return lista_sudokus[i];
}

void ListaSudoku::insertar(const tReglasSudoku& sudoku){
    if(size == cap){
        int newCap = (cap == 0) ? 1 : cap *2;
        resize_(newCap);
    }

    lista_sudokus[size] = sudoku;
    size++;
}

void ListaSudoku::eliminar(const int& pos){
    for (int i = pos; i < size; i++)
    {
        lista_sudokus[i] = lista_sudokus[i+1];
    }

    size--;
    
} // elimina el elemento de la posición pos