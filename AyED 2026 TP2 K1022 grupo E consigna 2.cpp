#include <iostream>
#include <cstring>
using namespace std;


//Estructuras
struct RegCorredores{
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char llegada[11];
};

struct NODO{
    RegCorredores Corredor;
    NODO* next;
};

//Funciones de la Pila
void push(NODO*&,RegCorredores);
RegCorredores pop(NODO*&);

//Funciones del programa


int main(){
    char carpetaRuta[] = "C:/Users/Administrator/Downloads/TP2-Algoritmos-master/TP2-Algoritmos/";
    char nombreDelArchivo[] = "Archivo corredores 4Refugios.bin";
    char ruta[100];
    strcpy(ruta, carpetaRuta);
    strcat(ruta, nombreDelArchivo);
    FILE *f = fopen(ruta, "rb");
    RegCorredores Corredor;
    NODO* PilaCorredores = NULL;

    if(!f){
        cout<< "ERROR 1: El Archivo binario "<< nombreDelArchivo <<"  no se pudo encontrar";
        return 1;
    }

    
    fread(&Corredor,sizeof(RegCorredores),1,f);
    while(!feof(f)) {
        push(PilaCorredores,Corredor);
        fread(&Corredor, sizeof(RegCorredores), 1, f);
    }



    
    return 0;
}

void push(NODO*& Pila, RegCorredores valor){
    NODO* Nuevo = new NODO();
    Nuevo -> Corredor = valor;
    Nuevo ->next = Pila;
    Pila = Nuevo;
}

RegCorredores pop(NODO*& Pila){
    RegCorredores Eliminado = Pila ->Corredor;
    NODO* Temporal = Pila;
    Pila = Temporal -> next;
    delete Temporal;
    return Eliminado;
}