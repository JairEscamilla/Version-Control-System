
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
        {COMMIT, commit},
        {CAMBIAR_DE_RAMA, cambiar_rama},
        {VER_COMMITS, ver_commits},
        {MERGE, merge}
    };

    // Bucle infinito para la maquina de estados
    for( ; ; ){
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
    int flag = 0, flag2 = 0;
    User login;
    char repositorio[200], sentencia[200], renglon[200];
    strcpy(sentencia, "mkdir ");
    printf("Ingresar nombre del repositorio-> ");
    __fpurge(stdin);
    fgets(repositorio, sizeof(repositorio), stdin);
    repositorio[strlen(repositorio) - 1] = '\0';
    login = logger(&flag2);
    if(!flag2){
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
        strcpy(sentencia, repositorio);
        strcat(sentencia, "/usuarios.dat");
        fp = fopen(sentencia, "ab");
        fwrite(&login, sizeof(User), 1, fp);
        fclose(fp); 
        strcpy(sentencia, repositorio);
        strcat(sentencia, "/branches.dat");
        fp = fopen(sentencia, "at");
        fprintf(fp, "master\n0\n");
        fclose(fp);
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
User logger(int* flag){
    User login;
    char user[100], pwd[100], renglon[100], renglon2[100];
    FILE* fp = fopen("app/users.dat", "rt");
    printf("Ingresa tu usuario-> ");
    fgets(user, sizeof(user), stdin);
    user[strlen(user) - 1] = '\0';
    printf("Ingresa tu password-> ");
    fgets(pwd, sizeof(pwd), stdin);
    pwd[strlen(pwd) - 1] = '\0';
    strcpy(login.user, user);
    strcpy(login.pwd, pwd);
    while (fgets(renglon, 98, fp) != NULL){
        renglon[strlen(renglon) - 1] = '\0';
        fgets(renglon2, 100, fp);
        renglon2[strlen(renglon2) - 1] = '\0';
        if(strcmp(user, renglon) == 0 && strcmp(pwd, renglon2) == 0){
            fclose(fp);
            *flag = 1;
        }
    }
    fclose(fp);
    return login;
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
    __fpurge(stdin);
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
        puts("Presiona enter para continuar... ");
        getchar();
        return;
    }
    printf("Ingresar descripcion del commit-> ");
    fgets(descripcion, sizeof(descripcion), stdin);
    descripcion[strlen(descripcion) - 1] = '\0';
    loggerCommit(descripcion, repositorio);
    puts("Commit realizado con exito!");
    puts("Presiona enter para continuar... ");
    getchar();
}

/* * Funcion que logea a un usuario al momento de crear un commit.
   * @param char* descripcion recibe la descripcion para el commit realizado por el usuario.
*/
int loggerCommit(char* descripcion, char* repositorio){
    int flag = 0;
    char renglon[200], renglon2[200], file[100];
    FILE *fp;
    User usuarios, temp;
    strcpy(file, repositorio);
    strcat(file, "/usuarios.dat");
    fp = fopen(file, "rb");
    printf("Ingresa tu usuario-> ");
    gets(renglon);
    printf("Ingresa tu password-> ");
    fread(&usuarios, sizeof(User), 1, fp);
    gets(renglon2);
    while (!feof(fp)){
        fread(&usuarios, sizeof(User), 1, fp);
        if (strcmp(usuarios.user, renglon) == 0 && strcmp(usuarios.pwd, renglon2) == 0){
            temp = usuarios;
            flag =  1;
        }
    }
    fclose(fp);
    if(flag == 0){
        puts("Este usuario no tiene permisos para hacer commits sobre este repositorio):");
        return 0;
    }
    make_commit(temp, descripcion, repositorio);
}

void make_commit(User temp, char* descripcion, char* repositorio){
    FILE* fp;
    char sentencia[100], linea[100];
    struct tm *timeinfo;
    Commit commit;
    commit.user = temp;
    commit.time = time(NULL);
    time(&commit.time);
    timeinfo = localtime(&commit.time);
    sprintf(commit.fechayhora, "%d/%d/%d-%d:%d:%d\n", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    strcpy(commit.descripcion, descripcion);
    strcpy(sentencia, repositorio);
    strcat(sentencia, "/branches.dat");
    fp = fopen(sentencia, "rt");
    strcpy(commit.branch, fgets(linea, 100, fp));
    commit.branch[strlen(commit.branch) - 1] = '\0';
    commit.id = atoi(fgets(linea, 10, fp));
    fclose(fp);
    fp = fopen(sentencia, "wt");
    fprintf(fp, "%s\n%d\n", commit.branch, commit.id + 1);
    fclose(fp);
    strcpy(sentencia, repositorio);
    strcat(sentencia, "/commits.dat");
    fp = fopen(sentencia, "ab");
    fwrite(&commit, sizeof(Commit), 1, fp);
    fclose(fp);
    mover_archivos(commit.id, repositorio, commit);
}

void mover_archivos(int id, char* repositorio, Commit commit){
    FILE* fp;
    char idx[10], directorioCommit[200], comando[800];
    DIR *d;
    strcpy(directorioCommit, "mkdir ");
    strcat(directorioCommit, repositorio);
    sprintf(idx, "%d", id);
    strcat(directorioCommit, "/");
    strcat(directorioCommit, commit.branch);
    strcat(directorioCommit, "/");
    strcat(directorioCommit, idx);
    system(directorioCommit);
    struct dirent *dir;
    d = opendir(repositorio);
    puts("FILES DEL COMMIT: ");
    if(d){
        while ((dir = readdir(d)) != NULL){
            if(strcmp(dir->d_name, "usuarios.dat") != 0 && strcmp(dir->d_name, "pruebas") != 0 && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "commits.dat") != 0 && strcmp(dir->d_name, "branches.dat") != 0 && strcmp(dir->d_name, "master") != 0){
                printf("\t-> %s\n", dir->d_name);
                strcpy(comando, "cp ");
                strcat(comando, repositorio);
                strcat(comando, "/");
                strcat(comando, dir->d_name);
                strcat(comando, " ");
                strcat(comando, repositorio);
                strcat(comando, "/");
                strcat(comando, commit.branch);
                strcat(comando, "/");
                strcat(comando, idx);
                system(comando);
            }
        }
        closedir(d);
    }
}

void cambiar_rama(State* state){
    *state = MENU;
    DIR *d;
    struct dirent *dir;
    char repo[200], comando[200], linea[200], linea2[20], aux[200], aux2[200], mayor[10] = "-1";
    FILE* fp;
    printf("Ingresar el repositorio al que se desea cambiar de rama-> ");
    __fpurge(stdin);
    gets(repo);
    if(!buscar_repositorios(repo)){
        puts("No se ha encontrado este repositorio):");
        puts("Presiona enter para continuar... ");
        getchar();
        return;
    }
    strcpy(comando, repo);
    strcat(comando, "/branches.dat");
    fp = fopen(comando, "rt");
    fgets(linea, 200, fp);
    linea[strlen(linea) - 1] = '\0';
    if(strcmp(linea, "master") == 0){
        strcpy(linea, "pruebas");
    }else
        strcpy(linea, "master");
    fgets(linea2, 20, fp);
    fclose(fp);
    fp = fopen(comando, "wt");
    fprintf(fp, "%s\n%s", linea, linea2);
    fclose(fp);

    strcpy(aux, repo);
    strcat(aux, "/");
    strcat(aux, linea);
    strcat(aux, "/");
    d = opendir(aux);
    if(d){
        while ((dir = readdir(d)) != NULL)
            if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
                if(atoi(dir->d_name) > atoi(mayor))
                    strcpy(mayor, dir->d_name);
        closedir(d);
    }
    if(atoi(mayor) != -1){
        strcat(aux, mayor);
        d = opendir(aux);
        if(d){
            while ((dir = readdir(d)) != NULL)
                if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
                    strcpy(aux2, "cp ");
                    strcat(aux2, aux);
                    strcat(aux2, "/");
                    strcat(aux2, dir->d_name);
                    strcat(aux2, " ");
                    strcat(aux2, repo);
                    strcat(aux2, "/");
                    system(aux2);
                }
            closedir(d);
        }
    }
    printf("Se ha cambiado correctamente de rama!\nAhora se encuentra trabajando en la rama %s\n", linea);
    puts("Presiona enter para continuar... ");
    getchar();
}

