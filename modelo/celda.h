#pragma once

class tCelda {
public:
    // Constructor por defecto
    tCelda();

    // Métodos consultores
    bool es_vacia() const;
    bool es_original() const;
    bool es_ocupada() const;
    int dame_valor() const;

    // Métodos modificadores
    void set_valor(int v);
    void set_ocupada();
    void set_original();
    void set_vacia();

private:
    // Enumeración privada
    enum tEstado {
        original,
        vacia,
        ocupada
    };

    // Miembros de datos privados
    int valor;
    tEstado estadoActual;

    // Métodos privados
    void Celda(int v, tEstado estado);
};