#include <fstream>
#include <string>

#include "logica/lista_sudoku.h"

#include "ui/interfaz.h"

using namespace std;

void continuar();
void new_game(ListaSudoku& listado);

/*TODO-LIST

Funcón para la lectura de partidas sin terminar
Revisar función para inicio de un juego nuevo
Funcón que imprima por pantalla el número de celdas con 1 valor posible, 2...

Revisar lógica para el cambio de partida
*/


int main()
{
    ListaSudoku lista;

    char opcion = start_menu();

    switch (opcion)
    {
    case 'N':
        new_game(lista);
        break;
    case 'C':

    default:
        break;
    }


    ifstream archivo("sudokus/sudoku_1.txt");

    comenzar_partida(archivo);
    return 0;
}

void new_game(ListaSudoku& listado){
    ifstream archivo("log_fules/lista_sudokus.txt");

    int num_sudokus;
    
    archivo >> num_sudokus;

    for (int i = 0; i < num_sudokus; i++)
    {
        string string_sudoku;
        
        archivo >> string_sudoku;
        ifstream archivo_sudoku(string_sudoku);
        if (archivo_sudoku.is_open())
        {
            tReglasSudoku regla;
            regla.carga_sudoku(archivo_sudoku);

            listado.insertar(regla);

            archivo_sudoku.close();
        }
        
    }
    
}

void continuar(){
    ifstream archivo("log_files/lista_partidas.txt");

    if (archivo.is_open())
    {
        int num_partidas;
    
        archivo >> num_partidas;

        for (int i = 0; i < num_partidas; i++)
        {
            tReglasSudoku sudoku_actual;


            bool carga = sudoku_actual.carga_sudoku(archivo);

            if(carga){

            }
        }
        
    }
}
