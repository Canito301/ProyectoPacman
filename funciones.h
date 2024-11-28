#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "funciones.h"

#define ANCHO 27  // Ancho de la matriz del laberinto
#define ALTO 30   // Alto de la matriz del laberinto
#define TILE_SIZE 25

extern int puntaje_pac;


typedef struct { //struct que da la posicion y se usa para pacman y fantasmas
    int x;
    int y;
} posicion;



FILE * AbrirArchivo(int numero);
int mapa(int numero);
int columnas(int numero);
void RellenarMatriz(int numero, int **Matriz, int filas, int columnas);
void CerrarArchivo(FILE *fp);
void mov_fan(int laberinto[ALTO][ANCHO], posicion *pos_fan);
int mov_pacman(int laberinto[ALTO][ANCHO], posicion *pos_pac, int a);
void consumir_moneda(int laberinto[ALTO][ANCHO], posicion *pos_pac);
int revisar_choque(int laberinto[ALTO][ANCHO], posicion pos_pac, posicion pos_fan1, posicion pos_fan2, posicion pos_fan3, posicion pos_fan4);
void drawWallOutline(SDL_Renderer *renderer, int x, int y, SDL_Color color, int linestodraw,int cont,int con);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, SDL_Color color);
void drawMap(SDL_Renderer *renderer,int map[ALTO][ANCHO]);

#endif