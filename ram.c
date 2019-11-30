
/*
 * @author: César Mauricio Arellano Velásquez, Allan Jair Escamilla Hernández, Raúl González Portillo.
 * @date:   25/Noviembre/2019
 * @file:   ram.c
 * @brief:  Implementación de máquinas de estados para el sistema controlador de versiones.
 */

#include "state_machine.h" // Incluimos el archivo que contiene las funciones para la maquina de estadps

// Funcion principal
int main(void){
    State state = LOAD_USERS; // Inicializamos el estado en el que se encontrara la maquina
    // Matriz que contiene las funciones y labels para cada estado
    Estados maquina[] = {
        {LOAD_USERS, load_users},
        {MENU, menu},
        {CREAR_REPOSITORIO, crear_repositorio},
        {COMMIT, commit}
        {EXIT, salir}
    };

    // Bucle infinito para la maquina de estados
    for( ; ;){
        system("clear");
        if(state > STATE_NUM){ // En caso de caer en un estado invalido, lanzamos una excepcion
            puts("Ha ocurrido un error con los estados):");
            return 0;
        }
        // Mandamos a ejecutar la funcion que se requiere ejecutar
        (*maquina[state].process)(&state);
    }

    return 0;
}

/* * Estado que lee carga los archivos que va a necesitar nuestro sistema.
   * @param States State Determina el siguiente estado de la máquina.
*/
void load_users(State *state){
    puts("Cargando files... ");
    FILE* fp = fopen("app/users.dat", "rt");
    if(fp == NULL){
        system("mkdir app");
        fp = fopen("app/users.dat", "wt");
        fprintf(fp, "%s\n%s\n", "UserPrueba", "12345");
        fclose(fp);
        fp = fopen("app/repositorios.dat", "wt");
        fclose(fp);
    }else 
        fclose(fp);
    puts("Los archivos se han cargado con exito!");
    *state = MENU;
}

/* * Estado que le muestra un menu de seleccion al usuario y así determinar el siguiente estado.
   * @param States State Determina el siguiente estado de la máquina.
*/
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

/* * Estado en el que creamos un nuevo repositorio segun lo que desee el usuario.
   * @param States State Determina el siguiente estado de la máquina.
*/
void crear_repositorio(State *state){
    *state = MENU;
    FILE* fp;
    int flag = 0;
    char repositorio[200], sentencia[200], renglon[200];
    strcpy(sentencia, "mkdir ");
    printf("Ingresar nombre del repositorio-> ");
    __fpurge(stdin);
    fgets(repositorio, sizeof(repositorio), stdin);
    repositorio[strlen(repositorio) - 1] = '\0';
    if(!logger()){
        puts("No se encuentra registrado en el sistema): ");
        puts("Presiona enter para continuar... ");
        getchar();
        return;
    }
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
        strcat(sentencia, "/master");
        system(sentencia);
        strcpy(sentencia, "mkdir ");
        strcat(sentencia, repositorio);
        strcat(sentencia, "/pruebas");
        system(sentencia);
        puts("Se ha creado con exito el repositorio!\n");
    }     
    puts("Presiona enter para continuar... ");
    getchar();
}

/* * Estado en el que salimos del sistema.
   * @param States State Determina el siguiente estado de la máquina.
*/
void salir(State *state){
    puts("Saliendo del sistema... ");
    system("sleep 5");
    exit(0);
}

/* * Funcion que usamos para iniciar sesion en el sistema.
*/
int logger(){
    char user[100], pwd[100], renglon[100], renglon2[100];
    FILE* fp = fopen("app/users.dat", "rt");
    printf("Ingresa tu usuario-> ");
    fgets(user, sizeof(user), stdin);
    user[strlen(user) - 1] = '\0';
    printf("Ingresa tu password-> ");
    fgets(pwd, sizeof(pwd), stdin);
    pwd[strlen(pwd) - 1] = '\0';
    while (fgets(renglon, 98, fp) != NULL){
        renglon[strlen(renglon) - 1] = '\0';
        fgets(renglon2, 100, fp);
        renglon2[strlen(renglon2) - 1] = '\0';
        if(strcmp(user, renglon) == 0 && strcmp(pwd, renglon2) == 0){
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* * Estado en el que el usuario tiene la posibilidad de hacer un commit.
   * @param States State Determina el siguiente estado de la máquina.
*/
void commit(State *state){
    *state = MENU;
    int flag = 0;
    char repositorio[200], linea[200], descripcion[200];
    FILE* fp = fopen("app/repositorios.dat", "rt");
    printf("Ingresar el repositorio al que desea hacer el commit-> ");
    fgets(repositorio, sizeof(repositorio), stdin);
    repositorio[strlen(repositorio) - 1] = '\0';
    while (fgets(linea, 150, fp) != NULL){
        linea[strlen(linea) - 1 ] = '\0';
        if(strcmp(repositorio, linea) == 0){
            flag = 1;
        }
    }
    if(flag == 0){
        printf("No se ha encontrado este repositorio):\n");
        return;
    }
    printf("Ingresar descripcion del commit-> ");
    fgets(descripcion, sizeof(descripcion), stdin);
    descripcion[strlen(descripcion) - 1] = '\0';
    loggerCommit(descripcion);
}

/* * Funcion que logea a un usuario al momento de crear un commit.
   * @param char* descripcion recibe la descripcion para el commit realizado por el usuario.
*/
int loggerCommit(char *descripcion){
    int flag = 0;
    char user[100], pwd[100], renglon[100], renglon2[100];
    FILE *fp = fopen("app/users.dat", "rt");
    printf("Ingresa tu usuario-> ");
    fgets(user, sizeof(user), stdin);
    user[strlen(user) - 1] = '\0';
    printf("Ingresa tu password-> ");
    fgets(pwd, sizeof(pwd), stdin);
    pwd[strlen(pwd) - 1] = '\0';
    while (fgets(renglon, 98, fp) != NULL){
        renglon[strlen(renglon) - 1] = '\0';
        fgets(renglon2, 100, fp);
        renglon2[strlen(renglon2) - 1] = '\0';
        if (strcmp(user, renglon) == 0 && strcmp(pwd, renglon2) == 0){
            fclose(fp);
            flag =  1;
        }
    }
    fclose(fp);
    flag =  0;
    if(flag == 0){
        puts("Este usuario no tiene permisos para hacer commits sobre este repositorio):");
        return 0;
    }
}