int buscar_repositorios(char* repositorio){
    FILE* fp = fopen("app/repositorios.dat", "rt");
    char linea[150];
    int flag = 0;
    while (fgets(linea, 150, fp) != NULL){
        linea[strlen(linea) - 1 ] = '\0';
        if(strcmp(repositorio, linea) == 0){
            flag = 1;
        }
    }
    fclose(fp);
    return flag;
}

void ver_commits(State* state){
    *state = MENU;
    char repo[200], comando[200], cadena[200];
    Commit commits;
    FILE* fp;
    printf("Ingresar el repositorio del que desea ver los commits-> ");
    __fpurge(stdin);
    gets(repo);
    if(!buscar_repositorios(repo)){
        puts("No se ha encontrado este repositorio):");
        puts("Presiona enter para continuar... ");
        getchar();
        return;
    }
    strcpy(comando, repo);
    strcat(comando, "/commits.dat");
    puts("LISTADO DE COMMITS: \n");
    fp = fopen(comando, "rb");
    fread(&commits, sizeof(Commit), 1, fp);
    while(!feof(fp)){
        printf("\tCommit numero %d: ", commits.id);
        printf("\n\t\tFecha y hora: %s\t\tUsuario: %s\n\t\tRama: %s\n\t\tDescripcion: %s\n", commits.fechayhora, commits.user.user, commits.branch, commits.descripcion);
        fread(&commits, sizeof(Commit), 1, fp);
    }
    fclose(fp);
    puts("\nPresiona enter para continuar...");
    getchar();
}

