#include <fstream>
#include <iostream>
#include <string>

#include "ui/interfaz.h"

using namespace std;

const int SALIR = 0;

typedef struct {
    tReglasSudoku sudoku; // El objeto sudoku listo para jugar
    int indice;           // Su posición en la lista (para borrar/actualizar después)
    char tipo;            // 'N' para Nuevo, 'C' para Continuar, 'S' para Salir
} tInfoSeleccion;

bool cargar_partidas(ListaSudoku& listado);
void guardar_partidas(ListaSudoku& lista);
void serializar_sudoku(ofstream& archivo, const tReglasSudoku& reglas);
bool cargar_lista_nuevos(ListaSudoku& listado);
void actualizar_listas(ListaSudoku& lista_partidas, const tInfoSeleccion& sel, bool completado);
tInfoSeleccion menu_inicio(const ListaSudoku& lista_nuevos, const ListaSudoku& lista_partidas);

int main()
{
    ListaSudoku lista_nuevos;
    ListaSudoku lista_partidas;

    cargar_lista_nuevos(lista_nuevos);
    cargar_partidas(lista_partidas);

    tInfoSeleccion sel;
    sel.tipo = ' ';

    while (sel.tipo != 'S')
    {
        cout << "Bienvenido, elije una de las siguientes opciones para continuar: " << endl;

        sel = menu_inicio(lista_nuevos, lista_partidas);
        
        if(sel.tipo != 'S'){

            bool completado = comenzar_partida(sel.sudoku);
            actualizar_listas(lista_partidas, sel, completado);
        }

        cout << "Volviendo al menu principal..." << endl;
    }

    guardar_partidas(lista_partidas);

    cout << "Guardando progreso..." << endl;

    return 0;
}

void actualizar_listas(ListaSudoku& lista_partidas, const tInfoSeleccion& sel, bool completado) {
    if (!completado) {
        // Si no terminó, guardamos o actualizamos el progreso
        if (sel.tipo == 'N') {
            lista_partidas.insertar(sel.sudoku);
        } else if (sel.tipo == 'C') {
            lista_partidas.eliminar(sel.indice); // Usamos el índice guardado
            lista_partidas.insertar(sel.sudoku);
        }
        cout << "Progreso guardado! Hasta la proxima" << endl;
    } else {
        // Si completó una partida que ya estaba en "Continuar", la borramos
        if (sel.tipo == 'C') {
            cout << "¡Felicidades! Has completado el sudoku. Se eliminara de tu lista de partidas." << endl;
            lista_partidas.eliminar(sel.indice);
        }
    }
}


tInfoSeleccion menu_inicio(const ListaSudoku& lista_nuevos, const ListaSudoku& lista_partidas) {
    tInfoSeleccion seleccion;
    seleccion.tipo = ' ';
    seleccion.indice = -1;

    bool terminado = false;
    while (!terminado) {
        char opcion = start_menu(); // Devuelve 'N', 'C' o 'A'

        if (opcion == 'A') {
            seleccion.tipo = 'S';
            terminado = true;
        } else if (opcion == 'N') {
            int idx = submenu_sudoku(lista_nuevos);
            if (idx != -1) {
                seleccion.tipo = 'N';
                seleccion.indice = idx;
                seleccion.sudoku = lista_nuevos[idx]; // Copiamos el sudoku elegido
                terminado = true;
            }
        } else if (opcion == 'C') {
            int idx = submenu_sudoku(lista_partidas);
            if (idx != -1) {
                seleccion.tipo = 'C';
                seleccion.indice = idx;
                seleccion.sudoku = lista_partidas[idx]; // Copiamos la partida guardada
                terminado = true;
            }
        }
    }
    return seleccion;
}
bool cargar_lista_nuevos(ListaSudoku& listado){
    bool cargado = false;
    ifstream archivo("log_files/lista_sudokus.txt");

    if (archivo.is_open())
    {
    
        int num_sudokus;
        
        archivo >> num_sudokus;

        for (int i = 0; i < num_sudokus; i++)
        {
            string string_sudoku;
            
            archivo >> string_sudoku;
            ifstream archivo_sudoku("sudokus/" + string_sudoku);
            cargado = archivo_sudoku.is_open();
            if (cargado)
            {
                tReglasSudoku regla;
                regla.carga_sudoku(archivo_sudoku);

                listado.insertar(regla);

                archivo_sudoku.close();
            }
            else
            {
                cout << "No se pudo abrir sudokus/" << string_sudoku << '\n';
            }
            
        }
    }

    archivo.close();

    return cargado;
}


bool cargar_partidas(ListaSudoku& listado){
    ifstream archivo("log_files/lista_partidas.txt");
    bool carga_ok = false;

    if (archivo.is_open())
    {
        int num_partidas;
    
        archivo >> num_partidas;

        if(num_partidas>0){
            for (int i = 0; i < num_partidas; i++)
            {
                tReglasSudoku reglas;

                bool carga = reglas.carga_sudoku(archivo);

                if(carga){
                    int fila = 0;
                    
                    while (fila != -1)
                    {
                        archivo >> fila;
                        
                        if (fila != -1)
                        {
                            int columna, valor;
                            archivo >> columna >> valor;
                            reglas.pon_valor(fila, columna, valor);
                        }
                    }

                    listado.insertar(reglas);
                }
            }

            archivo.close();
            carga_ok = true;
        } else{
            cout << "No hay partidas guardadas" << endl;
        }
    }
    else
    {
        cout << "No se pudo abrir log_files/lista_partidas.txt\n";
    }

    return carga_ok;
}

void serializar_sudoku(ofstream& archivo, const tReglasSudoku& reglas)
{
    tReglasSudoku original   = reglas;
    original.reset();

    int dim = reglas.dame_dimension();
    archivo << dim << endl;

    for (int f = 0; f < dim; f++)
    {
        for (int c = 0; c < dim; c++) {
            archivo << original.dame_celda(f, c) << ((c < dim - 1) ? " " : "");
            cout << original.dame_celda(f, c) << " ";
        }
        archivo << endl;
        cout << endl;
    }

    for (int f = 0; f < dim; f++)
    {
        for (int c = 0; c < dim; c++)
        {
            int val_actual   = reglas.dame_celda(f, c);
            int val_original = original.dame_celda(f, c);

            if (val_original == 0 && val_actual != 0)
                archivo << f << " " << c << " " << val_actual << endl;
        }
    }

    archivo << -1 << endl;
}

void guardar_partidas(ListaSudoku& lista)
{
    ofstream archivo("log_files/lista_partidas.txt");
    if (!archivo.is_open())
    {
        cout << "Error: no se pudo abrir lista_partidas.txt" << endl;
    } else{
        archivo << lista.dame_num_elems() << endl;
        for (int i = 0; i < lista.dame_num_elems(); i++)
            serializar_sudoku(archivo, lista[i]);

        archivo.close();

    }
}