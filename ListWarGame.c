#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>


#define screenWidth 450
#define screenHeight 780
#define margins 20

#define maxBalls 30
#define ballSize 15
#define ballDrag 1

#define ballRock 0
#define ballPaper 1
#define ballScissor 2
#define playerOwner 0
#define playerOwner 1

#define handSize 6


#define playingState 1

typedef struct Ball{
    int ballType;
    int owner;
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
void removeMiddle(List* playerHand, int index);
void createBall(Ball* ball, int X, int Y, int ballType, int owner);
void createDeck(List* deck);
void printHand(List* playerDeck);
void updateBall(Ball *ball);
void printBall(int ballType, int offset);
Vector2 calcDirection(Vector2 ball1, Vector2 ball2);
//------------------------------------------------------------------------------------

int main(void){
    
    int gameState = playingState, nextInVector;
    Ball ballsVector[maxBalls];
    List playerDeck, enemyDeck;
    Vector2 directionAux, throwDirection;
    bool ballsMoving, playerTurn, throwing=false;
    float ballSpeedAux;
    
    // Initialization
    //--------------------------------------------------------------------------------------
    
    srand(time(NULL));
    
    createDeck(&playerDeck); 
    
    createDeck(&enemyDeck);
    
    
    for(int i=0; i<maxBalls; i++){
        ballsVector[i].ballType = -1;
    }

    InitWindow(screenWidth, screenHeight, "List Game");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    createBall(&ballsVector[0], 200, 200, 0, 0);
    ballsVector[0].velocity = (Vector2){400, 400};
    
    createBall(&ballsVector[1], 400, 200, 0, 0);
    ballsVector[1].velocity = (Vector2){400, 400};
    
    createBall(&ballsVector[2], 400, 400, 0, 0);
    ballsVector[2].velocity = (Vector2){100, 400};

    // Main game loop
    while (!WindowShouldClose())    
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // IN GAME
        if(gameState == playingState){
            ballsMoving = false;
            //BALL UPDATE
            for(int i=0; i<maxBalls; i++){ // ITERATE THROUGH BALLS
                if(ballsVector[i].ballType >= 0){
                    for(int j=0; j<maxBalls; j++){ //COMPARE WITH OTHERS
                        if(i!=j){
                            if(CheckCollisionCircles(ballsVector[i].coords, ballSize, ballsVector[j].coords, ballSize)){ // COLLISIONS
                                directionAux = calcDirection(ballsVector[i].coords, ballsVector[j].coords);
                                ballSpeedAux = Vector2Length(ballsVector[i].velocity);
                                
                                ballsVector[i].velocity = Vector2Reflect(ballsVector[i].velocity, directionAux);
                                ballsVector[i].velocity = Vector2Invert(ballsVector[i].velocity);
                                ballsVector[i].velocity = Vector2Normalize(ballsVector[i].velocity);
                                ballsVector[i].velocity = Vector2Scale(ballsVector[i].velocity, Vector2Length(ballsVector[j].velocity));
                                
                                ballsVector[j].velocity = Vector2Reflect(ballsVector[j].velocity, directionAux);
                                ballsVector[j].velocity = Vector2Invert(ballsVector[j].velocity);
                                ballsVector[j].velocity = Vector2Normalize(ballsVector[j].velocity);
                                ballsVector[j].velocity = Vector2Scale(ballsVector[j].velocity, ballSpeedAux);                              
                                
                            } 
                        }
                    }
                    updateBall(&ballsVector[i]);
                    if(ballsVector[i].velocity.x != 0 || ballsVector[i].velocity.x != 0 ){ // CHECK IF MOVING
                        ballsMoving = true;
                    }
                }
            }
            
            
            
            //ENEMY ROUND
            
            //PLAYER ROUND         
            if(!ballsMoving && !throwing){
                throwing = true;
                for(int i=0; i<maxBalls; i++){
                    if(ballsVector[i].ballType < 0){
                        nextInVector = i;
                        break;
                    }
                }
                
                createBall(&ballsVector[nextInVector] ,250, 700, 0, 0);
            }
            if(throwing){
                throwDirection = Vector2Subtract(GetMousePosition(), ballsVector[nextInVector].coords);
                throwDirection = Vector2Normalize(throwDirection);
                
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    ballsVector[nextInVector].velocity = Vector2Scale(throwDirection, 700);
                }
            }
            
        }
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
        //----------------------------------------------------------------------------------
        // PLAYING STATE
        if(gameState == playingState){
            ClearBackground(RAYWHITE);
            DrawRectangle(margins, margins, screenWidth-margins*2, screenHeight-margins*2, BLACK);
            
            for(int i=0; i<maxBalls; i++){
                if(ballsVector[i].ballType >= 0){
                    DrawCircleV(ballsVector[i].coords, ballSize, WHITE);
                }
            }
            
            //printHand(&playerDeck);
            
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
// GAME FLOW FUNCTIONS
    
    

// GAME FLOW FUNCTIONS
//--------------------------------------------------------------------------------------



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
	}
}

