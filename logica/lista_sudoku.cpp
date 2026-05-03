#include "lista_sudoku.h"


// ==================== CONSTRUCTORES Y DESTRUCTORES ====================

ListaSudoku::ListaSudoku() : size(0) , cap(0), lista_sudokus(nullptr) {}

ListaSudoku::~ListaSudoku()
{

    for (int i = 0; i < size; i++)
    {
        delete lista_sudokus[i];
        lista_sudokus[i] = nullptr;
    }

    delete[] lista_sudokus;
    lista_sudokus = nullptr;
    
}

ListaSudoku::ListaSudoku(const ListaSudoku& lista){
    size = lista.size;
    cap = lista.cap;

    if(cap>0){
		// Copia profunda de los punteros a tReglasSudoku
        lista_sudokus = new tReglasSudoku*[cap];
        for (int i = 0; i < size; i++)
        {
            lista_sudokus[i] = new tReglasSudoku(lista[i]);
        }

        for (int i = size; i < cap; i++)
        {
            lista_sudokus[i] = nullptr;
        }
    } else {
        lista_sudokus = nullptr;
    }
}


// ==================== MÉTODOS CONSULTORES ====================

int ListaSudoku::dame_num_elems() const {return size;}

const tReglasSudoku& ListaSudoku::dame_sudoku(const int& i){
    return *lista_sudokus[i];
}


// ==================== MÉTODOS MODIFICADORES ====================

void ListaSudoku::insertar(const tReglasSudoku& sudoku) {
    if (size == cap) {
        int newCap = (cap == 0) ? 1 : cap * 2;
        resize_(newCap);
    }

    // Encontrar posición ordenada por número de celdas vacías (menor dificultad primero)
    int pos = size;
    bool encontrado = false;
    int i = 0;
    while (i < size && !encontrado) {
        if (sudoku < *lista_sudokus[i]) {
            pos = i;
            encontrado = true;
        }
        i++;
	}

    // Desplazar elementos a la derecha desde pos
    for (int i = size; i > pos; i--)
    {
        lista_sudokus[i] = lista_sudokus[i - 1];
    }

    // Insertar en posición ordenada
    lista_sudokus[pos] = new tReglasSudoku(sudoku);
    size++;
    
}

void ListaSudoku::eliminar(const int& pos){
    if(pos >= 0 && pos < size){

        delete lista_sudokus[pos];
        lista_sudokus[pos] = nullptr;
        for (int i = pos; i < size - 1; i++)
        {
            lista_sudokus[i] = lista_sudokus[i+1];
        }

        lista_sudokus[size-1] = nullptr;

        size--;
    } // Validar que pos es válido
    
} // elimina el elemento de la posición pos


// ==================== OPERADORES ====================

const tReglasSudoku& ListaSudoku::operator[](int indice) const{
    return *lista_sudokus[indice];
}

tReglasSudoku& ListaSudoku::operator[](int indice) {
    return *lista_sudokus[indice];
}

ListaSudoku& ListaSudoku::operator=(const ListaSudoku& lista) {
	// Creamos una copia local de 'lista' (que se construirá con el constructor de copia).
    ListaSudoku copia(lista);

	// Intercambiamos los miembros de datos de 'this' con los de 'copia'
    int temp_size = size;
    size = copia.size;
    copia.size = temp_size;

    int temp_cap = cap;
    cap = copia.cap;
    copia.cap = temp_cap;

	// Intercambiamos los punteros a las listas de sudokus
    tReglasSudoku** temp_lista = lista_sudokus;
    lista_sudokus = copia.lista_sudokus;
    copia.lista_sudokus = temp_lista;

	// Al salir de esta función, 'copia' se destruirá automáticamente y liberará la memoria que originalmente pertenecía a 'this', mientras que 'this' ahora apunta a la nueva lista de sudokus copiada desde 'lista'.

    
    return *this;
}

// Para comparar dos listas de sudokus, se puede usar el número de celdas vacías del primer sudoku en cada lista como criterio de ordenación.
bool ListaSudoku::operator<(const ListaSudoku& otra) const {
    bool resultado;
    if (size == 0 || otra.size == 0) {
        resultado = size < otra.size;
    } else {
        int vacios_este = (*this)[0].dame_num_celdas_vacias();
        int vacios_otro = otra[0].dame_num_celdas_vacias();
        resultado = vacios_este < vacios_otro;
    }
    return resultado;
}

bool ListaSudoku::operator==(const ListaSudoku& otra) const {
    bool resultado = true;
    if (size != otra.size) {
        resultado = false;
    } else {
        int i = 0;
        while (i < size && resultado) {
            if ((*this)[i].dame_num_celdas_vacias() != otra[i].dame_num_celdas_vacias()) {
                resultado = false;
            }
            i++;
        }
    }
    return resultado;
}