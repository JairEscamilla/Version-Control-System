#include "state_machine.h"

int main(void){
    State state = LOAD_USERS;
    // Matriz que contiene las funciones y labels para cada estado
    Estados maquina[] = {
        {LOAD_USERS, load_users}
    };

    // Bucle infinito para la maquina de estados
    for( ; ;){
        if(state > STATE_NUM){
            puts("Ha ocurrido un error con los estados):");
            return 0;
        }
        // Mandamos a ejecutar la funcion que se requiere ejecutar
        (*maquina[state].process)(&state);
    }

    return 0;
}

void load_users(State *state){
    puts("Cargando files");
    *state = LOAD_USERS;
    system("sleep 2");
}