/*
************************************************************************
 ECE 362 - Mini-Project C Source File - Spring 2017
***********************************************************************
	 	   			 		  			 		  		
 Team ID: < 13 >
 Project Name: < ? >
 Team Members: Michael Toner, Rahul Patni, Yash Bharatula, Alex Medoff
   - Team/Doc Leader: < ? >      Signature: ______________________
   
   - Software Leader: < ? >      Signature: ______________________
   - Interface Leader: < ? >     Signature: ______________________
   - Peripheral Leader: < ? >    Signature: ______________________
 Academic Honesty Statement:  In signing above, we hereby certify that we 
 are the individuals who created this HC(S)12 source file and that we have
 not copied the work of any other student (past or present) while completing 
 it. We understand that if we fail to honor this agreement, we will receive 
 a grade of ZERO and be subject to possible disciplinary action.
***********************************************************************
 The objective of this Mini-Project is to .... < ? >
***********************************************************************
 List of project-specific success criteria (functionality that will be
 demonstrated):
 1.
 2.
 3.
 4.
 5.
***********************************************************************
  Date code started: < ? >
  Update history (add an entry every time a significant change is made):
  Date: < ? >  Name: < ? >   Update: < ? >
  Date: < ? >  Name: < ? >   Update: < ? >
  Date: < ? >  Name: < ? >   Update: < ? >
***********************************************************************
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>
/* Special ASCII characters */
#define CR 0x0D		// ASCII return 
#define LF 0x0A		// ASCII new line 

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS 0x10		// RS pin mask (PTT[4])
#define RW 0x20		// R/W pin mask (PTT[5])
#define LCDCLK 0x40	// LCD EN/CLK pin mask (PTT[6])

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F	// LCD initialization command
#define LCDCLR 0x01	// LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	// LCD cursor move instruction
#define LINE1 = 0x80	// LCD line 1 cursor position
#define LINE2 = 0xC0	// LCD line 2 cursor position

#define RED 1
#define BLUE 2
#define PURPLE 3
#define GREEN 4
#define YELLOW 5
#define CYAN 6
#define WHITE 7


#define R1 PTT_PTT7
#define B1 PTT_PTT6
#define G1 PTT_PTT5

#define R2 PTM_PTM1
#define B2 PTM_PTM2
#define G2 PTM_PTM3

#define A PTT_PTT4
#define B PTT_PTT3
#define C PTT_PTT2
#define LEDCLK PTT_PTT1
#define LAT PTT_PTT0
#define OE PTM_PTM0
#define WIDTH 16
#define HEIGHT 32
#define P1UD ATDDR0H
#define P1LR ATDDR1H
#define rotbutton PORTAD0_PTAD6

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);
void displayT(void);
void clearRow(int row);
void rotate(void);
void moveLeft(char part[][2]);
void moveRight(char part[][2]);
void checkCompleteRow(void);
void initBoard(void);
void createPiece(void);
void addPiece(char part[][2],char color);
void removePiece(char part[][2]);
void gameoverBoard(void);
void jumpDown(char part[][2]);

/* Flag Declarations */
char rowCnt = 0;
char make = 0;
char row = 0;
int prev = 0;
int curr = 0;
char leftbutton = 0;
char rightbutton = 0;
char rotflag = 0;
char leftprev = 1;
char rightprev = 1;
char rotprev = 1;
char count;
char random = 0;
char piece = 5;
char tetris = 0;
char rot = 0;
char gameover = 0;
char rticnt = 0;
char jumpDownFlag = 0;
unsigned char pwmctr = 0;
unsigned char songctr = 0;

