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


// ==================== FUNCIONES DE MENÚ ====================

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


// ==================== FUNCIONES DE VISUALIZACIÓN ====================

void mostrar_sudoku(const tReglasSudoku& reglas){
    
    const int LINEA_HORIZONTAL = 196;
    const int LINEA_VERTICAL = 179;
    const int CRUCE = 197;
    const int ANCHO_CELDA = 3;

    int dim = reglas.dame_dimension();
    int dim_submatriz = (int)sqrt((double)dim);
    if (dim_submatriz <= 0)
        dim_submatriz = 1;

    cout << endl
         << CYAN << "================ SUDOKU ================" << RESET << endl;

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

void mostrar_lista(const ListaSudoku& lista){
    for (int i = 0; i < lista.dame_num_elems(); i++)
    {
        cout << i + 1 << ": ";

        cout << "Sudoku con " << lista[i].dame_num_celdas_vacias() << " celdas vacias" << '\n';
        for (int j = 1; j <= lista[i].dame_dimension(); j++)
        {
            cout << "\tCeldas con " << j << " valores posibles: " << lista[i].cuantas_celdas_pueden_tener(j) << '\n';
        }
    }
    
}

void mostrar_bloqueos(const tReglasSudoku& reglas){
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


// ==================== FUNCIONES DE SELECCIÓN ====================

int submenu_sudoku(const ListaSudoku& listado) {
    int indice_resultado = -1;
    bool terminado = false;
    int seleccion;

    while (!terminado) {
        mostrar_lista(listado);
        cout << "Elije un sudoku (1 - " << listado.dame_num_elems() << ") o 0 para cancelar: ";
        
        // Comprobamos si la entrada es un número
        if (cin >> seleccion) {
            if (seleccion == 0) {
                terminado = true; // El usuario cancela
            } else if (seleccion > 0 && seleccion <= listado.dame_num_elems()) {
                // Si elige jugar en el siguiente menú, guardamos el índice y terminamos
                if (gestionar_seleccion(listado[seleccion - 1])) {
                    indice_resultado = seleccion - 1;
                    terminado = true;
                }
                // Si no elige jugar (opción '3' en gestionar_seleccion), 
                // terminado sigue siendo false y el bucle vuelve a mostrar la lista.
            } else {
                cout << "Error: Indice fuera de rango." << endl;
            }
        } else {
            // Si no es un número, limpiamos el error del flujo
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: Por favor, introduce un numero." << endl;
        }
    }

    return indice_resultado;
}

bool gestionar_seleccion(const tReglasSudoku& sudoku) {
    char opcion = ' ';
    while (opcion != '2' && opcion != '3') {
        cout << "\n1. Ver sudoku\n2. Jugar sudoku\n3. Volver a la lista\nOpcion: ";
        cin >> opcion;

        if (opcion == '1') {
            cout << "\nVisualizando tablero:" << endl;
            mostrar_sudoku(sudoku);
        }
    }
    return (opcion == '2'); // Retorna true solo si eligió jugar
}


// ==================== FUNCIONES DE ENTRADA/MODIFICACIÓN ====================

void poner_valor(tReglasSudoku &reglas, tError &error) {
    if (error != bloqueada) {
        int fila, columna, v;
        bool lectura_ok = true;

        cout << "Introduce fila y columna: ";
        
        // Intentamos leer fila y columna
        if (!(cin >> fila >> columna)) {
            error = valor;           // Marcamos error
            cin.clear();             // Limpiamos el flujo
            cin.ignore(1000, '\n');  // Vaciamos basura
            lectura_ok = false;      // Impedimos que siga leyendo
        }
        
        // Solo pedimos el valor si la fila y columna se leyeron bien
        if (lectura_ok) {
            cout << "Introduce el valor: ";
            if (!(cin >> v)) {
                error = valor;       // Marcamos error si falla el valor
                cin.clear();
                cin.ignore(1000, '\n');
                lectura_ok = false;
            }
        }

        // Solo intentamos poner el valor si las lecturas fueron exitosas
        if (lectura_ok) {
            if (!reglas.pon_valor(fila, columna, v)) {
                error = valor;       // Error si la lógica del juego lo rechaza
            } else {
                if (reglas.bloqueo()) {
                    error = bloqueada; // Error si el movimiento bloquea celdas
                }
            }
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


// ==================== FUNCIONES DE JUEGO ====================

bool comenzar_partida(tReglasSudoku &reglas)
{
    tError error = ninguno;

    int opcion = 0;

    while (opcion != 7)
    {

        opcion = menu_sudoku(reglas, error);

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
        case 6:
            resolver_sudoku(reglas, 0,0);
            break;
        default:
            error = opciones;
            break;
        }

        if (error == ninguno && reglas.bloqueo())
            error = bloqueada;

    } 

    return reglas.terminado();
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
        cout << "5. Autocompletar celdas con valor unico\n";
        cout << "6. Resolver el sudoku\n";
        cout << "7. Salir\n";
        cout << YELLOW << "--------------------------------" << RESET << '\n';
        
        error_texto = traducir_error(error);
        
        cout << BG_RED << error_texto << RESET << endl;

        if (error == bloqueada)
        {
            mostrar_bloqueos(reglas);
        }
        
        cout << "Opcion: ";

        cin >> opcion;
    }
    else
    {
        cout << HK_PINK_PASTEL << "=========== SUDOKU TERMINADO ===========" << RESET;
        opcion = 7;
    }

    return opcion;
}

bool resolver_sudoku(tReglasSudoku& sudoku, int fila, int columna){


    bool resuelto = false;
    int dimension = sudoku.dame_dimension(); // Usamos la dimensión de tu clase

    // 1. CASO BASE: Si llegamos a la fila final, terminamos
    if (fila == dimension) {
        resuelto = true;
    }
    // 2. SALTO DE LÍNEA: Si nos salimos por la derecha, pasamos a la siguiente fila
    else if (columna == dimension) {
        resuelto = resolver_sudoku(sudoku, fila + 1, 0);
    }
    // 3. CELDA OCUPADA: Avanzamos a la siguiente celda
    else if (sudoku.dame_celda(fila, columna) != 0) {
        resuelto = resolver_sudoku(sudoku, fila, columna + 1);
    }
    // 4. CELDA VACÍA:
    else {
        // Asumimos que los valores del Sudoku van de 1 a la dimensión (ej. 1 al 9)
        for (int i = 1; i <= dimension && !resuelto; i++) {

            // Si el número es válido según las reglas...
            if (sudoku.es_valor_posible(fila, columna, i)) {

                sudoku.pon_valor(fila, columna, i); // Lo ponemos

                // Intentamos resolver el resto del tablero
                resuelto = resolver_sudoku(sudoku, fila, columna + 1);

                // BACKTRACKING: Si este camino nos llevó a un callejón sin salida...
                if (!resuelto) {
                    sudoku.quita_valor(fila,columna);// Lo volvemos a dejar vacío (0)
                }
            }
        }
    }


    return resuelto;
}


// ==================== FUNCIONES AUXILIARES ====================

void pausar()
{
    cout << "\nPresiona Enter para continuar...";
    cin.ignore();
    cin.get();
}

string traducir_error(tError error) {
    string texto = "";
    switch (error) {
        case opciones:  texto = "OPCION INCORRECTA"; break;
        case valor:     texto = "VALOR INCORRECTO"; break;
        case bloqueada: texto = "CELDA BLOQUEADA!"; break;
        case original:  texto = "CELDA ORIGINAL"; break;
        case vacia:     texto = "CELDA VACIA"; break;
        case ocupada:   texto = "CELDA OCUPADA"; break;
        default:        texto = ""; break;
    }
    return texto;
}