#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "funciones.h"



// Movimiento fantasmas
void mov_fan(int laberinto[ALTO][ANCHO], posicion *pos_fan) {
    int direccion = rand() % 4; //rand%4 para tener 4 tipos de movimientos aleatorios 0 arriba 1 abajo 2 izquierda 3 derecha
    posicion nueva_pos_fan = *pos_fan;

    if (direccion == 0 && nueva_pos_fan.y > 0 && (laberinto[nueva_pos_fan.y - 1][nueva_pos_fan.x] != 0)) //chequea 2 cosas, si no está pegado a un borde y si a donde se intenta mover no es una pared (0)
        nueva_pos_fan.y--; //en caso de que se permita se realiza el movimiento del fantasma en nueva_pos_fan
    if (direccion == 1 && nueva_pos_fan.y < (ALTO-1) && (laberinto[nueva_pos_fan.y + 1][nueva_pos_fan.x] != 0)) 
        nueva_pos_fan.y++;
    if (direccion == 2 && nueva_pos_fan.x > 0 && (laberinto[nueva_pos_fan.y][nueva_pos_fan.x - 1] != 0)) 
        nueva_pos_fan.x--;
    if (direccion == 3 && nueva_pos_fan.x < (ANCHO-1) && (laberinto[nueva_pos_fan.y][nueva_pos_fan.x + 1] != 0)) 
        nueva_pos_fan.x++;
    
    *pos_fan = nueva_pos_fan; //se actualiza la posicion real del fantasma
}

// Movimiento pacman
int mov_pacman(int laberinto[ALTO][ANCHO], posicion *pos_pac, int a) { 
    // 1 arriba, 2 abajo, 3 izquierda, 4 derecha
    if (a == 1) {
        if (pos_pac->y > 0 && laberinto[pos_pac->y - 1][pos_pac->x] != 0) { //mismos chequeos que para el movimiento del fantasma solo que la dirección se da por la variable "a" que se modifica en main dependiendo de que se ingresa por el teclado
            pos_pac->y -= 1;
            return 1;
        }
        if (pos_pac->y == 0 && laberinto[ALTO-1][pos_pac->x] != 0){ //permite atravesar de un lado al otro del mapa si es que es posible
            pos_pac->y = (ALTO-1);
            return 1;
        }
    }
    if (a == 2) {
        if (pos_pac->y < (ALTO-1) && laberinto[pos_pac->y + 1][pos_pac->x] != 0) {
            pos_pac->y += 1;
            return 1;
        }
        if (pos_pac->y == (ALTO-1) && laberinto[0][pos_pac->x] != 0){
            pos_pac->y = 0;
            return 1;
        }
    }
    if (a == 3) {
        if (pos_pac->x > 0 && laberinto[pos_pac->y][pos_pac->x - 1] != 0) {
            pos_pac->x -= 1;
            return 1;
        }
        if (pos_pac->x == 0 && laberinto[pos_pac->y][ANCHO-1] != 0){
            pos_pac->x = (ANCHO-1);
            return 1;
        }
        
    }
    if (a == 4) {
        if (pos_pac->x < (ANCHO-1) && laberinto[pos_pac->y][pos_pac->x + 1] != 0) {
            pos_pac->x += 1;
            return 1;
        }
        if (pos_pac->x == (ANCHO-1) && laberinto[pos_pac->y][0] != 0){
            pos_pac->x = 0;
            return 1;
        }
    }
    return 0;
}

void consumir_moneda(int laberinto[ALTO][ANCHO], posicion *pos_pac) {
    if (laberinto[pos_pac->y][pos_pac->x] == 2) {//si la coordenada donde se ubica pacman es -1(moneda) entonces -->
        laberinto[pos_pac->y][pos_pac->x] = 1; // se consume la moneda y se cambia a 1 (espacio vacio)
        //printf("moneda atrapada\n");
        puntaje_pac+=1;
    }
    else if(laberinto[pos_pac->y][pos_pac->x] == 8){
      laberinto[pos_pac->y][pos_pac->x] = 1;
      //printf("Cereza atrapada\n");
      puntaje_pac+=100;
    }
    else if(laberinto[pos_pac->y][pos_pac->x] == 9){
      laberinto[pos_pac->y][pos_pac->x] = 1;
      //printf("Moneda grande atrapada\n");
      puntaje_pac+=10;
    }
    
}

