#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define WIDTH 8
#define HEIGHT 10

char data[HEIGHT][WIDTH];
const char pieces[7][4][4][2] = {
    {   { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }
    }, // Square
    
    {   { { 1, 0 }, { 0, 0 }, { 2, 0 }, { 3, 0 } },
        { { 1, 0 }, { 1, -1 }, { 1, 1 }, { 1, 2 } },
        { { 1, 1 }, { 0, 1 }, { 2, 1 }, { 3, 1 } },
        { { 2, 0 }, { 2, -1 }, { 2, 1 }, { 2, 2 } }
    }, // Line
    
    {   { { 1, 0 }, { 0, 0 }, { 2, 0 }, { 1, 1 } },
        { { 1, 0 }, { 1, 1 }, { 1, -1 }, { 2, 0 } },
        { { 1, 0 }, { 2, 0 }, { 0, 0 }, { 1, -1 } },
        { { 1, 0 }, { 1, -1 }, { 1, 1 }, { 0, 0 } }
    }, // T
    
    {   { { 1, 0 }, { 0, 1 }, { 0, 0 }, { 2, 0 } },
        { { 1, 0 }, { 2, 1 }, { 1, 1 }, { 1, -1 } },
        { { 1, 0 }, { 2, -1 }, { 2, 0 }, { 0, 0 } },
        { { 1, 0 }, { 0, -1 }, { 1, -1 }, { 1, 1 } }
    }, // J
    
    {   { { 1, 0 }, { 0, 0 }, { 2, 0 }, { 2, 1 } },
        { { 1, 0 }, { 1, 1 }, { 1, -1 }, { 2, -1 } },
        { { 1, 0 }, { 2, 0 }, { 0, 0 }, { 0, -1 } },
        { { 1, 0 }, { 1, -1 }, { 1, 1 }, { 0, 1 } }
    }, // L

    {   { { 1, 1 }, { 1, 0 }, { 0, 1 }, { 2, 0 } },
        { { 1, 1 }, { 0, 1 }, { 1, 2 }, { 0, 0 } },
       	{ { 1, 1 }, { 1, 2 }, { 2, 1 }, { 0, 2 } },
       	{ { 1, 1 }, { 2, 1 }, { 1, 0 }, { 2, 2 } },
    },// Z
    
    {   { { 1, 1 }, { 1, 0 }, { 0, 0 }, { 2, 1 } },
        { { 1, 1 }, { 0, 1 }, { 0, 2 }, { 1, 0 } },
        { { 1, 1 }, { 1, 2 }, { 2, 2 }, { 0, 1 } },
        { { 1, 1 }, { 2, 1 }, { 2, 0 }, { 1, 2 } },
    },// S
};

char curr_piece[4][2] = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 0 },
	{ 1, 1 }
};
char create_new_piece = 1;
char left = 0;
char right = 0;
char rotateflag = 0;
char downflag = 0;
char tetris = 0;

int piece;
int rot = 0;


int i = 0;
//char pause = 0;


//FUNCTION DECLARATIONS
void printBoard(void);
void rotate(void);
void clearRow(int row);
void moveDown(void);
void moveLeft(char part[][2]);
void moveRight(char part[][2]);
void checkCompleteRow(void);
void initBoard(void);
void createPiece(void);

void createPiece(void){	
	piece = rand() % 7;
			
	checkCompleteRow();

    rot = 0;
	for (i = 0; i < 4; i++){
		curr_piece[i][0] = pieces[piece][0][i][0];
		curr_piece[i][1] = pieces[piece][0][i][1];
	}
	for(i = 0;i < 4;i++){
		data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
	}

}

void initBoard(void){
	for (i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			data[i][j] = 0;
		}
	}

}

