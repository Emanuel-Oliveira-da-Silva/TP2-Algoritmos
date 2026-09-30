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

struct PodioCategoria{
    char Categoria[50];
    RegCorredores top5[5];
};

struct NODOCATEGORIA{
    PodioCategoria podio;
    NODOCATEGORIA* next;
};

//Funciones de la consigna anterior
int convertirASegundos(char llegada[]);

//Funciones de las Pilas
void push(NODO*&,RegCorredores);
RegCorredores pop(NODO*&);
void pushCategoria(NODOCATEGORIA*&, PodioCategoria);
PodioCategoria popCategoria(NODOCATEGORIA*&);

//Funciones del programa
void crearPodio(NODOCATEGORIA*&, RegCorredores);
void procesarCorredor(PodioCategoria, RegCorredores);
void crearCategorias(NODO*&, NODOCATEGORIA*&);

int main(){
    char carpetaRuta[] = "C:/Users/Administrator/Downloads/TP2-Algoritmos-master/TP2-Algoritmos/";
    char nombreDelArchivo[] = "Archivo corredores 4Refugios.bin";
    char ruta[100];
    strcpy(ruta, carpetaRuta);
    strcat(ruta, nombreDelArchivo);
    FILE *f = fopen(ruta, "rb");
    RegCorredores TamanioCorredor;
    NODO* PilaCorredores = NULL;

    // Verificar si el archivo existe y cargarlo en la pila
    if(!f){
        cout<< "ERROR 1: El Archivo binario "<< nombreDelArchivo <<"  no se pudo encontrar";
        return 1;
    }
    
    fread(&TamanioCorredor,sizeof(RegCorredores),1,f);
    while(!feof(f)) {
        push(PilaCorredores,TamanioCorredor);
        fread(&TamanioCorredor, sizeof(RegCorredores), 1, f);
    }

    
    //Cargar las categorias existentes de los corredores en una pila, cada nodo es una categoria
    //Al mismo tiempo se comparan los tiempos
    NODOCATEGORIA* PilaPodios = NULL;

    //Crear las Categorias y sus Podios con sus respectivos corredores
    crearCategorias(PilaCorredores,PilaPodios);


    //Imprimir los podios de cada categoria
    while(PilaPodios != NULL){
        PodioCategoria Podio = popCategoria(PilaPodios);
        cout << "Categoria: " << Podio.Categoria << endl;
        cout << "Top 5 Corredores:" << endl;
        for(int i=0; i<5; i++){
            if(convertirASegundos(Podio.top5[i].llegada) != -1){
                cout << i+1 << ". " << Podio.top5[i].nombreApellido << " - Llegada: " << Podio.top5[i].llegada << endl;
            }
        }
        cout << endl;
    }
    
    
    return 0;
}

//Funciones de la consigna anterior
int convertirASegundos(char llegada[]){
    if(strcmp(llegada, "No termino")==0 || strcmp(llegada, "DNF") == 0 || strncmp(llegada, "DNF", 3) == 0 || strncmp(llegada, "DSQ", 3) == 0){
        return -1;
    }
    int h = (llegada[0] - '0') * 10 + (llegada[1] - '0');
    int m = (llegada[3] - '0') * 10 + (llegada[4] - '0');
    int s = (llegada[6] - '0') * 10 + (llegada[7] - '0');
    int d = (llegada[9] - '0');
    return (h * 3600 + m * 60 + s) * 10 + d;
}


//Funciones de las Pilas
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

void pushCategoria(NODOCATEGORIA*& Pila, PodioCategoria valor){
    NODOCATEGORIA* Nuevo = new NODOCATEGORIA();
    Nuevo ->podio= valor;
    Nuevo ->next = Pila;
    Pila = Nuevo;
}

PodioCategoria popCategoria(NODOCATEGORIA*& Pila){
    PodioCategoria Eliminado = Pila ->podio;
    NODOCATEGORIA* Temporal = Pila;
    Pila = Temporal ->next;
    delete Temporal;
    return Eliminado;
}


//Funciones del programa
void crearPodio(NODOCATEGORIA*& Pila, RegCorredores Corredor){
    PodioCategoria nuevo;
    strcpy(nuevo.Categoria, Corredor.categoria);
    for(int i=0;i<5;i++){
        nuevo.top5[i].numero = 0;
        strcpy(nuevo.top5[i].nombreApellido, "");
        strcpy(nuevo.top5[i].categoria, "");
        nuevo.top5[i].genero = '\0';
        strcpy(nuevo.top5[i].localidad, "");
        strcpy(nuevo.top5[i].llegada, "DNF");
    }
    procesarCorredor(nuevo, Corredor);
    pushCategoria(Pila,nuevo);
}

void procesarCorredor(PodioCategoria Categoria, RegCorredores Corredor){
    RegCorredores* Podio = Categoria.top5;

    //Verificar si el corredor termino la carrera
    if (convertirASegundos(Corredor.llegada) == -1) {
        return; // No entra si no terminó
    }
    else{

        //Verificar si el corredor puede entrar al podio
        for(int i=0; i<5; i++){
            //Verificar si el lugar está vacío
            if(convertirASegundos(Podio[i].llegada) == -1){
                Podio[i] = Corredor;
                return;                
            }

            //Verificar si el corredor es más rápido que Podio[i]
            if(convertirASegundos(Corredor.llegada) < convertirASegundos(Podio[i].llegada)){
                //Mover los corredores más lentos hacia abajo
                for(int j=4; j>i; j--){
                    Podio[j] = Podio[j-1];
                }
                //Insertar Corredor en el Podio
                Podio[i] = Corredor;
                return;
            }
        }
    }
}

void crearCategorias(NODO*& PilaCorredores, NODOCATEGORIA*& PilaPodios){
    while(PilaCorredores != NULL){

        RegCorredores Corredor = pop(PilaCorredores);

        if(PilaPodios == NULL){
            //agregar categoria y ver si el corredor puede entrar al podio
            crearPodio(PilaPodios,Corredor);
        }
        else{
            //Buscar categoria, Si no existe, crearla y su respectivo Podio
            
            //Pila auxiliar para no perder los podios al hacer pop
            NODOCATEGORIA* PilaAuxiliar = NULL;
            
            while(PilaPodios != NULL){
                PodioCategoria Podio = popCategoria(PilaPodios);
            
                if(strcmp(Podio.Categoria,Corredor.categoria) == 0){

                    //Agregarlo al Podio de esa categoria de manera ordenada
                    procesarCorredor(Podio,Corredor);

                    //devolver la categoria a la pila para evitar que PilaPodios quede vacía
                    pushCategoria(PilaPodios,Podio);
                    break;
                }
                else{
                    //Poner "Podio" en la PilaAuxiliar y seguir el ciclo
                    pushCategoria(PilaAuxiliar,Podio);
                }
            }
            //Si PilaPodios está vacía, significa que no fue encontrada ninguna categoria
            if(PilaPodios == NULL){
                //Crear categoria y poner al corredor en el podio
                crearPodio(PilaPodios,Corredor);
            }

            // Devolver todo lo puesto en PilaAuxiliar a PilaPodios despues de ya acomodar el corredor donde va
                while(PilaAuxiliar!=NULL){
                    pushCategoria(PilaPodios,popCategoria(PilaAuxiliar));
                }
        }
    }
}