int revisar_choque(int laberinto[ALTO][ANCHO], posicion pos_pac, posicion pos_fan1, posicion pos_fan2, posicion pos_fan3, posicion pos_fan4){
  if ((pos_pac.x == pos_fan1.x && pos_pac.y == pos_fan1.y) || (pos_pac.x == pos_fan2.x && pos_pac.y == pos_fan2.y) || (pos_pac.x == pos_fan3.x && pos_pac.y == pos_fan3.y) || (pos_pac.x == pos_fan4.x && pos_pac.y == pos_fan4.y)){
    return 1;
  }
  return 0;
}


FILE * AbrirArchivo(int numero) {
    FILE *fp;
    if (numero == 1) {
        fp = fopen("Mapa2.txt", "r");
    } else if (numero == 2) {
        fp = fopen("laberinto.txt", "r");
    } else if (numero == 3) {
      fp = fopen("Mapa.txt", "r");
    } else if (numero == 4) {
      fp = fopen("mapa3.txt", "r");
    } else {
        printf("No se pudo abrir el archivo.\n");
        return NULL;
    }
    return fp;
}

void CerrarArchivo(FILE *fp) {
    if (fp != NULL) {
        fclose(fp);
        printf("Se cerró el archivo correctamente.\n");
    } else {
        printf("No había archivo para cerrar.\n");
    }
}

int mapa(int numero) {
    FILE *fp = AbrirArchivo(numero);
    if (fp == NULL) return 0;

    char linea[256];
    int contador = 0;

    // Leer el archivo línea por línea usando fgets()
    while (fgets(linea, sizeof(linea), fp) != NULL) {
      for (int i = 0; linea[i] != '\0'; i++) {                            //El caracter toma toda la línea, por lo que lee hasta que termina, por eso hasta \0.
	if (isdigit(linea[i])) {                                      //Si el caracter es un dígito, aumenta el contador.
                contador++;
            }
        }
    }
    fclose(fp);
    return contador;                                                   //Va a retornar los dígitos
}

int columnas(int numero) {
    FILE *fp = AbrirArchivo(numero);
    if (fp == NULL) return 0;

    char linea[256];
    int columnas = 0;

    // Leer la primera línea para contar los números
    if (fgets(linea, sizeof(linea), fp) != NULL) {
        for (int i = 0; linea[i] != '\0'; i++) {
            if (isdigit(linea[i])) {
                columnas++;
            }
        }
    }

    fclose(fp);
    return columnas;
}

