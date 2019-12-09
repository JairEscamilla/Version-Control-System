
/*
 * @author: César Mauricio Arellano Velásquez, Allan Jair Escamilla Hernández, Raúl González Portillo.
 * @date:   25/Noviembre/2019
 * @file:   state_machine.c
 * @brief:  Implementación de máquinas de estados para el sistema controlador de versiones.
 */
#ifndef state_machine_h
#define state_machine_h 

// Importamos las biliotecas
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include <dirent.h> 

// Definimos las etiquetas de los estados de la maquina
typedef enum stateType{
    LOAD_USERS,
    MENU,
    CREAR_REPOSITORIO,
    CREAR_RAMA,
    COMMIT,
    VER_COMMITS,
    VER_REPOSITORIOS,
    REVERT, 
    PUSH,
    PULL_REQUEST,
    VER_PULLS,
    ACTUALIZAR_FILES,
    CREAR_USER,
    EXIT,
    STATE_NUM
}State;

// Definimos la estructura que contendrá la funcion a ejecutar junto a su label

typedef struct defEstado{
    State state;
    void (*process)(State* state);
}Estados;

typedef struct defUser{
    char user[200];
    char pwd[200];
}User;

typedef struct defCommit{
    int id;
    char branch[100], fechayhora[200];
    time_t time;
    User user;
    char descripcion[200];
}Commit;
// Prototipos de las funciones que ejecutara la maquina de estados
void load_users(State* state);
void menu(State* state);
void salir(State* state);
void crear_repositorio(State* state);
void commit(State* state);
void cambiar_rama(State* state);
void ver_commits(State* state);
void merge(State* state);
void ver_repositorios(State* state);
void crear_rama(State* state);
void revert(State* state);
void push(State* state);
void pull(State* state);
void ver_pulls(State* state);
void actualizar(State* state);
void crear_user(State* state);

User logger(int* flag);
int loggerCommit(char* descripcion, char* repositorio, char branch[]);
void make_commit(User temp, char* descripcion, char* repositorio, char branch[]);
void mover_archivos(int id, char* repositorio, Commit commit);
int buscar_repositorios(char* repositorio);
void buscarCommit(char* id, char* repositorio, char direccion[]);
void mergeFiles(char direccion[], char direccion2[], char repositorio[]);
int buscarArchivos(char* array[], int counter, char buscado[]);

#endif // !state_machine_h