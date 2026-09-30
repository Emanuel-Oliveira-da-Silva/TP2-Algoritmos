#include <iostream>
#include <cstring>
using namespace std;

struct RegCorredores {
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char llegada[11];
};

struct CorredorProcesado {
    RegCorredores datos;
    int tiempoSegundos;
    int posGeneral;
    int posGenero;
    int posCat;
    int difPrimero;
    int difAnterior;
    bool termino;
};

struct RegPodio {
    char carrera[30];
    char categoria[50];
    int posicionCat;
    int numero;
    char nombreApellido[50];
    int tiempoSegundos;
};

int convertirASegundos(const char llegada[]);
void convertirAStringTiempo(int decSeg, char destino[]);
void ordenarPodio(RegPodio podios[], int cantPodios);
void mostrarPodio(RegPodio podios[], int cantPodios, const char* nomArch);

int main() {
    char carpetaRuta[] = "C:/Users/Matias Pereyra/OneDrive/Documentos/Visual Studio 2022/";
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
                if (clasica[i].tiempoSegundos > clasica[j].tiempoSegundos) {
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
                if (nonStop[i].tiempoSegundos > nonStop[j].tiempoSegundos) {
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

    for (int i = 0; i < cantNonStop; i++) {
        int posicionCat = 1;
        for (int j = 0; j < i; j++) {
            if (nonStop[j].termino && strcmp(nonStop[j].datos.categoria, nonStop[i].datos.categoria) == 0) {
                posicionCat++;
            }
        }
        nonStop[i].posCat = nonStop[i].termino ? posicionCat : 0;
    }

    RegPodio podios[100];
    int cantPodios = 0;

    for (int i = 0; i < cantClasica; i++) {
        if (clasica[i].termino && clasica[i].posCat >= 1 && clasica[i].posCat <= 3) {
            strcpy(podios[cantPodios].carrera, "4 Refugios Clasica");
            strcpy(podios[cantPodios].categoria, clasica[i].datos.categoria);
            podios[cantPodios].posicionCat = clasica[i].posCat;
            podios[cantPodios].numero = clasica[i].datos.numero;
            strcpy(podios[cantPodios].nombreApellido, clasica[i].datos.nombreApellido);
            podios[cantPodios].tiempoSegundos = clasica[i].tiempoSegundos;
            cantPodios++;
        }
    }

    for (int i = 0; i < cantNonStop; i++) {
        if (nonStop[i].termino && nonStop[i].posCat >= 1 && nonStop[i].posCat <= 3) {
            strcpy(podios[cantPodios].carrera, "4 Refugios NonStop");
            strcpy(podios[cantPodios].categoria, nonStop[i].datos.categoria);
            podios[cantPodios].posicionCat = nonStop[i].posCat;
            podios[cantPodios].numero = nonStop[i].datos.numero;
            strcpy(podios[cantPodios].nombreApellido, nonStop[i].datos.nombreApellido);
            podios[cantPodios].tiempoSegundos = nonStop[i].tiempoSegundos;
            cantPodios++;
        }
    }

    ordenarPodio(podios, cantPodios);
    mostrarPodio(podios, cantPodios, "podio.dat");

    delete[] clasica;
    delete[] nonStop;

    return 0;
}


int convertirASegundos(const char llegada[]) {
    if (strcmp(llegada, "No termino") == 0 || strcmp(llegada, "DNF") == 0 || strncmp(llegada, "DNF", 3) == 0 || strncmp(llegada, "DSQ", 3) == 0) {
        return -1;
    }
    int h = (llegada[0] - '0') * 10 + (llegada[1] - '0');
    int m = (llegada[3] - '0') * 10 + (llegada[4] - '0');
    int s = (llegada[6] - '0') * 10 + (llegada[7] - '0');
    int d = (llegada[9] - '0');
    return (h * 3600 + m * 60 + s) * 10 + d;
}

void convertirAStringTiempo(int decSeg, char destino[]) {
    if (decSeg < 0) {
        strcpy(destino, "No Termino");
        return;
    }
    int sTotales = decSeg / 10;
    int d = decSeg % 10;
    int h = sTotales / 3600;
    int m = (sTotales % 3600) / 60;
    int s = sTotales % 60;

    sprintf(destino, "%02d:%02d:%02d.%d", h, m, s, d);
}

void ordenarPodio(RegPodio podios[], int cantPodios) {
    bool cambiar;
    RegPodio temp;

    for (int i = 0; i < cantPodios - 1; i++) {
        for (int j = i + 1; j < cantPodios; j++) {
            cambiar = false;

            if (strcmp(podios[i].carrera, podios[j].carrera) > 0) {
                cambiar = true;
            }
            else if (strcmp(podios[i].carrera, podios[j].carrera) == 0) {
                if (strcmp(podios[i].categoria, podios[j].categoria) > 0) {
                    cambiar = true;
                }
                else if (strcmp(podios[i].categoria, podios[j].categoria) == 0) {
                    if (podios[i].posicionCat > podios[j].posicionCat) {
                        cambiar = true;
                    }
                }
            }

            if (cambiar) {
                temp = podios[i];
                podios[i] = podios[j];
                podios[j] = temp;
            }
        }
    }
}

void mostrarPodio(RegPodio podios[], int cantPodios, const char* nomArch) {
    FILE* f = fopen(nomArch, "wb");
    if (f == NULL) {
        printf("Error al crear el archivo binario");
        return;
    }

    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout << " REPORTE DE PODIOS (TOP 3 POR CATEGORIA)" << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout << "Carrera            | Categoria                                      | Pos | N  | Nombre                  | Tiempo" << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < cantPodios; i++) {
        fwrite(&podios[i], sizeof(RegPodio), 1, f);
        char sTiempo[15];
        convertirAStringTiempo(podios[i].tiempoSegundos, sTiempo);

        cout << podios[i].carrera << " | "
            << podios[i].categoria << " | "
            << podios[i].posicionCat << " | "
            << podios[i].numero << " | "
            << podios[i].nombreApellido << " | "
            << sTiempo << endl;
    }
    fclose(f);
}