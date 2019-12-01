
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

// Definimos las etiquetas de los estados de la maquina
typedef enum stateType{
    LOAD_USERS,
    MENU,
    CREAR_REPOSITORIO,
    COMMIT,
    CAMBIAR_DE_RAMA, 
    VER_COMMITS,
    MERGE,
    AGREGAR_USUARIOS,
    VER_REPOSITORIOS,
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
// Prototipos de las funciones que ejecutara la maquina de estados
void load_users(State* state);
void menu(State* state);
void salir(State* state);
void crear_repositorio(State* state);
void commit(State* state);

User logger(int* flag);
int loggerCommit(char* descripcion);
#endif // !state_machine_h