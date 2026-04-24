#include "reglas_sudoku.h"

#include <cmath>
#include <iostream>


tReglasSudoku::tReglasSudoku() //Constructora básica sin argumentos, incializamos la los contadores a 0
{
    cont = 0;
    lista.cont = 0;
    valores_celda.nFilas = 0;
    valores_celda.nColumnas = 0;

    ini_matriz3D();
}

int tReglasSudoku::dame_dimension() const
{
    return tablero.dame_dimension();
}

int tReglasSudoku::dame_celda(int g, int c) const
{
    return tablero.dame_elem(g, c).dame_valor();
}

bool tReglasSudoku::terminado() const
{
    return dame_dimension() * dame_dimension() == cont;
}

bool tReglasSudoku::bloqueo() const
{
    return lista.cont != 0;
}

int tReglasSudoku::dame_num_celdas_bloqueadas() const
{
    return lista.cont;
}

int tReglasSudoku::dame_num_celdas_vacias() const
{
    return dame_dimension() * dame_dimension() - cont;
}

void tReglasSudoku::dame_celda_bloqueada(int p, int &f, int &c) const
{
    if (p >= 0 && p < lista.cont) //Comrpobamos que la posición esté dentro de los márgenes
    {
        f = lista.lista[p][0]; //Añadimos en la lista la fila y columna del bloqueo
        c = lista.lista[p][1];
    }
    else
    { //Si la posicion no es válida retornamos por valor -1;
        f = -1;
        c = -1;
    }
}

bool tReglasSudoku::coord_valid(const int& f, const int&c) const{
    int dim = dame_dimension();
    return f >= 0 && c >= 0 && f < dim && c < dim;
}

void tReglasSudoku::actualizar_bloqueos(){
    int dim = dame_dimension();
    lista.cont = 0;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (tablero.dame_elem(i, j).es_vacia() && posibles_valores(i, j) == 0)
            {
                if (lista.cont < MAX * MAX)
                {
                    lista.lista[lista.cont][0] = i;
                    lista.lista[lista.cont][1] = j;
                    lista.cont++;
                }
            }
        }
    }
}

bool tReglasSudoku::es_valor_posible(int f, int c, int v) const
{
    int dim = dame_dimension();

    bool posible = true;

    if(coord_valid(f, c)){

        if (v < 1 || v > dim || !tablero.dame_elem(f, c).es_vacia()) // Comrprobamos que el valor a introducir esté entre los posibles y que la celda no esté rellena o sea una celda original
            posible = false;

       if(posible && !valores_celda.valores[f][c][v-1].posible) posible = false;

    } else posible = false;

    return posible;
}

int tReglasSudoku::posibles_valores(int f, int c, int valores[]) const
{

    int num_posibles = 0; //Contador de valores posibles
    if(coord_valid(f, c)){
        if (tablero.dame_elem(f, c).es_vacia())
        {
            for (int v = 1; v <= dame_dimension(); v++)
            {
                if (es_valor_posible(f, c, v)) //Iteramos por cada valor posible (lista [1 - DIM])
                {
                    if (valores != nullptr) //Comprobamos que el puntero al array no sea NULL
                        valores[num_posibles] = v; //Añadimos el nuevo valor a la lista
                    num_posibles++;
                }
            }
        }
    }

    return num_posibles;
}


void tReglasSudoku::autocompletar()
{
    int dim = dame_dimension();
  
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            tCelda c = tablero.dame_elem(i, j); //Recibimos el elemento que estamos revisando

            if (c.es_vacia()){
                    
                //Inicializamos el contador de valores 
                int valores[MAX] = {0};

                //Recibimos el número de valores posible y por referencia un listado con ellos
                int numPosibles = posibles_valores(i, j, valores);

                if (numPosibles == 1) pon_valor(i, j, valores[0]);
                
            }
        }
    }
}

bool tReglasSudoku::pon_valor(int f, int c, int v)
{
    bool colocado = false;

    if(coord_valid(f, c)){
        tCelda celda = tablero.dame_elem(f, c); // Recibimos la celda a modificar
        if(celda.es_vacia() && es_valor_posible(f, c, v)){ //Comprobamos que esté vacía y que el valor sea posible

            celda.set_valor(v);
            celda.set_ocupada();
            tablero.colocar_celda(f, c, celda);
            cont++;
            actualizar_posibles(f, c, v);
            colocado = true;
        }

    }

    //En caso de haber colocado correctamente el valor actualizamos la lista de bloqueos
    if(colocado) actualizar_bloqueos();
    
    return colocado;
}

bool tReglasSudoku::quita_valor(int f, int c)
{
    bool quitado = false;

    if(coord_valid(f, c)){
        tCelda celda = tablero.dame_elem(f, c); // Recibimos la celda a modificar

        if(celda.es_ocupada()){ //Comprobamos que la celda esté ocupada
            int val = celda.dame_valor();

            celda.set_valor(0);
            celda.set_vacia();
            tablero.colocar_celda(f, c, celda);
            cont--;
            anadir_posibles(f, c, val);
            quitado = true;
        }

        //Si hemos quitado la celda actualizamos los bloqueos
        if(quitado) actualizar_bloqueos();

    }

    return quitado;
} // pone la celda (f,c) a VACIA

