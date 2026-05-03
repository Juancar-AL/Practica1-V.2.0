#include "reglas_sudoku.h"

#include "../checkML.h"

#include <cmath>
#include <iostream>


tReglasSudoku::tReglasSudoku() //Constructora básica sin argumentos, incializamos la los contadores a 0
{
    cont = 0;
    lista.cont = 0;
    lista.cap = 0;
    lista.lista = nullptr;
    valores_celda.nFilas = 0;
    valores_celda.nColumnas = 0;

    for (int i = 0; i < MAX; i++) {
        cuantas_celdas[i] = 0;
    }

    ini_matriz3D();
}

//Constructor por copia, realizamos una copia profunda de la lista de bloqueos y una copia simple del resto de miembros
tReglasSudoku::tReglasSudoku(const tReglasSudoku& otro) {
    cont = otro.cont;
    tablero = otro.tablero;
    valores_celda = otro.valores_celda;
    
    lista.cont = otro.lista.cont;
    lista.cap = otro.lista.cap;
    
    if (otro.lista.cap > 0) {
        lista.lista = new tPosicion*[otro.lista.cap];
        for (int i = 0; i < otro.lista.cont; i++) {
            lista.lista[i] = new tPosicion{otro.lista.lista[i]->fila, otro.lista.lista[i]->columna};
        }

        for (int i = otro.lista.cont; i < otro.lista.cap; i++)
        {
            lista.lista[i] = nullptr;
        }
    } else {
        lista.lista = nullptr;
    }

    for (int i = 0; i < MAX; i++) {
        cuantas_celdas[i] = otro.cuantas_celdas[i];
    }
}

//Destructora, liberamos la memoria de la lista de bloqueos y ponemos a nullptr el puntero y el contador a 0
tReglasSudoku::~tReglasSudoku(){
    for (int i = 0; i < lista.cont; i++)
    {
        delete lista.lista[i];
        lista.lista[i] = nullptr;
    }

    delete[] lista.lista;
    lista.lista = nullptr;
    
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
        f = lista.lista[p]->fila; //Añadimos en la lista la fila y columna del bloqueo
        c = lista.lista[p]->columna;
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

    for (int k = 0; k < lista.cont; k++)
    {
        delete lista.lista[k];
		lista.lista[k] = nullptr;
    }

    lista.cont = 0;

    // Asegurar consistencia: si hay capacidad pero no array, crear array vacío
    if (lista.cap > 0 && lista.lista == nullptr) {
        lista.lista = new tPosicion*[lista.cap];
        for (int k = 0; k < lista.cap; k++) lista.lista[k] = nullptr;
    }

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (tablero.dame_elem(i, j).es_vacia())
            {
                int posibles = posibles_valores(i, j); // usa la versión con default nullptr
                if (posibles == 0)
                {
                    if (lista.cont == lista.cap)
                    {
                        int newCap = (lista.cap == 0) ? 1 : lista.cap * 2;
                        tPosicion ** nuevo = new tPosicion*[newCap];
                        // copiar elementos existentes (si los hay)
                        for (int k = 0; k < lista.cont; k++)
                            nuevo[k] = lista.lista ? lista.lista[k] : nullptr;
                        // inicializar resto a nullptr
                        for (int k = lista.cont; k < newCap; k++)
                            nuevo[k] = nullptr;
						// liberar memoria antigua
                        delete[] lista.lista;
                        lista.lista = nuevo;
                        lista.cap = newCap;
                    }

                    lista.lista[lista.cont] = new tPosicion{i, j};
                    lista.cont++;
                }
            }
        }
    }
}

bool tReglasSudoku::es_valor_posible(int f, int c, int v) const {
    int dim = dame_dimension();
    bool posible = false; // Asumimos false por defecto

	// Verificamos que las coordenadas sean válidas, el valor esté dentro del rango permitido, la celda esté vacía y que el valor sea posible según nuestra matriz de posibilidades.
    if (coord_valid(f, c) && 
        v >= 1 && v <= dim && 
        tablero.dame_elem(f, c).es_vacia() && 
        valores_celda.valores[f][c][v-1].posible) 
    {
        posible = true;
    }

    return posible;
}

