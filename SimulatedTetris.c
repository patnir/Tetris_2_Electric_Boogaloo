#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define WIDTH 8
#define HEIGHT 16

char data[HEIGHT][WIDTH];
const char pieces[7][4][2] = {
	{ { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }, // Square
	{ { 1, 0 }, { 0, 0 }, { 2, 0 }, { 3, 0 } }, // Line
	{ { 1, 0 }, { 0, 0 }, { 2, 0 }, { 1, 1 } }, // T
	{ { 1, 0 }, { 0, 1 }, { 0, 0 }, { 2, 0 } }, // J
	{ { 1, 0 }, { 0, 0 }, { 2, 0 }, { 2, 1 } }, // L
	{ { 1, 1 }, { 1, 0 }, { 0, 1 }, { 2, 0 } }, // Z
	{ { 1, 1 }, { 1, 0 }, { 0, 0 }, { 2, 1 } }  // S
};

char curr_piece[4][2] = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 0 },
	{ 1, 1 }
};
char create_new_piece = 0;
char left = 0;
char right = 0;
char rotateflag = 0;
int i = 0;
//char pause = 0;


//FUNCTION DECLARATIONS
void printBoard(void);
void rotate(void);

int main(void){
	char key = '9';
	int stop = 0;
	//int endloop = 0;
	
	//INITIALIZES BOARD
	for (i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			data[i][j] = 0;
		}
	}

	for (i = 0; i < 4; i++){
		curr_piece[i][0] = pieces[0][i][0];
		curr_piece[i][1] = pieces[0][i][1];
	}
	for(i = 0; i < 4;i++){
		data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
	}
	printBoard();
		

	//INFINITE LOOP
	for(;;){ 
		int random = rand() % 8;
		//printBoard();
		if(create_new_piece == 1){
			create_new_piece = 0;
			for (i = 0; i < 4; i++){
				curr_piece[i][0] = pieces[random][i][0];
				curr_piece[i][1] = pieces[random][i][1];
			}
			for(i = 0;i < 4;i++){
				data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
			}

		
		}
		scanf("%c",&key);
		if(key == 'a'){
			left = 1;
		}
		if(key == 'd'){
			right = 1;
		}
		if(key == 'p'){
			break;
		}
		if(key == 'r'){
			rotateflag = 1;
		}

		if(right){
			right = 0;
			for(i = 0;i<4;i++){
				data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 0;
			}
			for(i = 0;i < 4;i++){
				if(curr_piece[i][0] + 1 > (WIDTH - 1) || data[(int)curr_piece[i][1]][(int)curr_piece[i][0] + 1] != 0){
					stop = 1;
					break;
				}
			}
			if(!stop){
				for(i = 0;i < 4;i++){
					curr_piece[i][0] = curr_piece[i][0] + 1;
					data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
				}
			}
			else{
				for(i = 0; i < 4;i++){
					data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
				}

				create_new_piece = 1;
			
			}
			stop = 0;

		}
		if(left){
			left = 0;
			for(i = 0;i<4;i++){
				data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 0;
			}
			for(i = 0;i < 4;i++){
				if(curr_piece[i][0] - 1 < 0 || data[(int)curr_piece[i][1]][(int)curr_piece[i][0] - 1] != 0){
					stop = 1;
					break;
				}
			}
			if(!stop){
				for(i = 0;i < 4;i++){
					curr_piece[i][0] = curr_piece[i][0] - 1;
					data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
				}
			}
			else{
				for(i = 0; i < 4;i++){
					data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
				}

		//		create_new_piece = 1;
			
			}
			stop = 0;

			
		}
		if(rotateflag){
			rotateflag = 0;
			rotate();
		}
		
		
		//Remove Current Piece from Data
		for(i = 0;i < 4;i++){
			data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 0;
		}
		//Check if space below is occupied
		for(i = 0; i < 4;i++){
			if(curr_piece[i][1] + 1 > (HEIGHT - 1) || data[(int)curr_piece[i][1] + 1][(int)curr_piece[i][0]] != 0){
				stop = 1;
				break;
			}	
		}
		if(!stop){
			for(i = 0;i < 4;i++){
				curr_piece[i][1] = curr_piece[i][1] + 1;
				data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
			}
		}
		else{
			for(i = 0; i < 4;i++){
				data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
			}

			create_new_piece = 1;
			stop = 0;
		}

		sleep(1);
		printBoard();
		
		//for (int i = 0; i < 4; i++){
		//	data[curr_piece[i][0] + 3][curr_piece[i][1] + 3] = 1;
		//}
		/*
		printBoard();
		rotate();
		printBoard();
		rotate();
		printBoard();
		rotate();
		printBoard();
		*/
	}

	return 1;
}

void printBoard(void){
	for (i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			printf("%d ", data[i][j]);
		}
		printf("\n");
	}
	printf("\n---------------------\n");
}

void rotate(){
	char newPos[4][2];
	int valid = 0;
	newPos[0][0] = curr_piece[0][0];
	newPos[0][1] = curr_piece[0][1];
	int temp = 0;
// { { 1, 0 }, { 0, 0 }, { 2, 0 }, { 1, 1 } }
	temp = curr_piece[1][1] - curr_piece[0][1];
	newPos[1][1] = (curr_piece[0][1] + curr_piece[1][0]) - curr_piece[0][0];
	newPos[1][0] = curr_piece[0][0] - temp;
	temp = curr_piece[2][1] - curr_piece[0][1];
	newPos[2][1] = (curr_piece[0][1] + curr_piece[2][0]) - curr_piece[0][0];
	newPos[2][0] = (curr_piece[0][0] - temp);
	temp = curr_piece[3][1] - curr_piece[0][1];
	newPos[3][1] = (curr_piece[0][1] + curr_piece[3][0]) - curr_piece[0][0];
	newPos[3][0] = curr_piece[0][0] - temp;

	for(i = 0;i < 4;i++){
		data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 0;
	}

	for(i = 0; i < 4;i++){
		if(newPos[i][0] + 1 > (WIDTH - 1) || newPos[i][0] - 1 < 0 || data[(int)newPos[i][1]][(int)newPos[i][0]+1] != 0){
			valid = 0;
			break;
		}
		if(newPos[i][1] + 1 > (HEIGHT - 1) || data[(int)newPos[i][1]+1][(int)newPos[i][0]] != 0){
			valid = 0;
			break;
		}
		else{
			valid = 1;
		}
	}
	if(valid == 1){

		for(i = 0; i < 4;i++){
			curr_piece[i][0] = newPos[i][0];
			curr_piece[i][1] = newPos[i][1];
		}
		for (i = 0; i < 4; i++){
			data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
		}
	}else{
		for(i = 0; i < 4;i++){
			data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
		}
	}
	return;
}

