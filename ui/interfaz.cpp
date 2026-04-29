#include "interfaz.h"

#include <cmath>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <thread>

#include "colors.h"

using namespace std;


void mostrar_sudoku(const tReglasSudoku& reglas){
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
                if (reglas.terminado())
                    this_thread::sleep_for(chrono::milliseconds(40));
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
}


int menu_sudoku(const tReglasSudoku &reglas, const tError error)
{
    int opcion = -1;
    string error_texto;

    mostrar_sudoku(reglas);

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
    if (error != bloqueada)
    {
        int fila, columna, v;
        cout << "Introduce fila y columna: ";
        cin >> fila >> columna;
        cout << "Introduce el valor: ";
        cin >> v;

        if (!reglas.pon_valor(fila, columna, v))
        {
            error = valor;
        }
        else
        {
            if (reglas.bloqueo())
                error = bloqueada;
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

    if (!reglas.quita_valor(fila, columna))
    {
        error = original;
    }
}

int posibles_valores(tReglasSudoku &reglas, tError &error)
{
    int cantidad = -1;
    int fila, columna;
    int dim = reglas.dame_dimension();
    cout << "Introduce fila y columna: " << endl;
    cin >> fila >> columna;
    int lista[MAX];
    if (fila >= 0 && columna >= 0 && fila < dim && columna < dim)
    {
        if (reglas.dame_celda(fila, columna) == 0)
        {
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
        }
        else
            error = ocupada;
    }
    else
        error = valor;
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

    cout << "Partida nueva (N) | Continuar partida (C) | Abandonar la aplicacion (A)" << endl;

    while (!opcion_valida)
    {
        cin >> opcion;

        opcion_valida = (opcion == 'A' || opcion == 'C' || opcion == 'N');

        if (!opcion_valida)
        {
            cout << "Opcion no valida | Debes introducir 'N' , 'C' o 'A'" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return opcion;
}

void comenzar_partida(const tReglasSudoku &reglas)
{
    bool guardada = false;
    tReglasSudoku sudoku_copia = reglas;
    tError error = ninguno;

    int opcion = menu_sudoku(sudoku_copia, error);
    while (opcion != 6)
    {
        error = ninguno;
        switch (opcion)
        {
        case 1:
            poner_valor(sudoku_copia, error);
            break;
        case 2:
            quitar_valor(sudoku_copia, error);
            break;
        case 3:
            sudoku_copia.reset();
            break;
        case 4:
            posibles_valores(sudoku_copia, error);
            pausar();
            break;
        case 5:
            sudoku_copia.autocompletar();
            break;
        default:
            error = opciones;
            break;
        }
        opcion = menu_sudoku(sudoku_copia, error);
    }

    if(!guardada){
        char guardar;
        cout << "¡Espera! No has guardado tu partida. ¿Quieres guardar tu progreso? (S/N)" << endl;
        pausar();
        while (guardar != 'S' || guardar != 'N')
        {
            cin >> guardar;
        }
    }    
}

void leer_casillas_sudoku(const tReglasSudoku& reglas){
    int posibles[MAX] = {0};
    int num_celdas_vacias = 0;

    for (int i = 0; i < reglas.dame_dimension(); i++)
    {
        for (int j = 0; j < reglas.dame_dimension(); j++)
        {
            if (reglas.dame_celda(i,j) == 0)
            {
                num_celdas_vacias++;
                int posibles_valores = reglas.posibles_valores(i,j);
                if (posibles_valores >= 1 && posibles_valores <= MAX)
                {
                    posibles[posibles_valores - 1]++;
                }
            }
            
        }
        
    }

    cout << "Sudoku con " << num_celdas_vacias << " celdas vacias" << '\n';
    for (int i = 1; i <= reglas.dame_dimension(); i++)
    {
        cout << "\tCeldas con " << i << " valores posibles: " << posibles[i - 1] << '\n';
    }
    
}