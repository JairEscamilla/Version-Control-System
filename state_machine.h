#ifndef state_machine_h
#define state_machine_h 

// Importamos las biliotecas
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// Definimos las etiquetas de los estados de la maquina
typedef enum stateType{
    LOAD_USERS,
    CREAR_REPOSITORIO,
    CAMBIAR_DE_RAMA, 
    COMMIT,
    VER_COMMITS,
    MERGE,
    AGREGAR_USUARIOS,
    EXIT,
    STATE_NUM
}State;

// Definimos la estructura que contendrá la funcion a ejecutar junto a su label

typedef struct defEstado{
    State state;
    void (*process)(State* state);
}Estados;


// Prototipos de las funciones que ejecutara la maquina de estados
void load_users(State* state);

#endif // !state_machine_h