#include <fstream>
#include <iostream>
#include <string>

#include "ui/interfaz.h"

using namespace std;

const int SALIR = 0;

int new_game(ListaSudoku& listado);
int continuar(ListaSudoku& listado);
bool cargar_partidas(ListaSudoku& listado);
void guardar_partida(ListaSudoku& lista, int pos);
void guardar_partidas(ListaSudoku& lista);
void serializar_sudoku(ofstream& archivo, ListaSudoku& lista, int pos);
void serializar_sudoku(ofstream& archivo, const tReglasSudoku& reglas);
void guardar_partida(const tReglasSudoku& reglas);
void guardar_partida(ListaSudoku& lista, int pos);
bool cargar_lista_nuevos(ListaSudoku& listado);



int main()
{
    ListaSudoku lista_nuevos;
    ListaSudoku lista_partidas;

    cargar_lista_nuevos(lista_nuevos);
    cargar_partidas(lista_partidas);

    int sudoku = -1;
    char opcion;

    cout << "Bienvenido, elije una de las siguientes opciones para continuar: " << endl;

    while (sudoku < 0)
    {
        opcion = start_menu();

        switch (opcion)
        {
        case 'N':
            sudoku = submenu_sudoku(lista_nuevos);
            break;
        case 'C':
            sudoku =  submenu_sudoku(lista_partidas);
            break;
        case 'A':
            guardar_partidas(lista_partidas);
            sudoku = SALIR;
            break;
        }
    }

    if(sudoku > 0){
        tReglasSudoku r_sudoku;

        if (opcion == 'N') {
            r_sudoku = lista_nuevos.dame_sudoku(sudoku - 1);
        } else {
            r_sudoku = lista_partidas.dame_sudoku(sudoku - 1);
        }

        bool completado = comenzar_partida(r_sudoku);
        
        if(!completado){
            if(opcion == 'N') {
                guardar_partida(r_sudoku);
            }
            else if(opcion == 'C'){
                lista_partidas.eliminar(sudoku-1);
                lista_partidas.insertar(r_sudoku);
                guardar_partidas(lista_partidas);
            }

            cout << "Progreso guardado! Hasta la proxima" << endl;
        
        } else {
            // Sudoku completado
            if(opcion == 'C'){
                cout << "¡Felicidades! Has completado el sudoku. Se eliminará de tu lista de partidas." << endl;
                lista_partidas.eliminar(sudoku-1);
                guardar_partidas(lista_partidas);
            }
        }
    }

    return 0;
}

bool cargar_lista_nuevos(ListaSudoku& listado){
    bool cargado = false;
    ifstream archivo("log_files/lista_sudokus.txt");

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

    return cargado;
}



int continuar(ListaSudoku& listado){
    int sudoku = -1;
    if (cargar_partidas(listado)) {
        sudoku = submenu_sudoku(listado);
    }

    return sudoku;
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


void serializar_sudoku(ofstream& archivo, ListaSudoku& lista, int pos)
{
    serializar_sudoku(archivo, lista.dame_sudoku(pos));
}

void serializar_sudoku(ofstream& archivo, const tReglasSudoku& reglas)
{
    tReglasSudoku actual   = reglas;
    tReglasSudoku original = actual;
    original.reset();

    int dim = actual.dame_dimension();
    archivo << dim << endl;

    for (int f = 0; f < dim; f++)
    {
        for (int c = 0; c < dim; c++)
        {
            archivo << original.dame_celda(f, c);
            if (c < dim - 1) archivo << " ";
        }
        archivo << endl;
    }

    for (int f = 0; f < dim; f++)
    {
        for (int c = 0; c < dim; c++)
        {
            int val_actual   = actual.dame_celda(f, c);
            int val_original = original.dame_celda(f, c);

            if (val_original == 0 && val_actual != 0)
                archivo << f << " " << c << " " << val_actual << endl;
        }
    }

    archivo << -1 << endl;
}

void guardar_partida(ListaSudoku& lista, int pos)
{
    guardar_partida(lista.dame_sudoku(pos));
}

void guardar_partida(const tReglasSudoku& reglas)
{
    int count = 0;
    string contenido_actual = "";

    ifstream leer("log_files/lista_partidas.txt");
    if (leer.is_open())
    {
        leer >> count;
        leer.ignore();
        string linea;
        while (getline(leer, linea))
            contenido_actual += linea + "\n";
        leer.close();
    }
    count++;

    ofstream archivo("log_files/lista_partidas.txt");
    if (!archivo.is_open())
    {
        cout << "Error: no se pudo abrir lista_partidas.txt" << endl;
        return;
    }

    archivo << count << endl;
    archivo << contenido_actual;
    serializar_sudoku(archivo, reglas);
    archivo.close();
}

void guardar_partidas(ListaSudoku& lista)
{
    ofstream archivo("log_files/lista_partidas.txt");
    if (!archivo.is_open())
    {
        cout << "Error: no se pudo abrir lista_partidas.txt" << endl;
        return;
    }

    archivo << lista.dame_num_elems() << endl;
    for (int i = 0; i < lista.dame_num_elems(); i++)
        serializar_sudoku(archivo, lista, i);

    archivo.close();
}