#pragma once

class tCelda
{
private:

    enum tEstado
    {
        original,
        vacia,
        ocupada
    };

    int valor;
    tEstado estadoActual;


    void Celda(int v, tEstado estado);
public:
    tCelda();
    
    /* métodos de consulta */
    bool es_vacia() const;    // devuelve true si y sólo si la celda es VACIA
    bool es_original() const; // devuelve true si y sólo si la celda es ORIGINAL
    bool es_ocupada() const;  // devuelve true si y sólo si la celda es OCUPADA
    int dame_valor() const;   // devuelve el valor de la celda
    /* métodos modificadores*/
    void set_valor(int v); // pone el valor v en la celda
    void set_ocupada();    // pone el estado de la celda a OCUPADA
    void set_original();   // pone el estado de la celda a ORIGINAL
    void set_vacia();      // pone el estado de la celda a VACIA
};