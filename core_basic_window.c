#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define screenWidth 450
#define screenHeight 780
#define margins 20

#define maxBalls 30
#define ballSize 15
#define ballDrag 1

#define playingState 1

typedef struct Ball{
    int ballType;
    Vector2 coords;
    Vector2 velocity;    
}Ball;

typedef struct Node{
 	int ballType;
 	struct Node* next;
}Node;

typedef struct list{
    Node *front;
    int size;
}List;

void createList(List* lista);
void addFirst(List* lista, int ballType);
void removeFirst(List* lista);
void addLast(List* lista, int ballType);
void createBall(Ball* ball, int X, int Y, int ballType);

//------------------------------------------------------------------------------------
int main(void)
{
    int gameState = 1;
    Ball ballsVector[maxBalls];
    
    // Initialization
    //--------------------------------------------------------------------------------------

    
    for(int i=0; i<maxBalls; i++){
        ballsVector[i].ballType = -1;
    }

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    createBall(&ballsVector[0], 200, 200, 0);
    ballsVector[0].velocity = (Vector2){100, 100};

    // Main game loop
    while (!WindowShouldClose())    
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // IN GAME
        if(gameState == 1){
            for(int i=0; i<maxBalls; i++){
                if(ballsVector[i].ballType >= 0){
                    updateBall(&ballsVector[i]);
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
        //----------------------------------------------------------------------------------
        // PLAYING STATE
        if(gameState == 1){
            ClearBackground(RAYWHITE);
            DrawRectangle(margins, margins, screenWidth-margins*2, screenHeight-margins*2, BLACK);
            
            for(int i=0; i<maxBalls; i++){
                if(ballsVector[i].ballType >= 0){
                    DrawCircleV(ballsVector[i].coords, ballSize, WHITE);
                }
            }
        }
        // PLAYING STATE
        //----------------------------------------------------------------------------------
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------
// LIST MANAGEMENT

void createList(List* lista){
	lista->front = NULL;
	lista->size = 0;
}

void addFirst(List* lista, int ballType){
	Node* novo = malloc(sizeof(Node));
	if(novo){
		novo->ballType = ballType;
		novo->next = lista->front;
		lista->front = novo;
		lista->size++;
	}else{
		printf("Erro ao criar novo no\n");
	}
}

void removeFirst(List* lista){
    Node *aux;
    
    if(lista->front){
        aux = lista->front;
        lista->front = lista->front->next;
        free(aux);
    }
}

void addLast(List* lista, int ballType){
	Node *novo, *aux;
	aux = lista->front;
	novo = malloc(sizeof(Node));
	if(novo){
		novo->ballType = 0;
		novo->next = NULL;

		if(lista->front == NULL){
			lista->front = novo;
		}
		else{
			while(aux->next){
				aux = aux->next;
			}
			aux->next = novo;
		}
	}
}

// LIST MANAGEMENT
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// BALL MANAGEMENT
void updateBall(Ball *ball){
    if(ball->velocity.x * ball->velocity.x > 1 && ball->velocity.y * ball->velocity.y > 1){
        ball->coords.x += ball->velocity.x * GetFrameTime();
        ball->coords.y += ball->velocity.y * GetFrameTime();
        ball->velocity.x -= ballDrag * GetFrameTime();
        ball->velocity.y -= ballDrag * GetFrameTime();
        
    }else{
        ball->velocity.x = 0;
        ball->velocity.y = 0;
    }
    
    if(ball->coords.x > screenWidth-margins){
        ball->velocity.x = -ball->velocity.x;
    }
    if(ball->coords.x < margins){
        ball->velocity.x = -ball->velocity.x;
    }
    if(ball->coords.y > screenHeight - margins){
        ball->velocity.y = -ball->velocity.y;
    }
    if(ball->coords.y < margins){
        ball->velocity.y = -ball->velocity.y;
    }
}

void createBall(Ball* ball, int X, int Y, int ballType){
    ball->ballType = ballType;
    ball->coords = (Vector2){X, Y};
    ball->velocity = (Vector2){0, 0};
}

// BALL MANAGEMENT
//--------------------------------------------------------------------------------------