void moveRight(char part[][2]){
	char stop = 0;
	for(i = 0;i<4;i++){
		data[(int)part[i][1]][(int)part[i][0]] = 0;
	}
	for(i = 0;i < 4;i++){
		if(part[i][0] + 1 > (WIDTH - 1) || data[(int)part[i][1]][(int)part[i][0] + 1] != 0){
			stop = 1;
			break;
		}
	}
	if(!stop){
		for(i = 0;i < 4;i++){
			part[i][0] = part[i][0] + 1;
			data[(int)part[i][1]][(int)part[i][0]] = 1;
		}
	}
	else{
		for(i = 0; i < 4;i++){
			data[(int)part[i][1]][(int)part[i][0]] = 1;
		}
			
	}
	stop = 0;

}

void moveLeft(char part[][2]){
	char stop = 0;	
	for(i = 0;i<4;i++){
		data[(int)part[i][1]][(int)part[i][0]] = 0;
	}
	for(i = 0;i < 4;i++){
		if(part[i][0] - 1 < 0 || data[(int)part[i][1]][(int)part[i][0] - 1] != 0){
			stop = 1;
			break;
		}
	}
	if(!stop){
		for(i = 0;i < 4;i++){
			part[i][0] = part[i][0] - 1;
			data[(int)part[i][1]][(int)part[i][0]] = 1;
		}
	}
	else{
		for(i = 0; i < 4;i++){
			data[(int)part[i][1]][(int)part[i][0]] = 1;
		}
			
	}
	stop = 0;

}

void checkCompleteRow(void){
	for(i = 0;i < HEIGHT;i++){
		tetris = 1;
		for(int j = 0; j < WIDTH;j++){
			if(data[i][j] == 0){
				tetris = 0;
				break;
			}

		}
		if(tetris != 0){
			clearRow(i);
			i--;
		}
	}
}

void moveDown(){
	char stop = 0;
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

}
int main(void){
	char key[10] = "9";
	//int stop = 0;

	//int endloop = 0;
	
	//INITIALIZES BOARD
   	initBoard(); 
	printBoard();
		

	//INFINITE LOOP
	for(;;){
		//printBoard();
		if(create_new_piece == 1){
			create_new_piece = 0;
			createPiece();
		}
		scanf("%s",key);
		if(key[0] == 'a'){
			left = 1;
		}
		if(key[0] == 'd'){
			right = 1;
		}
		if(key[0] == 'p'){
			break;
		}
		if(key[0] == 'r'){
			rotateflag = 1;
		}
        if(key[0] == 's'){
            downflag = 1;
        }

		if(right){
			right = 0;
			moveRight(curr_piece);
		}
		if(left){
			left = 0;
			moveLeft(curr_piece);
		}
		if(rotateflag){
			rotateflag = 0;
			rotate();
		}
		
        if(downflag){
            downflag = 0;
			moveDown();
        }
		sleep(1);
		printBoard();
		
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

void rotate(void){
	char newPos[4][2];
	int valid = 0;
    printf("%d\n",rot);
    
    for(i = 0; i < 4; i++){
        newPos[i][0] = pieces[piece][(rot + 1)%4][i][0] - pieces[piece][rot][i][0] + curr_piece[i][0];
        newPos[i][1] = pieces[piece][(rot + 1)%4][i][1] - pieces[piece][rot][i][1] + curr_piece[i][1];
    }


	for(i = 0;i < 4;i++){
		data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 0;
	}

	for(i = 0; i < 4;i++){
		if(newPos[i][0] + 1 > (WIDTH - 1) || newPos[i][0] < 0 || data[(int)newPos[i][1]][(int)newPos[i][0]+1] != 0){
			valid = 0;
			break;
		}
		if(newPos[i][1] + 1 > (HEIGHT - 1) || newPos[i][1] < 0 || data[(int)newPos[i][1]+1][(int)newPos[i][0]] != 0){
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
        rot = (rot + 1) % 4;
	}else{
		for(i = 0; i < 4;i++){
			data[(int)curr_piece[i][1]][(int)curr_piece[i][0]] = 1;
		}
	}
	return;
}

void clearRow(int row){
	//for(int j = 0; j < WIDTH;j++){
	//	data[row][j] = 0;
	//}
	for(int j = row;j >0; j--){
		for(int k = 0; k < WIDTH;k++){
			data[j][k] = data[j - 1][k];
		}
	}

}
