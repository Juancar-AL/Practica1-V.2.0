#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <cmath>

#include "lista_sudoku.h"

#include "colors.h"

using namespace std;

enum tError
{
    ninguno,
    opciones,
    valor,
    bloqueada,
    original,
    vacia,
    ocupada
};

int mostrar_sudoku(const tReglasSudoku &reglas, const tError error);
void poner_valor(tReglasSudoku &reglas, tError &error);
void quitar_valor(tReglasSudoku &reglas, tError &error);
int posibles_valores(tReglasSudoku &reglas, tError &error);
void comenzar_partida(ifstream& archivo);

//Modificado

char start_menu();

void continuar();
void new_game(ListaSudoku& listado);

void pausar();

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


    ifstream archivo("sudoku_1.txt");

    comenzar_partida(archivo);
    return 0;
}

int mostrar_sudoku(const tReglasSudoku &reglas, const tError error)
{
    const int LINEA_HORIZONTAL = 196;
    const int LINEA_VERTICAL = 179;
    const int CRUCE = 197;
    const int ANCHO_CELDA = 3;

    int opcion = -1;
    int dim = reglas.dame_dimension();
    int dim_submatriz = (int)sqrt((double)dim);
    if (dim_submatriz <= 0)
        dim_submatriz = 1;
    string error_texto;

    cout << endl
         << CYAN << "================ SUDOKU ================" << RESET << '\n';

    // Cabecera de columnas
    cout << "    ";
    for (int c = 0; c < dim; c++)
    {
        cout << setw(3) << c;
        if ((c + 1) % dim_submatriz == 0 && c != dim - 1)
            cout << "  ";
    }

    cout << '\n';

    for (int i = 0; i < dim; i++)
    {
        cout << setw(3) << i << " ";
        for (int j = 0; j < dim; j++)
        {
            int v = reglas.dame_celda(i, j);
            if (v == 0)
                cout << setw(3) << '.';
            else{
                cout << CYAN << setw(3) << v << RESET;
                if(reglas.terminado()) usleep(40000);
            }

            // Separador de subcuadros de tamano variable
            if ((j + 1) % dim_submatriz == 0 && j != dim - 1)
                cout << " " << char(LINEA_VERTICAL);
        }
        cout << '\n';

        if ((i + 1) % dim_submatriz == 0 && i != dim - 1)
        {
            cout << "    ";
            cout << char(LINEA_HORIZONTAL);
            for (int j = 0; j < dim; j++)
            {
                for (int k = 0; k < ANCHO_CELDA; k++)
                    cout << char(LINEA_HORIZONTAL);

                if ((j + 1) % dim_submatriz == 0 && j != dim - 1)
                {
                    cout << char(CRUCE);
                    cout << char(LINEA_HORIZONTAL);
                }
            }
            cout << '\n';
        }
    }

    if (!reglas.terminado())
    {
        cout << '\n';
        cout << YELLOW << "------------- MENU -------------" << RESET << '\n';
        cout << "1. Poner valor\n";
        cout << "2. Quitar valor\n";
        cout << "3. Reset\n";
        cout << "4. Posibles valores de una celda vacia\n";
        cout << "5. Autocompletar\n";
        cout << "6. Salir\n";
        cout << YELLOW << "--------------------------------" << RESET << '\n';
        switch (error)
        {
        case opciones:
            error_texto = "OPCION INCORRECTA";
            break;
        case valor:
            error_texto = "VALOR INCORRECTO";
            break;
        case bloqueada:
            error_texto = "CELDA BLOQUEADA!";
            break;
        case original:
            error_texto = "CELDA ORIGINAL";
            break;
        case vacia:
            error_texto = "CELDA VACIA";
            break;
        case ocupada:
            error_texto = "CELDA OCUPADA";
            break;
        default:
            error_texto = "";
            break;
        }
        cout << BG_RED << error_texto << RESET << endl;
        if(error == bloqueada){
            int f, c;
            cout << "Celdas bloqueadas: ";
            for (int i = 0; i < reglas.dame_num_celdas_bloqueadas(); i++)
            {
                reglas.dame_celda_bloqueada(i, f, c);
                cout << "(" << f << ", " << c << ")" << ", ";
            }
            cout << endl;
            pausar();
        }
        cout << "Opcion: ";

        cin >> opcion;
    }
    else
    {
        cout << HK_PINK_PASTEL << "=========== SUDOKU TERMINADO ===========" << RESET;
        opcion = 6;
    }

    return opcion;
}

