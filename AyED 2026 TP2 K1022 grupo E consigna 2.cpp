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
    int tiempoSegundos;
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
    int tiempoSegundos;
};

int convertirASegundos(const char llegada[]);
void ordenarLista(CorredorProcesado lista[], int total);

int main() {
    char carpetaRuta[] = "C:/Users/emmanuelp148/Documents/Emmanuel Pilco1/UTN/Algortimos y Estructura de datos/Ejercicios practicos/Archivo corredores 4Refugios.bin";
    char nombreDelArchivo[] = "Archivo corredores 4Refugios.bin";
    char ruta[100];
    strcpy(ruta, carpetaRuta);
    strcat(ruta, nombreDelArchivo);
    FILE* f = fopen(ruta, "rb+");

    if (!f) {
        cout << "No se pudo abrir el archivo principal en la ruta: " << ruta << endl;
        return 1;
    }

    RegCorredores aux;
    while (fread(&aux, sizeof(RegCorredores), 1, f)) {
        if (strcmp(aux.llegada, "DNF") == 0 || strncmp(aux.llegada, "DNF", 3) == 0 || strncmp(aux.llegada, "DNF", 3) == 0) {
            strcpy(aux.llegada, "No termino");
            fseek(f, -sizeof(RegCorredores), SEEK_CUR);
            fwrite(&aux, sizeof(RegCorredores), 1, f);
            fseek(f, 0, SEEK_CUR);
        }
    }

    CorredorProcesado* clasica = new CorredorProcesado[1000];
    CorredorProcesado* nonStop = new CorredorProcesado[1000];
    int cantClasica = 0, cantNonStop = 0;

    rewind(f);
    while (fread(&aux, sizeof(RegCorredores), 1, f)) {
        CorredorProcesado cp;
        cp.datos = aux;
        cp.tiempoSegundos = convertirASegundos(aux.llegada);
        cp.termino = (cp.tiempoSegundos >= 0);

        if (strstr(aux.categoria, "Clasica") != NULL) {
            clasica[cantClasica++] = cp;
        }
        else if (strstr(aux.categoria, "NonStop") != NULL) {
            nonStop[cantNonStop++] = cp;
        }
    }
    fclose(f);

    for (int i = 0; i < cantClasica - 1; i++) {
        for (int j = i + 1; j < cantClasica; j++) {
            bool cambiar = false;

            if (!clasica[i].termino && clasica[j].termino) {
                cambiar = true;
            }
            else if (clasica[i].termino && clasica[j].termino) {
                if (clasica[i].tiempoSegundos < clasica[j].tiempoSegundos) {
                    cambiar = true;
                }
            }

            if (cambiar) {
                CorredorProcesado temp = clasica[i];
                clasica[i] = clasica[j];
                clasica[j] = temp;
            }
        }
    }

    for (int i = 0; i < cantNonStop - 1; i++) {
        for (int j = i + 1; j < cantNonStop; j++) {
            bool cambiar = false;

            if (!nonStop[i].termino && nonStop[j].termino) {
                cambiar = true;
            }
            else if (nonStop[i].termino && nonStop[j].termino) {
                if (nonStop[i].tiempoSegundos < nonStop[j].tiempoSegundos) {
                    cambiar = true;
                }
            }

            if (cambiar) {
                CorredorProcesado temp = nonStop[i];
                nonStop[i] = nonStop[j];
                nonStop[j] = temp;
            }
        }
    }

    
    for (int i = 0; i < cantClasica; i++) {
        int posicionCat = 1;
        for (int j = 0; j < i; j++) {
            if (clasica[j].termino && strcmp(clasica[j].datos.categoria, clasica[i].datos.categoria) == 0) {
                posicionCat++;
            }
        }
        clasica[i].posCat = clasica[i].termino ? posicionCat : 0;
    }

    for (int i = 0; i < cantNonstop; i++) {
        int posicionCat = 1;
        for (int j = 0; j < i; j++) {
            if (nonstop[j].termino && strcmp(nonstop[j].datos.categoria, nonstop[i].datos.categoria) == 0) {
                posicionCat++;
            }
        }
        nonstop[i].posCat = nonstop[i].termino ? posicionCat : 0;
    }


    return 0;
}


int convertirASegundos(const char llegada[]) {
    //...
}

void ordenarLista(CorredorProcesado lista[], int total) {
    //...
}