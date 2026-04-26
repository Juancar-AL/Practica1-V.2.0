#include "celda.h"

tCelda::tCelda()
{
    valor = 0;
    estadoActual = vacia;
}

void tCelda::Celda(int v, tEstado estado)
{
    valor = v;
    estadoActual = estado;
}

bool tCelda::es_vacia()  const
{
    return estadoActual == vacia;
}

bool tCelda::es_original()  const
{
    return estadoActual == original;
}

bool tCelda::es_ocupada()  const
{
    return estadoActual == ocupada;
}

int tCelda::dame_valor()  const
{
    return valor;
}

void tCelda::set_valor(int v)
{
    valor = v;
}

void tCelda::set_ocupada()
{
    estadoActual = ocupada;
}

void tCelda::set_original()
{
    estadoActual = original;
}

void tCelda::set_vacia()
{
    estadoActual = vacia;
}