void RellenarMatriz(int numero, int **Matriz, int filas, int columnas) {
    FILE *fp = AbrirArchivo(numero);
    if (fp == NULL) return;

    char linea[256];
    int fila = 0;

    // Leer el archivo línea por línea y llenar la matriz
    while (fgets(linea, sizeof(linea), fp) != NULL && fila < filas) {
        int columna = 0;
        for (int i = 0; linea[i] != '\0' && columna < columnas; i++) {
            if (isdigit(linea[i])) {
                Matriz[fila][columna] = linea[i] - '0'; // Convertir char a int
                columna++;
            }
        }
        fila++;
    }

    fclose(fp);
}
//dibuja lineas para las paredes
void drawWallOutline(SDL_Renderer *renderer, int x, int y, SDL_Color color, int linestodraw,int cont,int con) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    float indent=(float)TILE_SIZE/5.0; //las paredes de pacman no estan al borde de un "tile", están mas adentro, eso es para eso
    float plus=0, plu=0; //se le agrega un "pedazo" si tiene otro bloque de pared al lado, sin esto, las lineas serian discontinuas
    if(cont==1)
        plus=indent*2;  //indicadores de si hay una linea al lado
    if(con==1)
        plu=indent*2;
    
    // arriba
    if(linestodraw==1)
        SDL_RenderDrawLine(renderer, x * TILE_SIZE+indent-plus, y * TILE_SIZE+indent, (x + 1) * TILE_SIZE-indent+plu, y * TILE_SIZE+indent);

    // abajo
    if(linestodraw==2)
        SDL_RenderDrawLine(renderer, x * TILE_SIZE+indent-plus, (y + 1) * TILE_SIZE-indent, (x + 1) * TILE_SIZE -indent+plu, (y + 1) * TILE_SIZE -indent);

    // izquierda
    if(linestodraw==3)
        SDL_RenderDrawLine(renderer, x * TILE_SIZE+indent, y * TILE_SIZE+indent-plus, x * TILE_SIZE+indent, (y + 1) * TILE_SIZE-indent+plu);

    // derecha
    if(linestodraw==4)
        SDL_RenderDrawLine(renderer, (x + 1) * TILE_SIZE-indent, y * TILE_SIZE+indent-plus, (x + 1) * TILE_SIZE-indent, (y + 1) * TILE_SIZE-indent+plu);
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) { //carga las texturas para pacman, fantasmas y cereza + puntaje
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (surface == NULL) {
        printf("No se pudo cargar la textura SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return newTexture;
}

void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w * w + h * h <= radius * radius) {
                int px = centerX + w;
                int py = centerY + h;
                
                //nos aseguramos que el ciculo esté en los limites
                if (px >= 0 && py >= 0 && px < ANCHO * TILE_SIZE && py < ALTO * TILE_SIZE) {
                    SDL_RenderDrawPoint(renderer, px, py);
                }
            }
        }
    }
}

void drawMap(SDL_Renderer *renderer,int map[ALTO][ANCHO]){
    // colores
    SDL_Color puntosgrand = {255, 229, 170, 130};   // un poco mas brillante que los normales
    SDL_Color puntos = { 255 , 229, 180, 1};   //durazno
    SDL_Color pared = {0, 0, 255, 255};  // azul
    SDL_Color normal = {0, 0, 0, 255};  // negro
    for (int y = 0; y < ALTO; y++) {    
        for (int x = 0; x < ANCHO; x++) {   
            SDL_Color tileColor = normal; 

            if (map[y][x] == 9) {  //puntos grandes
                tileColor = puntosgrand;
                drawCircle(renderer, x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 3, tileColor);
            } 
            else if (map[y][x] == 0) {      //paredes
                tileColor = pared;                            //para cada tipo de linea, solo la dibuja si no está "encerrada" por otros bloques
               if (y==0 || map[y-1][x]!=0){                     //luego chequea si es un borde o una de al medio para saber si agregar largo o no (variables p y q)
                    int p=0, q=0;
                    if(x!=0 && map[y][x-1]==0)
                        p=1;
                    if(x!=ANCHO && map[y][x+1]==0)
                        q=1;
                    drawWallOutline(renderer, x, y, tileColor,1,p,q);  //arriba
                }
                if (y==ALTO-1 || map[y+1][x]!=0){
                    int p=0, q=0;
                    if(x!=0 && map[y][x-1]==0)
                        p=1;
                    if(x!=ANCHO && map[y][x+1]==0)
                        q=1;
                    drawWallOutline(renderer, x, y, tileColor,2,p,q); //abajo
                }
                if (x==0 || map[y][x-1]!=0){
                    int p=0, q=0;
                    if(y!=0 && map[y-1][x]==0)
                        p=1;
                    if(y!=ALTO && map[y+1][x]==0)
                        q=1;
                    drawWallOutline(renderer, x, y, tileColor,3,p,q); //izquierda
                }
                if (x==ANCHO-1 || map[y][x+1]!=0 ){
                    int p=0, q=0;
                    if(y!=0 && map[y-1][x]==0)
                        p=1;
                    if(y!=ALTO && map[y+1][x]==0)
                        q=1;
                    drawWallOutline(renderer, x, y, tileColor,4,p,q);  //derecha
                }
            }
            else if (map[y][x] == 2) {    //puntos
                tileColor = puntos;
                drawCircle(renderer, x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 10, tileColor);
                }
           
    }
}
}