const char pieces[7][4][4][2] = {
    {   { { 1, 1 }, { 1, 0 }, { 0, 1 }, { 2, 0 } },
        { { 1, 1 }, { 0, 1 }, { 1, 2 }, { 0, 0 } },
       	{ { 1, 1 }, { 1, 2 }, { 2, 1 }, { 0, 2 } },
       	{ { 1, 1 }, { 2, 1 }, { 1, 0 }, { 2, 2 } },
    },// Z
    
    {   { { 1, 0 }, { 0, 1 }, { 0, 0 }, { 2, 0 } },
        { { 1, 0 }, { 2, 1 }, { 1, 1 }, { 1, -1 } },
        { { 1, 0 }, { 2, -1 }, { 2, 0 }, { 0, 0 } },
        { { 1, 0 }, { 0, -1 }, { 1, -1 }, { 1, 1 } }
    }, // J
    
    {   { { 1, 0 }, { 0, 0 }, { 2, 0 }, { 1, 1 } },
        { { 1, 0 }, { 1, 1 }, { 1, -1 }, { 2, 0 } },
        { { 1, 0 }, { 2, 0 }, { 0, 0 }, { 1, -1 } },
        { { 1, 0 }, { 1, -1 }, { 1, 1 }, { 0, 0 } }
    }, // T
    
    {   { { 1, 1 }, { 1, 0 }, { 0, 0 }, { 2, 1 } },
        { { 1, 1 }, { 0, 1 }, { 0, 2 }, { 1, 0 } },
        { { 1, 1 }, { 1, 2 }, { 2, 2 }, { 0, 1 } },
        { { 1, 1 }, { 2, 1 }, { 2, 0 }, { 1, 2 } },
    },// S
    
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
	
    {   { { 1, 0 }, { 0, 0 }, { 2, 0 }, { 2, 1 } },
        { { 1, 0 }, { 1, 1 }, { 1, -1 }, { 2, -1 } },
        { { 1, 0 }, { 2, 0 }, { 0, 0 }, { 0, -1 } },
        { { 1, 0 }, { 1, -1 }, { 1, 1 }, { 0, 1 } }
    }, // L
};
/*
Note = PWMPER/PWMDTY (Frequency)
F# = 255/128 (~370 Hz)
G = 239/120  (~392 Hz)
A = 215/107  (~440 Hz)
Bb = 203/101 (~466.16 Hz)
C = 181/90   (~523.25 Hz)
D = 161/80   (~587.33 Hz)
Eb = 152/76  (~622.25 Hz)
F = 135/68   (~698.46 Hz)
G = 121/60   (~784 Hz)
F# = 128/64  (~740 Hz)
*/
const char notes[10][2] = {{255,128},{239,120},{215,107},{203,101},{181,90},{161,80},{152,76},{135,68},{121,60},{128,64}};
const char song[128] = {
  5,5,2,3,4,4,3,2,
  1,1,1,3,5,5,4,3,
  2,2,2,3,4,4,5,5,
  3,3,1,1,1,1,2,3,
  4,4,4,6,8,8,7,6,
  5,5,5,3,5,5,4,3,
  2,2,2,3,4,4,5,5,
  3,3,1,1,1,1,-1,-1,
  5,5,5,5,3,3,3,3,
  4,4,4,4,2,2,2,2,
  3,3,3,3,1,1,1,1,
  0,0,0,0,2,2,-1,-1,
  5,5,5,5,3,3,3,3,
  4,4,4,4,2,2,2,2,
  3,3,5,5,8,8,8,8,
  9,9,9,9,-1,-1,-1,-1
};
  
char curr_piece[4][2];
char create_new_piece = 1;
char data[WIDTH][HEIGHT];


/* Variable declarations */
	 	   		
/*	 	   		
***********************************************************************
 Initializations
***********************************************************************
*/

