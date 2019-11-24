#include "state_machine.h"

int main(void){
    State state = LOAD_USERS;
    // Matriz que contiene las funciones y labels para cada estado
    Estados maquina[] = {
        {LOAD_USERS, load_users},
        {MENU, menu},
        {CREAR_REPOSITORIO, crear_repositorio},
        {EXIT, salir}
    };

    // Bucle infinito para la maquina de estados
    for( ; ;){
        system("clear");
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
    puts("Cargando files... ");
    FILE* fp = fopen("app/users.dat", "rt");
    if(fp == NULL){
        system("mkdir app");
        fp = fopen("app/users.dat", "wt");
        fprintf(fp, "%s\n%s\n", "UserPrueba", "12345");
        fclose(fp);
        fp = fopen("app/repositorios.dat", "wt");
        fprintf(fp, "rp\n");
        fclose(fp);
    }else 
        fclose(fp);
    puts("Los archivos se han cargado con exito!");
    *state = MENU;
}

void menu(State *state){
    int opcion;
    puts("\t\tMENU PRINCIPAL");
    puts("\t1.- Crear repositorio.");
    puts("\t2.- Hacer commit sobre un repositorio.");
    puts("\t3.- Cambiar de rama un repositorio.");
    puts("\t4.- Ver commits de un repositorio.");
    puts("\t5.- Combinar ramas de un repositorio.");
    puts("\t6.- Listar repositorios.");
    puts("\t7.- Agregar colaboradores a un repositorio.");
    puts("\t8.- Salir.\n");
    printf("\nSeleccione la opcion que desee realizar-> ");
    scanf("%d", &opcion);
    switch (opcion){
        case 1:
            *state = CREAR_REPOSITORIO;
            break;
        case 2: 
            *state = COMMIT;
            break;
        case 3: 
            *state = CAMBIAR_DE_RAMA;
            break;
        case 4: 
            *state = VER_COMMITS;
            break;
        case 5:
            *state = MERGE;
            break;
        case 6: 
            *state = VER_REPOSITORIOS;
            break;
        case 7: 
            *state = AGREGAR_USUARIOS;
            break;
        case 8: 
            *state = EXIT;
        default:
            *state = MENU;
            break;
    }
}

void crear_repositorio(State *state){
    FILE* fp;
    int flag = 0;
    char repositorio[200], sentencia[200], renglon[200];
    strcpy(sentencia, "mkdir ");
    printf("Ingresar nombre del repositorio-> ");
    __fpurge(stdin);
    fgets(repositorio, sizeof(repositorio), stdin);
    repositorio[strlen(repositorio) - 1] = '\0';
    fp = fopen("app/repositorios.dat", "rt");
    while (fgets(renglon, 100, fp) != NULL){
        renglon[strlen(renglon) - 1 ] = '\0';
        if (strcmp(repositorio, renglon) == 0)
            flag = 1;
    }
    fclose(fp);

    if(flag == 1)
        puts("Este repositorio ya existe):\n");
    else{
        fp = fopen("app/repositorios.dat", "at");
        strcat(sentencia, repositorio);
        fprintf(fp, "%s\n", repositorio);
        fclose(fp);
        system(sentencia);
        puts("Se ha creado con exito el repositorio!\n");
    } 
    
    puts("Presiona enter para continuar... ");
    getchar();
    *state = MENU;
}   

void salir(State *state){
    puts("HEEEERE");
    system("sleep 5");
    exit(0);
}