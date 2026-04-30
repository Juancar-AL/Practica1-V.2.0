#pragma once

#include "celda.h"

const int MAX = 9;

class tTablero {
public:
    // Constructores
    tTablero();
    tTablero(int dimension);

    // Métodos consultores
    int dame_dimension() const;
    tCelda dame_elem(int i, int j) const;

    // Métodos modificadores
    void colocar_celda(int i, int j, const tCelda& celda);

private:
    // Miembros de datos privados
    int dim;
    tCelda celdas[MAX][MAX];
};