int tReglasSudoku::posibles_valores(int f, int c, int valores[]) const
{
    int num_posibles = 0; //Contador de valores posibles
    if(coord_valid(f, c) && tablero.dame_elem(f, c).es_vacia()){

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

    recalcular_cuantas_celdas();
}

bool tReglasSudoku::pon_valor(int f, int c, int v)
{
    bool colocado = false;

    if(coord_valid(f, c) && tablero.dame_elem(f, c).es_vacia()  && es_valor_posible(f, c, v)){
        tCelda celda = tablero.dame_elem(f, c); // Recibimos la celda a modificar

        celda.set_valor(v);
        celda.set_ocupada();
        tablero.colocar_celda(f, c, celda);
        cont++;
        actualizar_posibles(f, c, v);
        colocado = true;

        recalcular_cuantas_celdas();

    }

    //En caso de haber colocado correctamente el valor actualizamos la lista de bloqueos
    if(colocado) actualizar_bloqueos();
    
    return colocado;
}

bool tReglasSudoku::quita_valor(int f, int c)
{
    bool quitado = false;

    if(coord_valid(f, c) && tablero.dame_elem(f,c).es_ocupada()){
        tCelda celda = tablero.dame_elem(f, c); // Recibimos la celda a modificar

        int val = celda.dame_valor();

        celda.set_valor(0);
        celda.set_vacia();
        tablero.colocar_celda(f, c, celda);
        cont--;
        anadir_posibles(f, c, val);
        quitado = true;

        recalcular_cuantas_celdas();
        actualizar_bloqueos();
    
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

    for (int i = 0; i < lista.cont; i++)
    {
        delete lista.lista[i];
        lista.lista[i] = nullptr;

    }

    cont = 0;
    lista.cont = 0;


    int dim;
    input >> dim;

    
	//MODIFICADO - Inicializamos la matriz de posibilidades con la dimensión del sudoku que vamos a cargar, para evitar problemas de acceso a índices fuera de rango.
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

					//MODIFICADO - Actualizamos los posibles valores de las celdas afectadas por el valor que acabamos de colocar.
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

        recalcular_cuantas_celdas();
        
    }

    return cargado;
}

void tReglasSudoku::quitar_posible(const int& f, const int& c, const int val){
    //Marcamos el valor como no posible y aumentalos las celdas que lo afectan en 1
    valores_celda.valores[f][c][val].posible = false;
    valores_celda.valores[f][c][val].celdas_que_afectan++;

}

void tReglasSudoku::restaurar_posible(const int& f, const int& c, const int val) {
    // Si le afecta alguna celda, reducimos ese número
    if (valores_celda.valores[f][c][val].celdas_que_afectan > 0) {
        valores_celda.valores[f][c][val].celdas_que_afectan--;
    }
    // Si ya no le afecta ninguna celda, lo establecemos como posible
    valores_celda.valores[f][c][val].posible = 
        (valores_celda.valores[f][c][val].celdas_que_afectan == 0);
}

void tReglasSudoku::modificar_afectados(const int& f, const int& c, const int& valor, bool quitar) {
    bool visitadas[MAX][MAX] = {};
    int dim = dame_dimension();

    if (coord_valid(f, c) && valor >= 1 && valor <= dim) {
        int idx = valor - 1;

        // 1. Iteramos la columna
        for (int i = 0; i < valores_celda.nColumnas; i++) {
            if (!visitadas[f][i]) {
                if (quitar) quitar_posible(f, i, idx);
                else restaurar_posible(f, i, idx);
                visitadas[f][i] = true;
            }
        }

        // 2. Iteramos la fila
        for (int j = 0; j < valores_celda.nFilas; j++) {
            if (!visitadas[j][c]) {
                if (quitar) quitar_posible(j, c, idx);
                else restaurar_posible(j, c, idx);
                visitadas[j][c] = true;
            }
        }

        // 3. Iteramos la submatriz
        int dim_submatriz = int(sqrt(double(dim)));
        int startF = (f / dim_submatriz) * dim_submatriz;
        int startC = (c / dim_submatriz) * dim_submatriz;

        for (int i = startF; i < startF + dim_submatriz; i++) {
            for (int j = startC; j < startC + dim_submatriz; j++) {
                if (!visitadas[i][j]) {
                    if (quitar) quitar_posible(i, j, idx);
                    else restaurar_posible(i, j, idx);
                    visitadas[i][j] = true;
                }
            }
        }
    }
}

void tReglasSudoku::actualizar_posibles(const int& f, const int& c, const int& valor) {
    modificar_afectados(f, c, valor, true); // true = queremos quitar los posibles
}

void tReglasSudoku::anadir_posibles(const int& f, const int& c, const int& valor) {
    modificar_afectados(f, c, valor, false); // false = queremos restaurar los posibles
}

// Inicializamos la matriz de posibilidades, marcando todos los valores como posibles y sin celdas que los afecten.
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

tReglasSudoku& tReglasSudoku::operator=(const tReglasSudoku& otro) {
    if (this != &otro) {
        // Liberamos memoria actual
        for (int i = 0; i < lista.cont; i++) {
            delete lista.lista[i];
        }
        delete[] lista.lista;
        
        // Copiar datos simples
        cont = otro.cont;
        tablero = otro.tablero;
        valores_celda = otro.valores_celda;
        
        // Copiar lista profundamente
        lista.cont = otro.lista.cont;
        lista.cap = otro.lista.cap;
        
        if (otro.lista.cap > 0) {
            lista.lista = new tPosicion*[otro.lista.cap];
            for (int i = 0; i < otro.lista.cont; i++) {
                lista.lista[i] = new tPosicion{otro.lista.lista[i]->fila, otro.lista.lista[i]->columna};
            }
            for (int i = otro.lista.cont; i < otro.lista.cap; i++)
            {
                lista.lista[i] = nullptr;
            }
        } else {
            lista.lista = nullptr;
        }

        for (int i = 0; i < MAX; i++) {
            cuantas_celdas[i] = otro.cuantas_celdas[i];
        }
    }
    return *this;
}

// Retorna la cantidad de celdas que pueden tener exactamente n_valores posibles. Si n_valores no es válido, retorna 0.
int tReglasSudoku::cuantas_celdas_pueden_tener(int n_valores) const{
    int cantidad = 0;

    if (n_valores >= 1 && n_valores <= dame_dimension()) {
        cantidad = cuantas_celdas[n_valores - 1];
    }

    return cantidad;
}

void tReglasSudoku::recalcular_cuantas_celdas(){
    for (int i = 0; i < MAX; i++) {
        cuantas_celdas[i] = 0;
    }
    
    // Guardamos la dimensión en una variable local para no llamar a la función 81 veces.
    int dim = dame_dimension(); 

    // 2. Recorremos el tablero UNA sola vez.
    for (int fila = 0; fila < dim; fila++) {
        for (int columna = 0; columna < dim; columna++) {
            
            if (dame_celda(fila, columna) == 0) {
                // Calculamos sus valores posibles una única vez por celda
                int num_posibles = posibles_valores(fila, columna);
                
                // Si tiene valores posibles (y está en rango), incrementamos su contador
                if (num_posibles > 0 && num_posibles <= dim) {
                    cuantas_celdas[num_posibles - 1]++;
                }
            }
        }
    }
}

// Un sudoku se considera "menor" que otro si tiene menos celdas vacías. Si ambos tienen el mismo número de celdas vacías, se compara la cantidad de celdas con 1 posible valor, luego con 2 posibles valores, etc. hasta encontrar una diferencia o agotar todas las posibilidades.
bool tReglasSudoku::operator<(const tReglasSudoku& s2) const{
    bool resultado = false;
    bool decidido = false;

    if (dame_num_celdas_vacias() != s2.dame_num_celdas_vacias()) {
		resultado = (dame_num_celdas_vacias() < s2.dame_num_celdas_vacias());
        decidido = true;
    }
    else {
        int dim = dame_dimension();

        int n = 1;
        while(n <= dim && !decidido) {
            int c1 = cuantas_celdas_pueden_tener(n);
            int c2 = s2.cuantas_celdas_pueden_tener(n);
            if (c1 != c2) {
                // Más celdas muy restringidas => sudoku más fácil => "menor".
                resultado = (c1 > c2);
                decidido = true;
            }
            n++;
		}
    }
    return resultado;
}

// Dos sudokus se consideran iguales si tienen la misma dimensión y la misma cantidad de celdas con 1 posible valor, 2 posibles valores, etc. (es decir, la misma distribución de posibilidades), independientemente de los valores específicos en el tablero.
bool tReglasSudoku::operator==(const tReglasSudoku& s2) const{
    bool iguales = true;

    if (dame_num_celdas_vacias() != s2.dame_num_celdas_vacias()) {
        iguales = false;
    }
    else {
        int dim = dame_dimension();

        int n = 1;
        while(n <= dim && iguales) {
            if (cuantas_celdas_pueden_tener(n) != s2.cuantas_celdas_pueden_tener(n)) {
                iguales = false;
            }
            n++;
		}
    }

    return iguales;
}