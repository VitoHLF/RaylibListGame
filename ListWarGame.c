#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>


#define screenWidth 450
#define screenHeight 780
#define margins 15

#define topMargin 40
#define bottomMargin 150

#define maxBalls 30
#define ballSize 15
#define ballDrag 1

#define ballRock 0
#define ballPaper 1
#define ballScissor 2
#define playerOwner 0
#define enemyOwner 1

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
int removeMiddle(List* playerHand, int index);
void createBall(Ball* ball, int X, int Y, int ballType, int owner);
void createDeck(List* deck);
void printHand(List* playerDeck, Texture* cards);
void updateBall(Ball *ball);
Vector2 calcDirection(Vector2 ball1, Vector2 ball2);
float Absolute(float number);
//------------------------------------------------------------------------------------

int main(void){
    
    int gameState = playingState, nextInVector, playerPoints=0, enemyPoints=0;
    Ball ballsVector[maxBalls];
    List playerDeck, enemyDeck;
    Vector2 directionAux, throwDirection;
    bool ballsMoving, playerTurn = false, throwing=false;
    float ballSpeedAux;
    Texture2D background[2], tokens[3], tokenBackground, cards[3];
    
    // Initialization
    //--------------------------------------------------------------------------------------
    
    srand(time(NULL));
    
    createDeck(&playerDeck); 
    
    createDeck(&enemyDeck);
    
    
    for(int i=0; i<maxBalls; i++){
        ballsVector[i].ballType = -1;
    }    
    

    InitWindow(screenWidth, screenHeight, "List Game");
    
    background[0] = LoadTexture("assets/fundoMoldura.png");
    background[1] = LoadTexture("assets/fundoRadar.png");
    
    tokens[0] = LoadTexture("assets/tokenRock.png");
    tokens[1] = LoadTexture("assets/tokenPaper.png");
    tokens[2] = LoadTexture("assets/tokenScissor.png");
    tokenBackground = LoadTexture("assets/tokenEnemyBackground.png");
    cards[0] = LoadTexture("assets/cardRock.png");
    cards[1] = LoadTexture("assets/cardPaper.png");
    cards[2] = LoadTexture("assets/cardScissor.png");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    /*createBall(&ballsVector[0], 200, 200, 0, 0);
    ballsVector[0].velocity = (Vector2){0, 0};
    
    createBall(&ballsVector[1], 400, 200, 0, 0);
    ballsVector[1].velocity = (Vector2){0, 0};
    
    createBall(&ballsVector[2], 400, 400, 0, 0);
    ballsVector[2].velocity = (Vector2){0, 0};*/

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
                        if(i!=j && ballsVector[j].ballType >= 0){
                            if(CheckCollisionCircles(ballsVector[i].coords, ballSize, ballsVector[j].coords, ballSize)){ // COLLISIONS
                                directionAux = calcDirection(ballsVector[i].coords, ballsVector[j].coords);
                                ballSpeedAux = (Vector2Length(ballsVector[i].velocity) + Vector2Length(ballsVector[j].velocity)) / 2;
                                
                                ballsVector[j].coords = Vector2Add(ballsVector[j].coords, (Vector2){-directionAux.x*1, -directionAux.y*1});
                                ballsVector[i].coords = Vector2Add(ballsVector[i].coords, Vector2Scale(directionAux, 1));
                                
                                CompareTypes(&ballsVector[i], &ballsVector[j], &playerPoints, &enemyPoints);
                                
                                if(Absolute(ballsVector[i].velocity.x) < 0.5 && Absolute(ballsVector[i].velocity.y) < 0.5){
                                    ballsVector[i].velocity = Vector2Scale(directionAux, ballSpeedAux);
                                }else{
                                    ballsVector[i].velocity = Vector2Reflect(ballsVector[i].velocity, directionAux);
                                    ballsVector[i].velocity = Vector2Invert(ballsVector[i].velocity);
                                    ballsVector[i].velocity = Vector2Normalize(ballsVector[i].velocity);
                                    ballsVector[i].velocity = Vector2Scale(ballsVector[i].velocity, ballSpeedAux);
                                }
                                
                                if(Absolute(ballsVector[j].velocity.x) < 0.5 && Absolute(ballsVector[j].velocity.y) < 0.5){
                                    ballsVector[i].velocity = Vector2Scale(directionAux, -ballSpeedAux);
                                }else{
                                    ballsVector[j].velocity = Vector2Reflect(ballsVector[j].velocity, directionAux);
                                    ballsVector[j].velocity = Vector2Invert(ballsVector[j].velocity);
                                    ballsVector[j].velocity = Vector2Normalize(ballsVector[j].velocity);
                                    ballsVector[j].velocity = Vector2Scale(ballsVector[j].velocity, ballSpeedAux);
                                }
                            } 
                        }
                    }
                    updateBall(&ballsVector[i]);
                    if(Absolute(ballsVector[i].velocity.x) > 0.5 || Absolute(ballsVector[i].velocity.y) > 0.5){ // CHECK IF MOVING
                        ballsMoving = true;
                    }
                }
            }
            
            
            
            //ENEMY TURN
            if(!ballsMoving && !playerTurn){
                for(int i=0; i<maxBalls; i++){
                    if(ballsVector[i].ballType < 0){
                        nextInVector = i;
                        break;
                    }
                }
                
                createBall(&ballsVector[nextInVector] ,225, 40, enemyDeck.front->ballType, enemyOwner);
                throwDirection = (Vector2){rand()%100, rand()%100};
                throwDirection = Vector2Normalize(throwDirection);
                ballsVector[nextInVector].velocity = Vector2Scale(throwDirection, 1000);
                removeFirst(&enemyDeck);
                playerTurn = true;
                ballsMoving = true;
            }
            
            //PLAYER TURN
            if(!ballsMoving && !throwing && playerTurn){
                for(int i=0; i<maxBalls; i++){
                    if(ballsVector[i].ballType < 0){
                        nextInVector = i;
                        break;
                    }
                }
                
                if(CheckCollisionPointRec(GetMousePosition(), (Rectangle){14, 675, 70, 90}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){                                        
                    createBall(&ballsVector[nextInVector] ,217, 626, removeMiddle(&playerDeck, 1), playerOwner);
                    throwing = true;
                }
                if(CheckCollisionPointRec(GetMousePosition(), (Rectangle){14+88, 675, 70, 90}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    createBall(&ballsVector[nextInVector] ,217, 626, removeMiddle(&playerDeck, 2), playerOwner);
                    throwing = true;
                }
                if(CheckCollisionPointRec(GetMousePosition(), (Rectangle){14+88*2, 675, 70, 90}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    createBall(&ballsVector[nextInVector] ,217, 626, removeMiddle(&playerDeck, 3), playerOwner);
                    throwing = true;
                }
                if(CheckCollisionPointRec(GetMousePosition(), (Rectangle){14+88*3, 675, 70, 90}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    createBall(&ballsVector[nextInVector] ,217, 626, removeMiddle(&playerDeck, 4), playerOwner);
                    throwing = true;
                }
            }
            if(throwing){
                throwDirection = Vector2Subtract(GetMousePosition(), ballsVector[nextInVector].coords);                
                
                if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                    throwDirection = Vector2Normalize(throwDirection);
                    ballsVector[nextInVector].velocity = Vector2Scale(throwDirection, 1000);
                    throwing = false;
                    playerTurn = false;
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
            DrawTexture(background[0], 0, 0, WHITE);
            DrawTexture(background[1], 11, 16, WHITE);
            printHand(&playerDeck, &cards);
            DrawText(TextFormat("Pontos: %2d", playerPoints), 20, 20, 20, GREEN);
            DrawText(TextFormat("Inimigo: %2d", enemyPoints), 335, 20, 20, GREEN);
            ClearBackground(RAYWHITE);
            
            if(!ballsMoving && playerTurn) DrawText("Sua vez.", 183, 20, 20, GREEN);
            DrawText(TextFormat("%d", playerDeck.size), 385, 712, 30, WHITE);
            
            
            for(int i=0; i<maxBalls; i++){
                if(ballsVector[i].ballType>=0){
                    if(ballsVector[i].owner == enemyOwner) DrawTextureEx(tokenBackground, Vector2Subtract(ballsVector[i].coords, (Vector2){22.5, 22.5}), 0, 0.5, WHITE);
                    DrawTextureEx(tokens[ballsVector[i].ballType], Vector2Subtract(ballsVector[i].coords, (Vector2){15, 15}), 0, 0.5, WHITE);
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
// GAME FLOW FUNCTIONS
    
void CompareTypes(Ball* ball1, Ball* ball2, int* playerPoints, int* enemyPoints){
    if(ball1->ballType == ballRock){
        if(ball2->ballType == ballScissor){
            if(ball1->owner == playerOwner && ball2->owner == enemyOwner){
                ball2->ballType = -1;
                *playerPoints+=1;
            }
            if(ball1->owner == enemyOwner && ball2->owner == playerOwner){
                ball2->ballType = -1;
                *enemyPoints+=1;
            }
        }
        if(ball2->ballType == ballPaper){
            if(ball1->owner == playerOwner && ball2->owner == enemyOwner){
                ball1->ballType = -1;
                *enemyPoints+=1;
            }
            if(ball1->owner == enemyOwner && ball2->owner == playerOwner){
                ball1->ballType = -1;
                *playerPoints+=1;
            }
        }
    }
    
    if(ball1->ballType == ballPaper){
        if(ball2->ballType == ballRock){
            if(ball1->owner == playerOwner && ball2->owner == enemyOwner){
                ball2->ballType = -1;
                *playerPoints+=1;
            }
            if(ball1->owner == enemyOwner && ball2->owner == playerOwner){
                ball2->ballType = -1;
                *enemyPoints+=1;
            }
        }
        if(ball2->ballType == ballScissor){
            if(ball1->owner == playerOwner && ball2->owner == enemyOwner){
                ball1->ballType = -1;
                *enemyPoints+=1;
            }
            if(ball1->owner == enemyOwner && ball2->owner == playerOwner){
                ball1->ballType = -1;
                *playerPoints+=1;
            }
        }
    }
    
    if(ball1->ballType == ballScissor){
        if(ball2->ballType == ballPaper){
            if(ball1->owner == playerOwner && ball2->owner == enemyOwner){
                ball2->ballType = -1;
                *playerPoints+=1;
            }
            if(ball1->owner == enemyOwner && ball2->owner == playerOwner){
                ball2->ballType = -1;
                *enemyPoints+=1;
            }
        }
        if(ball2->ballType == ballRock){
            if(ball1->owner == playerOwner && ball2->owner == enemyOwner){
                ball1->ballType = -1;
                *enemyPoints+=1;
            }
            if(ball1->owner == enemyOwner && ball2->owner == playerOwner){
                ball1->ballType = -1;
                *playerPoints+=1;
            }
        }
    }
    
}

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
//3
int removeMiddle(List* playerHand, int index){
    Node *aux = playerHand->front;
    Node *remove;
    int ballType;
    if(playerHand->front){
        if(index == 1){
            ballType = aux->ballType;
            remove = aux;
            playerHand->front = aux->next;
        }else{
            for(int i=1; i<index-1; i++){
                aux = aux->next;
            }
            remove = aux->next;
            ballType = aux->next->ballType;
            aux->next = aux->next->next;
            
        }
        
        free(remove);        
        playerHand->size--;
    }
    
    return ballType;
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

void printHand(List* playerDeck, Texture* cards){
    Node* aux;
    if(playerDeck->front) aux = playerDeck->front;
    for(int i=0; i<4; i++){
        if(aux){
            //printCard(aux->ballType, i, cards);
            DrawTexture(cards[aux->ballType], 14+88*i, 675, WHITE);
            aux = aux->next;
        }
    }
    
}

/*void printCard(int ballType, int handSpot, Texture* cards){
    if(ballType == ballRock) DrawRectangle(14+88*handSpot, 675, 70, 90, RED);
    if(ballType == ballPaper) DrawRectangle(14+88*handSpot, 675, 70, 90, GREEN);
    if(ballType == ballScissor) DrawRectangle(14+88*handSpot, 675, 70, 90, BLUE);

    
}*/

// DECK MANAGEMENT
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// BALL MANAGEMENT
void updateBall(Ball *ball){
    if(ball->velocity.x * ball->velocity.x > 0.5 && ball->velocity.y * ball->velocity.y > 0.5){
        ball->coords.x += ball->velocity.x * GetFrameTime();
        ball->coords.y += ball->velocity.y * GetFrameTime();
        ball->velocity.x -= ball->velocity.x * ballDrag * GetFrameTime();
        ball->velocity.y -= ball->velocity.y * ballDrag * GetFrameTime();        
    }else{
        ball->velocity.x = 0;
        ball->velocity.y = 0;
    }
    
    if(ball->coords.x > screenWidth - margins - ballSize){
        ball->velocity.x = -Absolute(ball->velocity.x);
    }
    if(ball->coords.x < margins + ballSize){
        ball->velocity.x = Absolute(ball->velocity.x);
    }
    if(ball->coords.y > screenHeight - bottomMargin - ballSize){
        ball->velocity.y = -1 * Absolute(ball->velocity.y);
    }
    if(ball->coords.y < topMargin + ballSize){
        ball->velocity.y = Absolute(ball->velocity.y);
    }
}

void createBall(Ball* ball, int X, int Y, int ballType, int owner){
    ball->ballType = ballType;
    ball->coords = (Vector2){X, Y};
    ball->velocity = (Vector2){0, 0};
    ball->owner = owner;
}

Vector2 calcDirection(Vector2 ball1, Vector2 ball2){
    Vector2 newDirection;
    
    newDirection = Vector2Subtract(ball1, ball2);
    newDirection = Vector2Normalize(newDirection);
    
    return newDirection;    
}

// BALL MANAGEMENT
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// MISC
float Absolute(float number){
    float newNumber = number;
    
    if(newNumber<0) newNumber = -newNumber;
    
    return newNumber;
}
// MISC
//--------------------------------------------------------------------------------------
