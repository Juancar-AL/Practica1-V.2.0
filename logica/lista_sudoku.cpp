#include "lista_sudoku.h"


ListaSudoku::ListaSudoku() : size(0) , cap(0), lista_sudokus(nullptr) {}

ListaSudoku::~ListaSudoku()
{
    delete[] lista_sudokus;
    lista_sudokus = nullptr;
}


int ListaSudoku::dame_num_elems() const {return size;}

const tReglasSudoku& ListaSudoku::dame_sudoku(const int& i){
    return lista_sudokus[i];
}

void ListaSudoku::insertar(const tReglasSudoku& sudoku){
    if(size == cap){
        int newCap = (cap == 0) ? 1 : cap *2;
        resize_(newCap);
    }

    // Encontrar posición ordenada por número de celdas vacías (menor dificultad primero)
    int pos = size;
    int celdas_vacias_nuevo = sudoku.dame_num_celdas_vacias();
    
    for (int i = 0; i < size; i++)
    {
        if (celdas_vacias_nuevo < lista_sudokus[i].dame_num_celdas_vacias())
        {
            pos = i;
            break;
        }
    }

    // Desplazar elementos a la derecha desde pos
    for (int i = size; i > pos; i--)
    {
        lista_sudokus[i] = lista_sudokus[i - 1];
    }

    // Insertar en posición ordenada
    lista_sudokus[pos] = sudoku;
    size++;
}

void ListaSudoku::eliminar(const int& pos){
    for (int i = pos; i < size; i++)
    {
        lista_sudokus[i] = lista_sudokus[i+1];
    }

    size--;
    
} // elimina el elemento de la posición pos