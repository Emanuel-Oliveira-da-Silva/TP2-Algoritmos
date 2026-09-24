#include <iostream>
#include <cstring>
using namespace std;

struct RegCorredores{
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char llegada[11];
};

struct CorredorProcesado{
    RegCorredores datos;
    int segundosTotales;
    int posGeneral;
    int posGenero;
    int posCat;
    int difPrimero;
    int difAnterior;
    bool termino;
};

struct RegPodio{
    char carrera[30];
    char categoria[50];
    int posicionCat;
    int numero;
    char nombreApellido[50];
    int segundosTotales;
};

int convertirASegundos(const char llegada[]);
void ordenarLista(CorredorProcesado lista[], int total);