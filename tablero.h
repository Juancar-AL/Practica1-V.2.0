#pragma once

#include "celda.h"

const int MAX = 9;

class tTablero
{
private:
    int dim;
    tCelda celdas[MAX][MAX];

public:
    tTablero();
    tTablero(int dimension);
    int dame_dimension() const;
    tCelda dame_elem(int i, int j) const;
    void colocar_celda(int i, int j, const tCelda& celda);
};