void  initializations(void) {

/* Set the PLL speed (bus clock = 24 MHz) */
  CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
  PLLCTL = PLLCTL | 0x40; //; turn on PLL
  SYNR = 0x02;            //; set PLL multiplier
  REFDV = 0;              //; set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; //; engage PLL

/* Disable watchdog timer (COPCTL register) */
  COPCTL = 0x40   ; //COP off; RTI and COP stopped in BDM-mode

/* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
  SCIBDH =  0x00; //set baud rate to 9600
  SCIBDL =  0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
  SCICR1 =  0x00; //$9C = 156
  SCICR2 =  0x0C; //initialize SCI for program-driven operation
  DDRB   =  0x10; //set PB4 for output mode
  PORTB  =  0x10; //assert DTR pin on COM port
  
  initBoard();
  
/* Initialize peripherals */
  DDRT = 0xFF;
  DDRM = 0x0F;
  DDRAD = 0b00011100;
  ATDDIEN = 0b11011100;
  TSCR2 = 0x0C;
  TIOS = 0xC0;
  TIE = 0xC0;
  TSCR1 = 0x80;
  TCTL1 = 0x00;
  TCTL2 = 0x00;
  
  TC7 = 20000;
  TC6 = 60000;
            
/* Initialize interrupts */
  OE = 1;
/* Initialize RTI for 2.048 ms interrupt rate */  
  RTICTL = 0x1F;
  CRGINT = 0x80;
//Initialize ATD
  ATDCTL2 = 0b10000000; //Must change these for 2 joysticks
  ATDCTL3 = 0b00100000;
  ATDCTL4 = 0b10000101;
  
/* Initialize PWM */
  MODRR |= 0b00010000;
  PWME_PWME4 = 1;
  PWMPOL |= 0b00100000;
  PWMCTL = 0;
  PWMCAE = 0;
  PWMCLK_PCLK4 = 1;
  PWMPRCLK = 0b00000000;
  PWMSCLA = 0x7F;
  PWMPER4 = 215;
  PWMDTY4 = 215 / 2; // 
  
  

   
}

	 		  			 		  		
/*	 		  			 		  		
***********************************************************************
Main
***********************************************************************
*/
void main(void) {
  char stop = 0;
  //char add = 0;
  //char broken = 0;
  //char newx,newy;
  //char x,y;
  //char newdata[4][2] = {{0,0},{0,0},{0,0},{0,0}};    
  DisableInterrupts
	initializations(); 		  			 		  		
	EnableInterrupts;
                                                                                                                        
 for(;;) {
  /* < start of your main loop > */ 
   if(create_new_piece) {
     	create_new_piece = 0;
	createPiece();
   }
   // Check Left Move
   if(leftbutton && !gameover) {
      leftbutton = 0;
      moveLeft(curr_piece); 
   }
   if(rightbutton & !gameover) {
      rightbutton = 0;
      moveRight(curr_piece);
   }
   if(rotflag){
      rotflag = 0;
      rotate();  
   }
   if(jumpDownFlag){
    jumpDownFlag = 0;
    jumpDown(curr_piece);
    
   }
   
   
   
   } /* loop forever */
   
}   /* do not leave main */

void createPiece(void){
  char i;	
	piece = (piece + 1) % 7;		
	checkCompleteRow();
  	rot = 0;
	for (i = 0; i < 4; i++){
		curr_piece[i][0] = pieces[piece][0][i][0] + WIDTH/2;
		curr_piece[i][1] = pieces[piece][0][i][1];
	}
	for(i = 0; i < 4; i++){
		if(data[curr_piece[i][0]][curr_piece[i][1]] != 0){
			gameover = 1;
		}
	}
	if(gameover){
		gameoverBoard();
	}else{
		addPiece(curr_piece,piece);
	}
}