void poner_valor(tReglasSudoku &reglas, tError &error)
{
    if(error != bloqueada){
        int fila, columna, v;
        cout << "Introduce fila y columna: ";
        cin >> fila >> columna;
        cout << "Introduce el valor: ";
        cin >> v;

        if (!reglas.pon_valor(fila, columna, v))
        {
            error = valor;
        } else{
            if(reglas.bloqueo()) error = bloqueada;
            
        }
    }

}

void quitar_valor(tReglasSudoku &reglas, tError &error)
{
    int fila, columna;
    cout << "Introduce fila y columna: " << endl;
    cin >> fila >> columna;

    int dim = reglas.dame_dimension();
    if (fila < 0 || fila >= dim || columna < 0 || columna >= dim)
    {
        error = valor;
        return;
    }

    if (reglas.dame_celda(fila, columna) == 0)
    {
        error = vacia;
        return;
    }

    if (!reglas.quita_valor(fila, columna)){
        error = original;
    }
}

int posibles_valores(tReglasSudoku& reglas, tError& error)
{
    int cantidad = -1;
    int fila, columna;
    int dim = reglas.dame_dimension();
    cout << "Introduce fila y columna: " << endl;
    cin >> fila >> columna;
    int lista[MAX];
    if(fila >= 0 && columna >= 0 && fila < dim && columna < dim){
        if(reglas.dame_celda(fila, columna) == 0){
            cantidad = reglas.posibles_valores(fila, columna, lista);

            cout << "POSIBLES VALORES (" << cantidad << "): ";
            if (cantidad == 0)
            {
                cout << "ninguno";
            }
            else
            {
                for (int i = 0; i < cantidad; i++)
                {
                    cout << lista[i];
                    if (i < cantidad - 1)
                        cout << ", ";
                }
            }
            cout << endl;
        } else error = ocupada;
    } else error = valor;
    return cantidad;
}

void pausar()
{
    cout << "\nPresiona Enter para continuar...";
    cin.ignore();
    cin.get();
}

char start_menu()
{
    char opcion;
    bool opcion_valida = false;

    cout << "Bienvenido, elije una de las siguientes opciones para continuar: " << endl;
    cout << "Partida nueva (N) | Continuar partida (C) | Abandonar la aplicacion (A)" << endl;

    while (!opcion_valida)
    {
        cin >> opcion;

        opcion_valida = (opcion == 'A' || opcion == 'C' || opcion == 'N');

        if(!opcion_valida){
            cout << """Opcion no valida | Debes introducir 'N' , 'C' o 'A' """ << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }
    }
    
    return opcion;
}

void new_game(ListaSudoku& listado){
    ifstream archivo("lista_sudokus.txt");

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
    ifstream archivo("lista_partidas.txt");

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

void comenzar_partida(ifstream &archivo){

    tReglasSudoku reglas;

    tError error = ninguno;

    if (archivo.is_open())
    {
        if(reglas.carga_sudoku(archivo)){


            int opcion = mostrar_sudoku(reglas, error);
            while (opcion != 6)
            {
                error = ninguno;
                switch (opcion)
                {
                case 1:
                    poner_valor(reglas, error);
                    break;
                case 2:
                    quitar_valor(reglas, error);
                    break;
                case 3:
                    reglas.reset();
                    break;
                case 4:
                    posibles_valores(reglas, error);
                    pausar();
                    break;
                case 5:
                    reglas.autocompletar();
                    break;
                default:
                    error = opciones;
                    break;
                }
                opcion = mostrar_sudoku(reglas, error);
            }
        } else cout << BG_RED << "La dimension de la matriz propuesta es mayor a la admitida." << RESET;
    } else cout << BG_RED << "Ha habido un problema al cargar el archivo" << RESET;

}