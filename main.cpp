#include <fstream>
#include <iostream>
#include <string>

#include "ui/interfaz.h"

using namespace std;

const int SALIR = 0;

int new_game(ListaSudoku& listado);
int continuar(ListaSudoku& listado);

/*TODO-LIST

Funcón para la lectura de partidas sin terminar
Revisar función para inicio de un juego nuevo

Revisar lógica para el cambio de partida
*/


int main()
{
    ListaSudoku lista;

    int sudoku = -1;

    cout << "Bienvenido, elije una de las siguientes opciones para continuar: " << endl;

    while (sudoku < 0)
    {
        char opcion = start_menu();

        switch (opcion)
        {
        case 'N':
            sudoku = new_game(lista);
            break;
        case 'C':
            sudoku = continuar(lista);
            break;
        case 'A':
            sudoku = SALIR;
            break;
        }
    }

    if(sudoku > 0){
        comenzar_partida(lista.dame_sudoku(sudoku - 1));
    }
    return 0;
}

int new_game(ListaSudoku& listado){
    ifstream archivo("log_files/lista_sudokus.txt");

    int num_sudokus;
    
    archivo >> num_sudokus;

    for (int i = 0; i < num_sudokus; i++)
    {
        string string_sudoku;
        
        archivo >> string_sudoku;
        ifstream archivo_sudoku("sudokus/" + string_sudoku);
        if (archivo_sudoku.is_open())
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

    for (int i = 0; i < listado.dame_num_elems(); i++)
    {
        cout << i + 1 << ": ";
        leer_casillas_sudoku(listado.dame_sudoku(i));
    }

    int sudoku;
    cout << "Elije un sudoku (1 - " << listado.dame_num_elems() << ") : ";
    cin >> sudoku;

    return sudoku;
    
}

int continuar(ListaSudoku& listado){
    ifstream archivo("log_files/lista_partidas.txt");
    int sudoku = -1;

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

            for (int i = 0; i < listado.dame_num_elems(); i++)
            {
                cout << i + 1 << ": ";
                leer_casillas_sudoku(listado.dame_sudoku(i));
            }

            cout << "Elije un sudoku (1 - " << listado.dame_num_elems() << ") : ";
            cin >> sudoku;
        } else{
            cout << "No hay partidas guardadas" << endl;
        }
    }
    else
    {
        cout << "No se pudo abrir log_files/lista_partidas.txt\n";
    }

    return sudoku;
}