void checkCompleteRow(void){
  char i;
  char j;
	for(i = 0; i < HEIGHT; i++){
		tetris = 1;
		for(j = 0; j < WIDTH; j++){
			if(data[j][i] == 0){
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

void initBoard(void){
  char i;
  char j;
	for (i = 0; i < HEIGHT; i++){
		for (j = 0; j < WIDTH; j++){
			data[j][i] = 0;
		}
	}

}

void gameoverBoard(void){
  char i;
  char j;
  
  initBoard();
  
	for(i = 0; i < HEIGHT;i+=2){
		for(j = 0; j < WIDTH;j+=2){
			data[j][i] = 0;
			data[j + 1][i] = RED;
		}
	}
	
	for(i = 1; i < HEIGHT;i+=2){
		for(j = 0; j < WIDTH;j+=2){
			data[j][i] = RED;
			data[j + 1][i] = 0;
		}
	}
}

void addPiece(char part[][2],char color){
  char i;
	for(i = 0;i < 4;i++){
		data[part[i][0]][part[i][1]] = (color + 1);
	}
}

void removePiece(char part[][2]){
	char i;
	for(i = 0;i < 4;i++){
		data[part[i][0]][part[i][1]] = 0;
	}
}

void jumpDown(char part[][2]){
  char i;
  char stop = 0;
  while(!stop){
    	  // Remove current piece from data
      for(i = 0;i < 4;i++){
        data[part[i][0]][part[i][1]] = 0;
      }
      // Check if there is something below it, or its at the bottom
      for(i = 0;i < 4;i++) {
        if(part[i][1] + 1 > 31 || data[part[i][0]][part[i][1] + 1] != 0){
          stop = 1;
          break;
        }
      }
      // If there's not, move it down
      if(!stop){
         //move down;
         for(i = 0;i < 4;i++) {
          data[part[i][0]][++part[i][1]] = piece + 1;
         }
      } 
  }
  for(i = 0;i < 4;i++)
    data[part[i][0]][part[i][1]] = piece + 1;
    create_new_piece = 1;
}

void rotate(void){
  char i;
	char newPos[4][2];
	char valid = 0;
    
  for(i = 0; i < 4; i++){
      newPos[i][0] = pieces[piece][(rot + 1)%4][i][0] - pieces[piece][rot][i][0] + curr_piece[i][0];
      newPos[i][1] = pieces[piece][(rot + 1)%4][i][1] - pieces[piece][rot][i][1] + curr_piece[i][1];
  }
  removePiece(curr_piece);
	for(i = 0; i < 4;i++){
		if(newPos[i][0] + 1 > (WIDTH - 1) || newPos[i][0] < 0 || data[newPos[i][0] + 1][newPos[i][1]] != 0){
			valid = 0;
			break;
		}
		if(newPos[i][1] + 1 > (HEIGHT - 1) || newPos[i][1] < 0 || data[newPos[i][0]][newPos[i][1] + 1] != 0){
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
    rot = (rot + 1) % 4;
	}
	addPiece(curr_piece,piece);
	return;
}

void clearRow(int row){
  char j;
  char k;
	for(j = row;j >0; j--){
		for(k = 0; k < WIDTH; k++){
			data[k][j] = data[k][j-1];
		}
	}

}

void moveLeft(char part[][2]){
	char stop = 0;
	char i;	
	removePiece(part);

	for(i = 0;i < 4;i++){
		if(part[i][0] - 1 < 0 || data[part[i][0]-1][part[i][1]] != 0){
			stop = 1;
			break;
		}
	}
	if(!stop){
		for(i = 0;i < 4;i++){
			part[i][0] = part[i][0] - 1;
			data[part[i][0]][part[i][1]] = piece + 1; //update for 2 players
		}
	}
	else{
		addPiece(part,piece);
	
	}
}

void moveRight(char part[][2]){
  char i;
	char stop = 0;
	removePiece(part);

	for(i = 0;i < 4;i++){
		if(part[i][0] + 1 > (WIDTH - 1) || data[part[i][0] + 1][part[i][1]] != 0){
			stop = 1;
			break;
		}
	}
	if(!stop){
		for(i = 0;i < 4;i++){
			part[i][0] = part[i][0] + 1;
			data[part[i][0]][part[i][1]] = piece + 1;  //update for 2 player
		}
	}
	else{
		addPiece(part,piece);
	
	}
	stop = 0;

}


/*
***********************************************************************                       
 RTI interrupt service routine: RTI_ISR
************************************************************************
*/

interrupt 7 void RTI_ISR(void)
{
  	// clear RTI interrupt flag 
  	CRGFLG = CRGFLG | 0x80;
    random = (random + 1) % 8;
    rticnt++;
    
    if(rticnt > 75){
      rticnt = 0;  
      ATDCTL5 = 0b00010000;
      while(ATDSTAT0_SCF == 0){ 
      }
      
      if(P1LR < 70){                 
        leftbutton = 1;
      }else if( P1LR > 180){
        rightbutton = 1;
      }
      if(P1UD > 200){
        jumpDownFlag = 1;
      }
    }
    
    if(!rotbutton && rotprev)  // check rotate button
      rotflag = 1;
    rotprev = rotbutton;
    
}

/*
***********************************************************************                       
  TIM interrupt service routine	  		
***********************************************************************
*/

interrupt 15 void TIM_ISR7(void)
{
  char i = 0;
  char j = 0;
  char stop = 0;
  char note = 0;
  

  	// clear TIM CH 7 interrupt flag
  	TFLG1 = TFLG1 | 0x80;
  	displayT();
  	if(pwmctr++ == 16) {
  	  pwmctr = 0;
  	  note = song[songctr++];
  	  PWMPER4 = note >= 0 ? notes[note][0] : PWMPER4;
  	  PWMDTY4 = note >= 0 ? notes[note][1] : 0;
  	  if(songctr == 128) songctr = 0;
  	}
  	if(count == 25){
  	  count = 0;
  	  // Remove current piece from data
      for(i = 0;i < 4;i++){
        data[curr_piece[i][0]][curr_piece[i][1]] = 0;
      }
      // Check if there is something below it, or its at the bottom
      for(i = 0;i < 4;i++) {
        if(curr_piece[i][1] + 1 > 31 || data[curr_piece[i][0]][curr_piece[i][1] + 1] != 0){
          stop = 1;
          break;
        }
      }
      // If there's not, move it down
      if(!stop){
         //move down;
         for(i = 0;i < 4;i++) {
          data[curr_piece[i][0]][++curr_piece[i][1]] = piece + 1;
         }
      } 
      // If there is, keep it
      else {
        for(i = 0;i < 4;i++) 
          data[curr_piece[i][0]][curr_piece[i][1]] = piece + 1;
          create_new_piece = 1;
      }
          
  	}
  	count++;
}

/*
interrupt 14 void TIM_ISR6(void){
  data[curr/32][curr%32] = 1;
  curr = (curr + 1) % 256;   
}
*/

void displayT(void){
 char i = 0;
 char j = 0;
 char color;
 char color2; 
 for(i = 0; i < 8; i++){
    for(j = 0; j < 32; j++){      
     	color = data[i][j];
     	color2 = data[i + 8][j];
     	OE = 0;
      R1 = (color & 0b00000001);
      B1 = (color & 0b00000010) >> 1;
      G1 = (color & 0b00000100) >> 2;
      
      R2 = (color2 & 0b00000001);
      B2 = (color2 & 0b00000010) >> 1;
      G2 = (color2 & 0b00000100) >> 2;
      LEDCLK = 1;
      LEDCLK = 0; 
      
    }
    
   	LAT = 1;
   	LAT = 0;
    A = (i & 0b00000001);
    B = (i & 0b00000010) >> 1;
    C = (i & 0b00000100) >> 2;   	
  }
  OE = 1;
}


/*
***********************************************************************                       
  SCI interrupt service routine		 		  		
***********************************************************************
*/

interrupt 20 void SCI_ISR(void)
{
 


}

/*
***********************************************************************
 Character I/O Library Routines for 9S12C32 
***********************************************************************
 Name:         inchar
 Description:  inputs ASCII character from SCI serial port and returns it
 Example:      char ch1 = inchar();
***********************************************************************
*/

char inchar(void) {
  /* receives character from the terminal channel */
        while (!(SCISR1 & 0x20)); /* wait for input */
    return SCIDRL;
}

/*
***********************************************************************
 Name:         outchar    (use only for DEBUGGING purposes)
 Description:  outputs ASCII character x to SCI serial port
 Example:      outchar('x');
***********************************************************************
*/

void outchar(char x) {
  /* sends a character to the terminal channel */
    while (!(SCISR1 & 0x80));  /* wait for output buffer empty */
    SCIDRL = x;
}