void removeFirst(List* lista){
    Node *aux;
    
    if(lista->front){
        aux = lista->front;
        lista->front = lista->front->next;
        free(aux);
        lista->size--;
    }
}

void addLast(List* lista, int ballType){
	Node *novo, *aux;
	novo = malloc(sizeof(Node));
	if(novo){
        aux = lista->front;
		novo->ballType = ballType;
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
        lista->size++;
	}
}

void removeMiddle(List* playerHand, int index){
    Node *aux = playerHand->front;
    Node *remove;
    if(index<=6 && index>0 && playerHand->front){
        for(int i=1; i<index-1; i++){
            aux = aux->next;
        }
        remove = aux->next;
        aux->next = aux->next->next;
        free(remove);        
        playerHand->size--;
    }
    
    
}

// LIST MANAGEMENT
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// DECK MANAGEMENT

void createDeck(List* deck){
    int newCardID;
    for(int i=0; i<20; i++){
        newCardID = rand()%3;
        addFirst(deck, newCardID);
    }
}

void printHand(List* playerDeck){
    Node* aux;
    if(playerDeck->front) aux = playerDeck->front;
    for(int i=0; i<6; i++){
        if(aux){
            printBall(aux->ballType, i);
            aux = aux->next;
        }
    }
    
}

void printBall(int ballType, int offset){
    if(ballType == ballRock) DrawRectangle(50+offset*50, 700, 75, 80, RED);
    if(ballType == ballPaper) DrawRectangle(50+offset*50, 700, 75, 80, GREEN);
    if(ballType == ballScissor) DrawRectangle(50+offset*50, 700, 75, 80, BLUE);
}

// DECK MANAGEMENT
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// BALL MANAGEMENT
void updateBall(Ball *ball){
    if(ball->velocity.x * ball->velocity.x > 1 && ball->velocity.y * ball->velocity.y > 1){
        ball->coords.x += ball->velocity.x * GetFrameTime();
        ball->coords.y += ball->velocity.y * GetFrameTime();
        ball->velocity.x -= ball->velocity.x * ballDrag * GetFrameTime();
        ball->velocity.y -= ball->velocity.y * ballDrag * GetFrameTime();
        
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

void createBall(Ball* ball, int X, int Y, int ballType, int owner){
    ball->ballType = ballType;
    ball->coords = (Vector2){X, Y};
    ball->velocity = (Vector2){0, 0};
}

Vector2 calcDirection(Vector2 ball1, Vector2 ball2){
    Vector2 newDirection;
    
    newDirection = Vector2Subtract(ball1, ball2);
    newDirection = Vector2Normalize(newDirection);
    
    return newDirection;    
}

// BALL MANAGEMENT
//--------------------------------------------------------------------------------------