void merge(State* state){
    *state = MENU;
    FILE* fp;
    char repo[200], comando[200], linea[200], aux[10], direccion[150], direccion2[150];
    int ultimoCommit, penultimoCommit;
    printf("Ingresar el nombre del repositorio que quiere combinar-> ");
    __fpurge(stdin);
    gets(repo);
    if(!buscar_repositorios(repo)){
        puts("No se ha encontrado este repositorio):");
        puts("Presiona enter para continuar... ");
        getchar();
        return;
    }
    strcpy(comando, repo);
    strcat(comando, "/branches.dat");
    fp = fopen(comando, "rt");
    fgets(linea, sizeof(linea), fp);
    ultimoCommit = atoi(fgets(linea, sizeof(linea), fp)) - 1;
    penultimoCommit = ultimoCommit - 1;
    fclose(fp);
    sprintf(aux, "%d", ultimoCommit);
    buscarCommit(aux, repo, direccion);     
    sprintf(aux, "%d", penultimoCommit);
    buscarCommit(aux, repo, direccion2);
    mergeFiles(direccion, direccion2, repo);
    puts("Las ramas han sido combinadas de manera exitosa!");
    puts("Presiona enter para continuar... ");
    getchar();
}
void buscarCommit(char* id, char* repositorio, char direccion[]){
    char branch[100], aux[150];
    DIR *d;
    struct dirent *dir;
    strcpy(aux, repositorio);
    strcat(aux, "/master");
    d = opendir(aux);
    if(d){
        while ((dir = readdir(d)) != NULL){
            if(strcmp(dir->d_name, id) == 0){
                strcpy(direccion, aux);
                strcat(direccion, "/");
                strcat(direccion, id);
                strcat(direccion, "/");
            }
        }
        closedir(d);
    }
    strcpy(aux, repositorio);
    strcat(aux, "/pruebas");
    d = opendir(aux);
    if(d){
        while ((dir = readdir(d)) != NULL){
            if(strcmp(dir->d_name, id) == 0){
                strcpy(direccion, aux);
                strcat(direccion, "/");
                strcat(direccion, id);
                strcat(direccion, "/");
            }
        }
        closedir(d);
    }
}

void mergeFiles(char direccion[], char direccion2[], char repositorio[]){
    FILE* fp, *fp2;
    char aux1[200], aux2[200], aux[200];
    char* merged[100];
    int counter = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(direccion);
    if(d){
        while ((dir = readdir(d)) != NULL){
            if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..")){
                merged[counter] = dir->d_name;
                counter++;
                strcpy(aux, "cp ");
                strcat(aux, direccion);
                strcat(aux, dir->d_name);
                strcat(aux, " ");
                strcat(aux, repositorio);
                strcat(aux, "/");   
                system(aux);
            } 
        }
        closedir(d);
    }
    d = opendir(direccion2);
    if(d){
        while ((dir = readdir(d)) != NULL){
            if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..")){
                if(buscarArchivos(merged, counter, dir->d_name)){
                    strcpy(aux1, direccion);
                    strcat(aux1, dir->d_name);
                    strcpy(aux2, direccion2);
                    strcat(aux2, dir->d_name);
                    fp = fopen(aux1, "rt");
                    fp2 = fopen(aux2, "rt");    
                    fseek(fp, 0L, SEEK_END);
                    fseek(fp2, 0L, SEEK_END);
                    if(ftell(fp2) >= ftell(fp)){
                        strcpy(aux, "cp ");
                        strcat(aux, direccion2);
                        strcat(aux, dir->d_name);
                        strcat(aux, " ");
                        strcat(aux, repositorio);
                        strcat(aux, "/");   
                        system(aux);
                    }
                    fclose(fp);
                    fclose(fp2);
                }
            } 
        }
        closedir(d);
    }
    
}

int buscarArchivos(char* array[], int counter, char buscado[]){
    int flag = 0;
    for(int i = 0; i < counter; i++)
        if(strcmp(array[i], buscado) == 0)
            flag = 1;
    return flag;
}