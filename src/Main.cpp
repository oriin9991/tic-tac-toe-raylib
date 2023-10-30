#include <iostream>
#include <string>
#include <unistd.h>
#include "raygui.h"
#include "raylib.h"
#include "raymath.h"

#define HIGHT 1000
#define WIDTH 1000

typedef struct Box{ 
	Rectangle box;
	bool selected;
}Box;
typedef struct Ticbox{ 
	Rectangle box;
	int selected;
}Ticbox;

typedef struct Game{
	int turn; 
	bool finnished;
	int won; 
}Game;

void drawlines(int width, int hight){
	for(int i = 0; i < width; i+=width/3){
		DrawLine(i,hight, i,0,BLACK);
	}
	for(int i = 0; i < hight; i+=hight/3){
		DrawLine(width,i, 0,i,BLACK);
	}
}

bool selectBox(Rectangle Rect, Rectangle Mouse){
	return CheckCollisionRecs(Rect,Mouse)&&IsMouseButtonDown(MOUSE_LEFT_BUTTON);
}

//win chacker
int IsWon(Ticbox boxes[9],int index){
	int cntW = 0;
	int cntH = 0;
	for(int i=0;i<9;i+=3){
		for(int j=0;j<3;j++){
			if(boxes[i+j].selected == index)cntW++;
		}
		if(cntW==3)return index;
		cntW = 0;
	}
	for(int i=0; i<3; i++){
		for(int j=0; j<9 ; j+=3){
			if(boxes[i+j].selected==index)cntH++;
		}
		if(cntH==3)return index;
		cntH=0;
	}
	if(boxes[0].selected==index)
		if(boxes[4].selected==index)
			if(boxes[8].selected==index)
				return index;
	if(boxes[2].selected==index)
		if(boxes[4].selected==index)
			if(boxes[6].selected==index)
				return index;
	return 0;
}


//GameLoop
Game game(Ticbox boxes[9],Rectangle mouse,int turn){
	bool end = false;
	drawlines(WIDTH,HIGHT);
	for(int i = 0; i < 9; i++){		
			if (selectBox(boxes[i].box, mouse)&&turn%2!=0&&boxes[i].selected==0&&!end){
				boxes[i].selected = 1;
				std::cout << i<<std::endl;
				turn++;
			}
			else if(selectBox(boxes[i].box, mouse)&&turn%2==0&&boxes[i].selected==0&&!end){
				boxes[i].selected = 2;
				std::cout << i<<std::endl;
				turn++;
			}
		if(boxes[i].selected==1) DrawRectangleRec(boxes[i].box,GREEN);
		if(boxes[i].selected==2) DrawRectangleRec(boxes[i].box,RED);
	}

	int WhoWon=0; 
	if(turn>=5){
		if(IsWon(boxes, 2)){
			end = true;
			WhoWon=2;
		}
		else if(IsWon(boxes, 1)){
			end = true;
			WhoWon=1; 
		}
		else if(turn==10){
			end = true;
			WhoWon=-1; 
		}
	}
	return {turn, end, WhoWon};
	
}


//end screem

Game Rest(Ticbox box[9]){
	for(int i = 0; i < 9; i+=3){
		for(int j = 0; j<3; j++){
			box[i+j]={{(float)j*WIDTH/3, (float)i*HIGHT/9, WIDTH/3, HIGHT/3},0};
		}
	}
	return {1,false,0};
}

Game EndScreen(int whoWon,Rectangle mouse,Box button,Ticbox box[9]){
	button  = {{WIDTH/3,2*HIGHT/3,WIDTH/3,HIGHT/3},false};
	if(whoWon==2){
		DrawText("O Won",WIDTH/2-100,HIGHT/2-100,100,BLACK);
		if(IsKeyPressed(KEY_R)) return Rest(box);
	}
	else if(whoWon==1){
		DrawText("X Won",WIDTH/2-100,HIGHT/2-100,100,BLACK);
		if(IsKeyPressed(KEY_R)) return Rest(box);
	}
	else if(whoWon==-1){
		DrawText("DRAW",WIDTH/2-100,HIGHT/2-100,100,BLACK);
		if(IsKeyPressed(KEY_R)) return Rest(box);
	}
	return {0,true,whoWon};
} 

int main ()
{

	Rectangle mouse; 
	Box button;
	Game g;
	int turn = 1, whoWon = 0;
	bool end = false;
	Ticbox boxes[9];
	for(int i = 0; i < 9; i+=3){
		for(int j = 0; j<3; j++){
			boxes[i+j]={{(float)j*WIDTH/3, (float)i*HIGHT/9, WIDTH/3, HIGHT/3},0};
		}
	}

	InitWindow(HIGHT,WIDTH,"tic tac toe");
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{ 
		BeginDrawing();
			ClearBackground(RAYWHITE);
			
			if (!end){
				g =game(boxes,mouse,turn);
				mouse={GetMousePosition().x,GetMousePosition().y,10,10};
				turn = g.turn;
				end = g.finnished;
				whoWon = g.won;
			}
			if(end){
				mouse={GetMousePosition().x,GetMousePosition().y,10,10};
				g = EndScreen(whoWon,mouse,button,boxes);
				end= g.finnished;
				turn = g.turn;
				whoWon = g.won;
			}
			if(IsKeyDown(KEY_R)&&!end){
				Rest(boxes);
				turn = 1;
			}
			DrawText("PRESS R TO RESTART",0,0,50,BLACK);
		EndDrawing();
	
	}
	CloseWindow();
    return 0;

}

