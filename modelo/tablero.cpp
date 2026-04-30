#include "tablero.h"

// ==================== CONSTRUCTORES ====================

tTablero::tTablero() {
    dim = MAX;
}

tTablero::tTablero(int dimension) {
    dim = dimension;
}

// ==================== MÉTODOS CONSULTORES ====================

int tTablero::dame_dimension() const {
    return dim;
}

tCelda tTablero::dame_elem(int i, int j) const {
    tCelda celda;
    if (i >= 0 && j >= 0 && i < dim && j < dim) {
        celda = celdas[i][j];
    }
    return celda;
}

// ==================== MÉTODOS MODIFICADORES ====================

void tTablero::colocar_celda(int i, int j, const tCelda& celda) {
    if (i >= 0 && j >= 0 && i < dim && j < dim) {
        celdas[i][j] = celda;
    }
}