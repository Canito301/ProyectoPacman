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
int vidas = 3; //
int contador_niveles=1;
int puntaje_pac = 0;
posicion pos_pac = {1, 1}; //inicializacion de posiciones de pacman y fantasmas
posicion pos_fan1 = {13, 13};
posicion pos_fan2 = {13, 13};
posicion pos_fan3 = {13, 13};
posicion pos_fan4 = {13, 13};







int main(int argc, char *argv[]) {
    int m, n, contador, numero;
    printf("Para ganar debe superar el nivel seleccionado 3 veces. \nTendras un total de 3 vidas para poder lograrlo. \nSi superas el nivel una vez, se te otorgara una vida extra\n");
    printf("Presione un numero del 1 al 4 en su teclado para elegir el nivel: \n");
    numero = _getch() - '0'; //evita presionar enter

    FILE *fp = AbrirArchivo(numero);
    if (fp == NULL) return 1;

    contador = mapa(numero);
    n = columnas(numero);
    m = contador / n;

    int **Matriz = malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        Matriz[i] = malloc(n * sizeof(int));
    }

    RellenarMatriz(numero, Matriz, m, n);
    int laberinto[ALTO][ANCHO];
    for (int i = 0;i<ALTO;i++){
      for (int j = 0;j<ANCHO;j++){
        laberinto[i][j]=Matriz[i][j];
      }
    }
    
    
    CerrarArchivo(fp);

    // Inicializa
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL no se pudo inicializar SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Pac-Man Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO * TILE_SIZE, ALTO * TILE_SIZE+50, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("No se pudo crear la ventana SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("No se pudo crear el render! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        
        return 1;
    }


    SDL_Texture* n0Texture = loadTexture(renderer, "0.bmp");
    SDL_Texture* n1Texture = loadTexture(renderer, "1.bmp");
    SDL_Texture* n2Texture = loadTexture(renderer, "2.bmp");
    SDL_Texture* n3Texture = loadTexture(renderer, "3.bmp");
    SDL_Texture* n4Texture = loadTexture(renderer, "4.bmp");
    SDL_Texture* n5Texture = loadTexture(renderer, "5.bmp");
    SDL_Texture* n6Texture = loadTexture(renderer, "6.bmp");
    SDL_Texture* n7Texture = loadTexture(renderer, "7.bmp");
    SDL_Texture* n8Texture = loadTexture(renderer, "8.bmp");
    SDL_Texture* n9Texture = loadTexture(renderer, "9.bmp");
    SDL_Texture *arr[]={n0Texture ,n1Texture,n2Texture,n3Texture,n4Texture,n5Texture,n6Texture,n7Texture,n8Texture,n9Texture};

    SDL_Texture* pacman1Texture = loadTexture(renderer, "pacmanM.bmp"); //frame intermedio, pacman con boca cerrada
    SDL_Texture* pacmanRTexture = loadTexture(renderer, "pacmanr.bmp"); //derecha
    SDL_Texture* pacmanLTexture = loadTexture(renderer, "pacmanL.bmp");  //izquierda
    SDL_Texture* pacmanUTexture = loadTexture(renderer, "pacmanU.bmp");  //arriba
    SDL_Texture* pacmanDTexture = loadTexture(renderer, "pacmanD.bmp");    //abajo
        
        

    SDL_Texture* ghost1Texture = loadTexture(renderer, "blinky.bmp");
    SDL_Texture* ghost2Texture = loadTexture(renderer, "pinky.bmp");
    SDL_Texture* ghost3Texture = loadTexture(renderer, "inky.bmp");
    SDL_Texture* ghost4Texture = loadTexture(renderer, "clyde.bmp");
    SDL_Texture* cherryTexture = loadTexture(renderer, "cherry.bmp");

    SDL_Texture* live1Texture = loadTexture(renderer, "1 LIFE.bmp");
    SDL_Texture* live2Texture = loadTexture(renderer, "2 LIVES.bmp");
    SDL_Texture* live3Texture = loadTexture(renderer, "3 LIVES.bmp");
    SDL_Texture* scoreTexture = loadTexture(renderer, "SCORE.bmp");
    SDL_Texture* lvl1Texture = loadTexture(renderer, "LVL 1.bmp");
    SDL_Texture* lvl2Texture = loadTexture(renderer, "LVL 2.bmp");
    SDL_Texture* lvl3Texture = loadTexture(renderer, "LVL 3.bmp");

    SDL_Rect pacmanRect = { pos_pac.x * TILE_SIZE, pos_pac.y * TILE_SIZE, (float)TILE_SIZE *1.0 , (float)TILE_SIZE *1.0  };
    SDL_Rect ghost1Rect = { pos_fan1.x * TILE_SIZE, pos_fan1.y * TILE_SIZE, (float)TILE_SIZE *1 , (float)TILE_SIZE *1  };
    SDL_Rect ghost2Rect = { pos_fan2.x * TILE_SIZE, pos_fan2.y * TILE_SIZE, (float)TILE_SIZE *1 , (float)TILE_SIZE *1  };
    SDL_Rect ghost3Rect = { pos_fan3.x * TILE_SIZE, pos_fan3.y * TILE_SIZE, (float)TILE_SIZE *1 , (float)TILE_SIZE *1  };
    SDL_Rect ghost4Rect = { pos_fan4.x * TILE_SIZE, pos_fan4.y * TILE_SIZE, (float)TILE_SIZE *1 , (float)TILE_SIZE *1  };
    SDL_Rect livesRect = { 225, 750, 225 , 50 };
    SDL_Rect scoreRect = { 0, 750, 130 , 50 };
    SDL_Rect lvlRect = { 460, 750, 225 , 50 };
    SDL_Rect mRect = { 130, 750, 23 , 50 };
    SDL_Rect cRect = { 153, 750, 23 , 50 };
    SDL_Rect dRect = { 176, 750, 23 , 50 };
    SDL_Rect uRect = { 199, 750, 23 , 50 };
   


    // Main loop
    SDL_Event e;
    int a=0; //variable para dirección pacman
    int quit = 0;
    int idea=0; //cierra o abre la boca
    int cont=0;     //contador para que no lo haga inmediatamente
    const Uint8 *key = SDL_GetKeyboardState(NULL);
    

   

    SDL_Event event;
    while (!quit) { //ciclo principal del juego
    

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (e.type == SDL_KEYDOWN) { 
                if (key[SDL_SCANCODE_W])
                    a=1;  
                else if (key[SDL_SCANCODE_S]) 
                    a=2;  
                else if (key[SDL_SCANCODE_A]) 
                    a=3;  
                else if (key[SDL_SCANCODE_D]) 
                    a=4;  
                else if (key[SDL_SCANCODE_ESCAPE])
                    quit = 1; 
                else
                    a=0;
            }
        }

        // limpiar pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // negro
        SDL_RenderClear(renderer);

        if(idea%2==0){
            SDL_RenderCopy(renderer, pacman1Texture, NULL, &pacmanRect);
            }
        else{
            if(a==1)
                SDL_RenderCopy(renderer, pacmanUTexture, NULL, &pacmanRect);
            else if(a==2)
                SDL_RenderCopy(renderer, pacmanDTexture, NULL, &pacmanRect);
            else if(a==3)
                SDL_RenderCopy(renderer, pacmanLTexture, NULL, &pacmanRect);
            else if(a==4)
                SDL_RenderCopy(renderer, pacmanRTexture, NULL, &pacmanRect);
            }
            cont++;
            if(cont%2==0)
                idea++;

        SDL_RenderCopy(renderer, ghost1Texture, NULL, &ghost1Rect);
        SDL_RenderCopy(renderer, ghost2Texture, NULL, &ghost2Rect);
        SDL_RenderCopy(renderer, ghost3Texture, NULL, &ghost3Rect);
        SDL_RenderCopy(renderer, ghost4Texture, NULL, &ghost4Rect);

        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

        drawMap(renderer,laberinto);
        for (int y = 0; y < ALTO; y++) 
            for (int x = 0; x < ANCHO; x++) 
                if (laberinto[y][x] == 8) {    //1
                    SDL_Rect cherryRect = { x * TILE_SIZE, y * TILE_SIZE, (float)TILE_SIZE *1.2 , (float)TILE_SIZE *1.2  };
                    SDL_RenderCopy(renderer, cherryTexture, NULL, &cherryRect);
             }
    
        consumir_moneda(laberinto, &pos_pac);
    
        if(a!=0)    //solo si se ha apretado alguna de las teclas definidas
            if (mov_pacman(laberinto, &pos_pac, a)) {   //solo cambia la posición del pacman si el movimiento es válido
                pacmanRect.x = pos_pac.x * TILE_SIZE;
                pacmanRect.y = pos_pac.y * TILE_SIZE;
            }
        if (revisar_choque(laberinto, pos_pac, pos_fan1, pos_fan2, pos_fan3, pos_fan4) == 1){
          vidas-=1;
          printf("Vida Perdida\n");

          if (vidas ==0){
            printf("Puntaje final: %d\n", puntaje_pac);
            printf("Juego Finalizado\n");
            quit = 1;
          }
            
        }
        // se mueven los fantasmas antes que se realice un movimiento
        //mov_pacman(laberinto, &pos_pac, a);
        mov_fan(laberinto, &pos_fan1);
        ghost1Rect.x = pos_fan1.x*TILE_SIZE;
        ghost1Rect.y = pos_fan1.y*TILE_SIZE;
        mov_fan(laberinto, &pos_fan2);
        ghost2Rect.x = pos_fan2.x*TILE_SIZE;
        ghost2Rect.y = pos_fan2.y*TILE_SIZE;
        mov_fan(laberinto, &pos_fan3);
        ghost3Rect.x = pos_fan3.x*TILE_SIZE;
        ghost3Rect.y = pos_fan3.y*TILE_SIZE;
        mov_fan(laberinto, &pos_fan4);
        ghost4Rect.x = pos_fan4.x*TILE_SIZE;
        ghost4Rect.y = pos_fan4.y*TILE_SIZE;

        if (revisar_choque(laberinto, pos_pac, pos_fan1, pos_fan2, pos_fan3, pos_fan4) == 1){
            vidas-=1;
            printf("Vida Perdida\n");

            a=0;                                    //se reinicia ubicación y movimiento si es que pacman muere

            pos_fan1.x=13;    pos_fan1.y=13;
            pos_fan2.x=13;    pos_fan2.y=13;
            pos_fan3.x=13;    pos_fan3.y=13;
            pos_fan4.x=13;    pos_fan4.y=13;
            pos_pac.x=1;      pos_pac.y=1;

            ghost1Rect.x = pos_fan1.x*TILE_SIZE;
            ghost1Rect.y = pos_fan1.y*TILE_SIZE;
            ghost2Rect.x = pos_fan2.x*TILE_SIZE;
            ghost2Rect.y = pos_fan2.y*TILE_SIZE;
            ghost3Rect.x = pos_fan3.x*TILE_SIZE;
            ghost3Rect.y = pos_fan3.y*TILE_SIZE;
            ghost4Rect.x = pos_fan4.x*TILE_SIZE;
            ghost4Rect.y = pos_fan4.y*TILE_SIZE;
            pacmanRect.x = pos_pac.x * TILE_SIZE;
            pacmanRect.y = pos_pac.y * TILE_SIZE;

            if (vidas ==0){
                printf("Puntaje final: %d\n", puntaje_pac);
                printf("Juego Finalizado");
                quit = 1;
            }
            
        }
        if(vidas==1)
            SDL_RenderCopy(renderer, live1Texture, NULL, &livesRect);
        else if(vidas==2)
            SDL_RenderCopy(renderer, live2Texture, NULL, &livesRect);
        else if(vidas==3)
            SDL_RenderCopy(renderer, live3Texture, NULL, &livesRect);

        consumir_moneda(laberinto, &pos_pac); //se evalua cada vez que se hacen movimientos si se consume una moneda o no, si habia un fantasma en el lugar no alcanza a consumir la moneda
        
        int uu = puntaje_pac % 10;  
        SDL_RenderCopy(renderer, arr[uu], NULL, &uRect);  //unidad

        int dd = (puntaje_pac / 10) % 10;
        SDL_RenderCopy(renderer, arr[dd], NULL, &dRect);  //decena

        int cc = (puntaje_pac / 100) % 10;  //cien
        SDL_RenderCopy(renderer, arr[cc], NULL, &cRect); 

        int mm = (puntaje_pac / 1000) % 10;  // mil
        SDL_RenderCopy(renderer, arr[mm], NULL, &mRect);  


        int verficador = 0; //verificador que revisa si todas las monedas y bonus fueron atrapados
        for (int i=0;i<ALTO;i++){
            for (int j=0;j<ANCHO;j++){
                if (laberinto[i][j]!=0 && laberinto[i][j]!=1){
                    verficador=1;
                }
            }
        }
        if (verficador==0){ //si el verificador se mantieen, reinicia el nivel devolviendo todas las monedas
            for (int i = 0;i<ALTO;i++){
            for (int j = 0;j<ANCHO;j++){
                laberinto[i][j]=Matriz[i][j];
                }
            }
            contador_niveles+=1; //aumenta 1 el contador de niveles y suma una vida extra x ganar
            if(vidas<3)
                vidas+=1;
            printf("Nivel superado\n");
            printf("Vida conseguida\n");
        }
        if (contador_niveles==3){ //si supera 3 veces el mismo nivel, gana el juego y se cierra automaticamente
            puntaje_pac=puntaje_pac+(vidas*1000); //da puntaje extra en caso de que sobren vidas al finalizar el juego
            printf("Has ganado\nPuntaje total:%d\n",puntaje_pac);
            quit = 1;
        }

        if (contador_niveles==3)
            SDL_RenderCopy(renderer, lvl3Texture, NULL, &lvlRect);
        else if (contador_niveles==2)
            SDL_RenderCopy(renderer, lvl2Texture, NULL, &lvlRect);
        else if (contador_niveles==1)
            SDL_RenderCopy(renderer, lvl1Texture, NULL, &lvlRect);


        // actualizar pantalla
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }


        


    

    // Liberar memoria
    for (int i = 0; i < m; i++) {
        free(Matriz[i]);
    }
    free(Matriz);
    SDL_DestroyTexture(pacmanDTexture);
    SDL_DestroyTexture(pacmanRTexture);
    SDL_DestroyTexture(pacmanUTexture);
    SDL_DestroyTexture(pacmanLTexture);
    SDL_DestroyTexture(pacman1Texture);
    SDL_DestroyTexture(ghost1Texture);
    SDL_DestroyTexture(ghost2Texture);
    SDL_DestroyTexture(ghost3Texture);
    SDL_DestroyTexture(ghost4Texture);
    SDL_DestroyTexture(cherryTexture);
    SDL_DestroyTexture(live1Texture);
    SDL_DestroyTexture(live2Texture);
    SDL_DestroyTexture(live3Texture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(lvl1Texture);
    SDL_DestroyTexture(lvl2Texture);
    SDL_DestroyTexture(lvl3Texture);
    SDL_DestroyTexture(n0Texture);
    SDL_DestroyTexture(n1Texture);
    SDL_DestroyTexture(n2Texture);
    SDL_DestroyTexture(n3Texture);
    SDL_DestroyTexture(n4Texture);
    SDL_DestroyTexture(n5Texture);
    SDL_DestroyTexture(n6Texture);
    SDL_DestroyTexture(n7Texture);
    SDL_DestroyTexture(n8Texture);
    SDL_DestroyTexture(n9Texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}

//gcc -I src/include -I /msys64/mingw64/include/SDL2 -L /msys64/mingw64/lib -o uy MORE2.c funciones.c -lmingw32 -lSDL2main -lSDL2