void tReglasSudoku::reset()
{
    //Iteramos todo el sudoku en busca de celdas no originales y las restauramos a 0
    for (int i = 0; i < dame_dimension(); i++)
    {
        for (int j = 0; j < dame_dimension(); j++)
        {
            if (!tablero.dame_elem(i, j).es_original())
                quita_valor(i, j);
        }
    }
}

bool tReglasSudoku::carga_sudoku(ifstream &input)
{
    bool cargado = false;
    tCelda c;

    cont = 0;
    lista.cont = 0;


    int dim;
    input >> dim;

    
    //MODIFICADO
    valores_celda.nColumnas = dim;
    valores_celda.nFilas = dim;

    ini_matriz3D();

    cargado = (dim == 4 || dim == 9 );

    if(cargado){
        tablero = tTablero(dim);

        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                int v;
                input >> v;
                if (v != 0)
                {

                    c.set_valor(v);
                    c.set_original();
                    tablero.colocar_celda(i, j, c);
                    cont++;

                    //MODIFICADO
                    actualizar_posibles(i, j, v);
                }
                else
                {
                    c.set_valor(0);
                    c.set_vacia();
                    tablero.colocar_celda(i, j, c);
                }

            }
        }

        actualizar_bloqueos();
    }

    return cargado;
}

void tReglasSudoku::actualizar_posibles(const int& f, const int& c, const int& valor){

    bool visitadas[MAX][MAX] = {};

    int dim = dame_dimension();

    if(coord_valid(f,c) && valor >= 1 && valor <= dim){
        int idx = valor - 1;

        for (int i = 0; i < valores_celda.nColumnas; i++) //Iteramos la columna 
        {
            if(!visitadas[f][i]){
                quitar_posible(f, i, idx);
                visitadas[f][i] = true;
            }
        }

        for (int j = 0; j < valores_celda.nFilas; j++) //Iteramos la fila
        {
            if(!visitadas[j][c]){
                quitar_posible(j, c, idx);
                visitadas[j][c] = true;
            }
        }

        //Iteramos la submatriz
        int dim_submatriz = int(sqrt(double(dim)));
        int startF = (f / dim_submatriz) * dim_submatriz;
        int startC = (c / dim_submatriz) * dim_submatriz;

        for (int i = startF; i < startF+dim_submatriz; i++)
        {
            for (int j = startC; j < startC+dim_submatriz; j++)
            {
                if(!visitadas[i][j]){
                    quitar_posible(i, j, idx);
                    visitadas[i][j] = true;
                }
            }
            
        }
    }  
}

void tReglasSudoku::quitar_posible(const int& f, const int& c, const int val){
    //Marcamos el valor como no posible y aumentalos las celdas que lo afectan en 1
    valores_celda.valores[f][c][val].posible = false;
    valores_celda.valores[f][c][val].celdas_que_afectan++;

}

void tReglasSudoku::anadir_posibles(const int& f, const int& c, const int& valor){

    bool visitadas[MAX][MAX] = {};

    int dim = dame_dimension();

    if(coord_valid(f,c) && valor >= 1 && valor <= dim){
        int idx = valor - 1;

        //Iteramos la columna
        for (int i = 0; i < valores_celda.nColumnas; i++)
        {
            if(!visitadas[f][i]){
                if (valores_celda.valores[f][i][idx].celdas_que_afectan > 0) //Comprobamos si le afecta alguna celda
                    valores_celda.valores[f][i][idx].celdas_que_afectan--; //Reducimos ese número

                valores_celda.valores[f][i][idx].posible =
                    (valores_celda.valores[f][i][idx].celdas_que_afectan == 0); //En caso de que ya no le afecte ninguna celda, establecemos ese valor como posible
                visitadas[f][i] = true;
            }
        }

        //Iteramos columnas
        for (int j = 0; j < valores_celda.nFilas; j++)
        {
            if(!visitadas[j][c]){
                if (valores_celda.valores[j][c][idx].celdas_que_afectan > 0) //Comprobamos si le afecta alguna celda
                    valores_celda.valores[j][c][idx].celdas_que_afectan--; //Reducimos ese número

                valores_celda.valores[j][c][idx].posible =
                    (valores_celda.valores[j][c][idx].celdas_que_afectan == 0); //En caso de que ya no afecte ninguna celda, establecemos ese valor como posible
                
                    visitadas[j][c] = true;
            
            }
        }

        //Iteramos la submatriz
        int dim_submatriz = int(sqrt(double(dim)));
        int startF = (f / dim_submatriz) * dim_submatriz;
        int startC = (c / dim_submatriz) * dim_submatriz;

        for (int i = startF; i < startF+dim_submatriz; i++)
        {
            for (int j = startC; j < startC+dim_submatriz; j++)
            {
                if(!visitadas[i][j]){    
                    //Comprobamos si le afecta alguna celda
                    if (valores_celda.valores[i][j][idx].celdas_que_afectan > 0)
                        valores_celda.valores[i][j][idx].celdas_que_afectan--; //Reducimos ese número

                    valores_celda.valores[i][j][idx].posible =
                        (valores_celda.valores[i][j][idx].celdas_que_afectan == 0); //En caso de que ya no afecte ninguna celda, establecemos ese valor como posible
                    visitadas[i][j] = true;
                }
            }
            
        }
    }  
}

void tReglasSudoku::ini_matriz3D(){
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            for (int v = 0; v < MAX; v++)
            {
                valores_celda.valores[i][j][v].posible = true;
                valores_celda.valores[i][j][v].celdas_que_afectan = 0;
            }
        }
    }
}