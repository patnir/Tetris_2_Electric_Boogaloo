/*
************************************************************************
 ECE 362 - Mini-Project C Source File - Spring 2017
***********************************************************************
      
 Team ID:  13
 Project Name:  Tetris 2: Electric Boogaloo
 Team Members: Michael Toner, Rahul Patni, Yash Bharatula, Alex Medoff
   - Team/Doc Leader:  Michael Toner       Signature: ______________________
   
   - Software Leader: Yash Bharatula       Signature: ______________________
   - Interface Leader:   Alex Medoff       Signature: ______________________
   - Peripheral Leader:  Rahul Patni       Signature: ______________________
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
#define CR 0x0D // ASCII return 
#define LF 0x0A // ASCII new line 

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F // LCD initialization command
#define LCDCLR 0x01 // LCD clear display command
#define TWOLINE 0x38 // LCD 2-line enable command
#define CURMOV 0xFE // LCD cursor move instruction
#define LINE1  0x80 // LCD line 1 cursor position
#define LINE2  0xC0 // LCD line 2 cursor position

#define RED 1
#define BLUE 2
#define PURPLE 3
#define GREEN 4
#define YELLOW 5
#define CYAN 6
#define WHITE 7

#define LEDCLK PTAD_PTAD7
#define LAT PTAD_PTAD6
#define WIDTH 16
#define HEIGHT 32
#define P1UD ATDDR1H
#define P1LR ATDDR0H
#define P2UD ATDDR3H
#define P2LR ATDDR2H
#define P1ROT PTT_PTT7 
#define P2ROT PTT_PTT6
#define musicbutton PTT_PTT5
#define brushMode PTT_PTT4

//#define rotbutton PORTAD0_PTAD6

#define SSLED PTM_PTM0
#define changeColor PTT_PTT6
#define selectbutton PTT_PTT7

#define down1 0b00000001 & jumpDownFlag
#define down2 0b00000010 & jumpDownFlag
#define left1 0b00000001 & leftbutton
#define left2 0b00000010 & leftbutton
#define right1 0b00000001 & rightbutton
#define right2 0b00000010 & rightbutton
#define rot1 0b00000001 & rotflag
#define rot2 0b00000010 & rotflag
#define up1flag 0b00000001 & upflag
#define up2flag 0b00000010 & upflag
#define makepiece1 0b00000001 & create_new_piece
#define makepiece2 0b00000010 & create_new_piece

#define games 4

/* LCD COMMUNICATION BIT MASKS */
#define RS PTT_PTT2		// RS pin mask (PTT[2])
#define RW PTT_PTT1		// R/W pin mask (PTT[3])
#define LCDCLK PTT_PTT0	// LCD EN/CLK pin mask (PTT[4])

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);
void displayT(void);
void clearRow(int row,char player);
void rotate(char curr_piece[][2],char color, char *rotAdd);
void moveLeft(char part[][2], char color);
void moveRight(char part[][2],char color);
void checkCompleteRow(char player);
void initBoard(void);
void createPiece(char curr_piece[][2],char players);
void addPiece(char part[][2],char color);
void removePiece(char part[][2]);
void gameoverBoard(char color);
void jumpDown(char part[][2],char player);
void tetrisGame(int players);
void moveDown(char part[][2],char player);
void chgMode(char mode);
void shiftout(char x);
void lcdwait(void);
void send_byte(char x);
void send_i(char x);
void chgline(char x);
void print_c(char x);
void pmsglcd(char[]); 
void tron(void);
void advancePlayer(char player[2],char vel,char color);
void paint(void);



/* Flag Declarations */
char rowCnt = 0;
char make = 0;
char row = 0;
char leftbutton = 0;
char rightbutton = 0;
char rotflag = 0;
char upflag = 0;
char leftprev = 1;
char rightprev = 1;
char rotprev = 1;
char count;
char random = 0;
char piece = 5;
char tetris = 0;
char r1 = 0;
char r2 = 0;
char gameover = 0;
char rticnt = 0;
char jumpDownFlag = 0;
char rowsel = 0;
char create_new_piece = 0;
char numPlayers = 2;
char p1color;
char p2color;
char linecurr = 1;
char resetflag = 1;
char gameMode = 0;
char prevColor = 0;
char prevSelect = 0;
char colorflag = 0;
char selectflag = 0;
char prevrot1 = 0;
char prevrot2 = 0;
char prevmusic = 0;
char musicflag = 0;
char stopmusic = 0;
char brushflag = 0;
char prevbrush = 0;
char brushchange = 0;
char rowClear = 0;
int score = 0;

//FOR MUSIC
unsigned char pwmctr = 0;
unsigned char songctr = 0;
char musicToggle = 0;
//

//North 0
//East 1
//South 2
//West 3
char p1[] = {WIDTH/2,5};       
char p2[] = {WIDTH/2,HEIGHT-6};
char p1Vel = 2;
char p2Vel = 0;
char brush[] = {WIDTH/2,HEIGHT/2};
char direction = 4;
//DIRECTION
//0 --> Up
//1 --> Right
//2 --> Down
//3 --> Left
//4 --> Not Moving
  

const char pieces[7][4][4][2] = { //contains all 4 rotations of the 7 tetris pieces
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
  
char p1_piece[4][2];
char p2_piece[4][2];

//                        F#  0      G         G#        A         A#        B   5    C        C#       D        D#       E  10    F        F#       G        G#       A        A#      B        C       C#      D       D#     E        F
const char notes[][2] = {{255,128},{239,120},{227,113},{215,107},{203,101},{191,95},{181,90},{171,85},{161,80},{152,76},{144,72},{135,68},{128,64},{121,60},{114,57},{108,54},{102,51},{96,48},{91,45},{86,43},{81,40},{77,38},{71,36},{69,34}};
 // Tetris Theme
 const char song[128] = {
  10,10,5,6,8,8,6,5,
  3,3,3,6,10,10,8,6,
  5,5,5,6,8,8,10,10,
  6,6,3,3,3,3,5,6,
  8,8,8,11,15,15,13,11,
  10,10,10,6,10,10,8,6,
  5,5,5,6,8,8,10,10,
  6,6,3,3,3,3,-1,-1,
  10,10,10,10,6,6,6,6,
  8,8,8,8,5,5,5,5,
  6,6,6,6,3,3,3,3,
  2,2,2,2,5,5,-1,-1,
  10,10,10,10,6,6,6,6,
  8,8,8,8,5,5,5,5,
  6,6,10,10,15,15,15,15,
  14,14,14,14,-1,-1,-1,-1  
 };
 // Dance of the Sugar Plum Fairy
  const char song2[] = {
  -1,-1,23,20,23,23,22,22,
  19,19,20,20,18,18,18,18,
  17,17,17,17,16,16,16,16,
  15,20,16,20,15,15,8,5,
  2,2,23,20,23,23,22,22,
  1,1,22,19,22,22,21,21,
  0,0,21,18,21,21,20,20,
  19,19,20,20,18,18,18,18,
  17,17,17,17,16,16,16,16,
  15,20,16,20,15,15,15,15
 };
 // Titanic Theme
 const char song3[] = {
  6,8,
  10,10,10,10,10,10,8,6,
  8,13,13,13,13,13,10,13,
  15,15,15,15,15,15,13,8,
  8,8,8,8,8,8,-1,1,
  6,6,6,6,6,6,6,8,
  8,8,8,8,8,8,8,8,
  13,13,13,13,11,11,11,10,
  8,8,8,8,10,10,11,11,
  10,10,10,10,8,8,6,6,
  5,5,5,5,6,6,-1,6,
  5,5,5,5,6,6,8,8,
  10,10,10,10,-1,10,8,8,
  6,6,6,6,-1,-1,-1,-1
 };
//The board that gets updated to the LED matrix
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

//Clear the board
  initBoard();
  
/* Initialize peripherals */
  DDRT = 0b00001111;
  DDRAD = 0b11000000;
  ATDDIEN = 0b00110000;
    
//Initialize Timer for roughly 75Hz
  TSCR2 = 0x0C;
  TIOS = 0xC0;
  TIE = 0x00;
  TSCR1 = 0x80;
  TCTL1 = 0x00;
  TCTL2 = 0x00;
  TC7 = 20000;

/* Initialize RTI for 2.048 ms interrupt rate */  
  RTICTL = 0x1F;
  CRGINT = 0x80;
    
//Initialize ATD
  ATDCTL2 = 0b10000000;
  ATDCTL3 = 0b01000000;
  ATDCTL4 = 0b10000101;
  
//Initialize SPI
  SPIBR = 0x00;
  SPICR1 = 0b01010000;
  SPICR2 = 0b00000000;

//Initialize SSLED pin
  DDRM = 0b00000001;
  
  
  /* Initialize the LCD
     - pull LCDCLK high (idle)
     - pull R/W' low (write state)
     - turn on LCD (LCDON instruction)
     - enable two-line mode (TWOLINE instruction)
     - clear LCD (LCDCLR instruction)
     - wait for 2ms so that the LCD can wake up     
  */

  LCDCLK = 1;
  RW = 0;;
  send_i(LCDON);
  send_i(TWOLINE);
  send_i(LCDCLR);
  lcdwait();
  
  // Initialize PWM
  MODRR |= 0b00001000;
  PWME_PWME3 = 1;
  PWMPOL |= 0b00100000;
  PWMCTL = 0;
  PWMCAE = 0;
  PWMCLK_PCLK3 = 1;
  PWMPRCLK = 0b00000000;
  PWMSCLB = 0x7F;
  PWMPER3 = 215;
  PWMDTY3 = 215 / 2; // 
  
  //displayT(); 
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
  PWMPER3 = 0;
  PWMDTY3 = 0;
  for(;;) {
    
    if (left1 == 1) {
        leftbutton &= 2;
        gameMode = (gameMode + 1) % games;
        chgMode(gameMode); 
    }
    if (right1 == 1) {
        rightbutton &= 2;
        gameMode = (gameMode + games-1) % games;
        chgMode(gameMode); 
    }
    if (resetflag == 1) {
      resetflag = 0;
      chgline(LINE1);
      pmsglcd("Game mode ");
      chgline(LINE2);
      pmsglcd("1 Player");
    }
    if(selectflag == 1){
      selectflag = 0;
      if(gameMode <2){
        tetrisGame(gameMode + 1);
      }
      if(gameMode == 2){
        tron(); 
      }
      if(gameMode == 3){
        paint();
      }
    }
  }
    /* loop forever */
}   /* do not leave main */

void tetrisGame(int players){
    leftbutton = 0;
    rightbutton = 0;
    rotflag = 0;
    jumpDownFlag = 0;
    gameover = 0;
    if(players == 1){
       create_new_piece = 1;
    }else{
       create_new_piece = 3;
    }
    
    chgline(LINE1);
    pmsglcd("         ");
    pmsglcd("Score:");
    
    TIE = 0x80;
    initBoard();
    numPlayers = players;
    for(;;) {
    
       if(musicflag == 1){
          musicflag = 0;
          stopmusic = (stopmusic + 1) % 4;  
       }
           
       if (resetflag == 1) {
         return; 
       }
       if(makepiece1 && !gameover) {
         create_new_piece &= 2;
         createPiece(p1_piece,1);
       }
       
  // Check Left Move
        if(left1 && !gameover) {
          leftbutton &= 2;
          moveLeft(p1_piece,p1color); 
        }
        if(right1 && !gameover) {
          rightbutton &= 2;
          moveRight(p1_piece,p1color);
        }
        if(rot1){
          rotflag &= 2;
          rotate(p1_piece,p1color,&r1);  
        }
        if(down1){
          jumpDownFlag &= 2;
          moveDown(p1_piece,1);
    
        }
        if(brushflag){
          
          brushflag = 0;
          leftbutton = 0;
          rightbutton = 0;
          rotflag = 0;
          jumpDownFlag = 0;
          gameover = 0;
          if(players == 1){
             create_new_piece = 1;
          }else{
             create_new_piece = 3;
          }
          initBoard();
        }
        if(players == 2){
           if(makepiece2 && !gameover){
              create_new_piece &= 1;
              createPiece(p2_piece,2); 
           }
           if(left2 && !gameover){
              leftbutton &= 1;
              moveLeft(p2_piece,p2color);
           }
           if(right2 && !gameover){
              rightbutton &= 1;
              moveRight(p2_piece,p2color);
           }
           if(rot2){
              rotflag &= 1;
              rotate(p2_piece,p2color,&r2);
           }
           if(down2){
              jumpDownFlag &= 1;
              moveDown(p2_piece,2);
           }
        }
   
     } /* loop forever */
}

void tron(void){
    leftbutton = 0;
    rightbutton = 0;
    rotflag = 0;
    jumpDownFlag = 0;
    gameover = 0;
    upflag = 0;
    TIE = 0x80;
    initBoard();
    
    p1color = PURPLE;
    p2color = GREEN;
    
    for(;;){
      if(musicflag == 1){
           musicflag = 0;
           stopmusic = (stopmusic + 1) % 4;  
      }
      if(left1 && !gameover) {
        leftbutton &= 2;
        p1Vel = 1; 
      }
      if(right1 && !gameover) {
        rightbutton &= 2;
        p1Vel = 3;
      }
      if(up1flag){
        upflag &= 2;
        p1Vel = 0;  
      }
      if (brushflag) {
        brushflag = 0;
        leftbutton = 0;
        rightbutton = 0;
        rotflag = 0;
        jumpDownFlag = 0;
        gameover = 0;
        upflag = 0;
        initBoard();
        p1Vel = 2;
        p2Vel = 0;
        p1[0] = WIDTH/2;
        p1[1] = 5;       
        p2[0] = WIDTH/2;
        p2[1] = HEIGHT-6;  
      }
      if(down1){
        jumpDownFlag &= 2;
        p1Vel = 2;  
      }
      if(left2 && !gameover) {
        leftbutton &= 1;
        p2Vel = 1; 
      }
      if(right2 && !gameover) {
        rightbutton &= 1;
        p2Vel = 3;
      }
      if(up2flag){
        upflag &= 1;
        p2Vel = 0;  
      }
      if(down2){
        jumpDownFlag &= 1;
        p2Vel = 2;  
      } 
    }
    
}

void paint(void){
  char tempColor = 0;
  char toggle = 1;
  TIE = 0x80;
  initBoard();
  p1color = RED;
  data[brush[0]][brush[1]] = p1color;
  for(;;){
      
      if(musicflag == 1){
           musicflag = 0;
           stopmusic = (stopmusic + 1) % 4;  
      }
      if(direction == 0){
        direction = 4;
        if(brush[1] > 0){
          brush[1] -= 1;
          data[brush[0]][brush[1] + 1] = tempColor;
          tempColor = data[brush[0]][brush[1]];
          data[brush[0]][brush[1]] = p1color; 
        }
      }
      if(direction == 1){
        direction = 4;
        if(brush[0] < (WIDTH - 1)){
          brush[0] += 1;
          data[brush[0] - 1][brush[1]] = tempColor;
          tempColor = data[brush[0]][brush[1]];
          data[brush[0]][brush[1]] = p1color; 
        }
      }
      if(direction == 2){
        direction = 4;
        if(brush[1] < (HEIGHT - 1)){
          brush[1] += 1;
          data[brush[0]][brush[1] - 1] = tempColor;
          tempColor = data[brush[0]][brush[1]];
          data[brush[0]][brush[1]] = p1color;
        }
      }
      if(direction == 3){
        direction = 4;
        if(brush[0] > 0){
          brush[0] -= 1;
          data[brush[0] + 1][brush[1]] = tempColor;
          tempColor = data[brush[0]][brush[1]];
          data[brush[0]][brush[1]] = p1color;
        }
      }
      if(brushflag){
          brushchange = !brushchange;
          brushflag = 0;
          selectflag = 0; 
      }
      if(brushchange){
         if(toggle){
           data[brush[0]][brush[1]] = p1color;
           tempColor = p1color;
         }
      }
      if(selectflag){
          data[brush[0]][brush[1]] = p1color;
          tempColor = p1color;
          selectflag = 0;
          if(brushchange){
            toggle = !toggle; 
          }

      }
      if(colorflag){
          colorflag = 0;
          p1color = (p1color + 1) % 8;
          data[brush[0]][brush[1]] = p1color;
      }
  }
  return;
}
  
void chgMode(char mode){
  chgline(LINE2);
  pmsglcd("              ");
  if(mode == 0){
     pmsglcd("1 Player");
  }
  if(mode == 1){
     pmsglcd("2 Players");
  }
  if(mode == 2){
     pmsglcd("TRON"); 
  }
  if(mode == 3){
	   pmsglcd("Paint");
  }
  
}


void createPiece(char curr_piece[][2],char player){
	char i; 
	piece = random + player;
	checkCompleteRow(player);
	if(numPlayers == 1){
		for (i = 0; i < 4; i++){
			curr_piece[i][0] = pieces[piece][0][i][0] + WIDTH/2;
			curr_piece[i][1] = pieces[piece][0][i][1];
		}
	}
	if(numPlayers == 2){
		if(player == 1){
			for (i = 0; i < 4; i++){
				curr_piece[i][0] = pieces[piece][0][i][0] + 5;
				curr_piece[i][1] = pieces[piece][0][i][1];
			}
		}	
		if(player == 2){
			for (i = 0; i < 4; i++){
				curr_piece[i][0] = pieces[piece][0][i][0] + 10 ;
				curr_piece[i][1] = pieces[piece][0][i][1];
			}
		}
	}
	for(i = 0; i < 4; i++){
		if(data[curr_piece[i][0]][curr_piece[i][1]] != 0){
			gameover = 1;
		}
	}
	if(gameover){
		gameoverBoard(RED);
	}else{
		addPiece(curr_piece,piece);
	}
	if(player == 1){
		p1color = piece;
		r1 = 0; 
	}else{
		p2color = piece; 
		r2 = 0;
	}
}

void checkCompleteRow(char player){
  char i;
  char j;
  char rowsCleared = 0;
  for(i = 0; i < HEIGHT; i++){
    tetris = 1;
    for(j = 0; j < WIDTH; j++){
      if(data[j][i] == 0){
        tetris = 0;
        break;
      }

    }
    if(tetris != 0){
      clearRow(i,player);
      i--;
      rowsCleared++;
    }
  }
  if(rowsCleared > 0){
      score += rowsCleared * rowsCleared * 10;  
      rowClear++;
  }
  
}

void initBoard(void){
  char i;
  char j;
  for (i = 0; i < 32; i++){
    for(j = 0; j < 16; j++){
      data[j][i] = 0; 
    }
  }
}

void gameoverBoard(char color){
  char i;
  char j;
  char points[] = "0000000000";
      SSLED = 0;
      chgline(LINE2);
      pmsglcd("        ");
      points[8] = 48 + score%10;
      points[7] = 48 + (score/10) % 10;
      points[6] = 48 + (score/100) % 10;
      points[5] = 48 + (score/1000) % 10;
      points[4] = 48 + (score/10000) % 10;
      pmsglcd(points);  
  initBoard();
  
	for(i = 0; i < HEIGHT;i+=2){
		for(j = 0; j < WIDTH;j+=2){
			data[j][i] = 0;
			data[j + 1][i] = color;
		}
	}
	for(i = 1; i < HEIGHT;i+=2){
		for(j = 0; j < WIDTH;j+=2){
			data[j][i] = color;
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

void jumpDown(char part[][2], char player){
  char i;
  char stop = 0;
  char color;
  if(player == 1){
    color = p1color; 
  }else{
    color = p2color; 
  }
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
          data[part[i][0]][++part[i][1]] = color + 1;
         }
      } 
  }
  for(i = 0;i < 4;i++)
    data[part[i][0]][part[i][1]] = color + 1;
  create_new_piece = player;
}

void rotate(char curr_piece[][2],char color, char *rotAdd){
  char i;
  char newPos[4][2];
  char valid = 0;
  char rot = *rotAdd;
    
  for(i = 0; i < 4; i++){
      newPos[i][0] = pieces[color][(rot + 1)%4][i][0] - pieces[color][rot][i][0] + curr_piece[i][0];
      newPos[i][1] = pieces[color][(rot + 1)%4][i][1] - pieces[color][rot][i][1] + curr_piece[i][1];
  }
  removePiece(curr_piece);
  for(i = 0; i < 4;i++){
    if(newPos[i][0] > (WIDTH - 1) || newPos[i][0] < 0 || data[newPos[i][0]][newPos[i][1]] != 0){
      valid = 0;
      break;
    }
    if(newPos[i][1] > (HEIGHT - 1) || newPos[i][1] < 0 || data[newPos[i][0]][newPos[i][1]] != 0){
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
      *rotAdd = (rot + 1) % 4;
  }
  addPiece(curr_piece,color);
  return;
}

void clearRow(int row,char player){
  char j;
  char k;
  if(player == 1 && numPlayers == 2){
     removePiece(p2_piece);  
  }else if(player == 2 && numPlayers == 2){
     removePiece(p1_piece); 
  }
  for(j = row;j >0; j--){
	  for(k = 0; k < WIDTH; k++){
		  data[k][j] = data[k][j-1];
    }
  }
  if(player == 1 && numPlayers == 2){
    addPiece(p2_piece,p2color);
  }else if(player == 2 && numPlayers == 2){
    addPiece(p1_piece,p1color); 
  }
 }

void moveLeft(char part[][2],char color){
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
			data[part[i][0]][part[i][1]] = color + 1;
		}
	}
	else{
		addPiece(part,color);
	}
}

void moveRight(char part[][2],char color){
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
			data[part[i][0]][part[i][1]] = color + 1;  //update for 2 player
		}
	}
	else{
		addPiece(part,color);
	}
}

void moveDown(char part[][2],char player){
      char color;
      char i;
      char stop = 0;
      if(player == 1){
        color = p1color; 
      }else{
        color = p2color; 
      }
      
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
          data[part[i][0]][++part[i][1]] = color + 1;
         }
      } 
      // If there is, keep it
      else {
        for(i = 0;i < 4;i++) 
          data[part[i][0]][part[i][1]] = color + 1;
          create_new_piece = player;
      }
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
    random = (random + 1) % 7;
    rticnt++;
    
    if(rticnt > 75){ //only check the Joysticks if enough time has passed to avoid wasting time
      rticnt = 0;  
      ATDCTL5 = 0b00010000;
      while(ATDSTAT0_SCF == 0){ 
      }
      
      if(P1LR < 70){                 
        leftbutton |= 1;
        direction = 3;
      }else if( P1LR > 180){
        rightbutton |= 1;
        direction = 1;
      }
      if(P1UD > 200){
        jumpDownFlag |= 1;
        direction = 2;
      }
      if(P1UD < 70){
        upflag |= 1; 
        direction = 0;
      }
      
      
      if(P2LR < 70){                 
        leftbutton |= 2;
      }else if( P2LR > 180){
        rightbutton |= 2;
      }
      if(P2UD > 200){
        jumpDownFlag |= 2;
      }
      if(P2UD < 70){
        upflag |= 2; 
      }
      
    }
     
    if(!selectbutton && prevSelect)  // check select button in paint
      selectflag = 1;
    prevSelect = selectbutton;

    if(!changeColor && prevColor){ //check color button in paint
      colorflag = 1;
    } 
    prevColor = changeColor;
    
    if(!P1ROT && prevrot1)
      rotflag |= 1;
    prevrot1 = P1ROT;
    
    if(!P2ROT && prevrot2)
      rotflag |= 2;
    prevrot2 = P2ROT;
    
    if(!musicbutton && prevmusic){
      songctr = 0;  
      musicflag = 1;
    }
    prevmusic = musicbutton;
    
    if(!brushMode && prevbrush)
      brushflag = 1;
    prevbrush = brushMode;

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
  
  if(stopmusic == 1){
    if(pwmctr++ == 16) {
  	  pwmctr = 0;
  	  note = song[songctr++];
  	  PWMPER3 = note >= 0 ? notes[note][0] : PWMPER3;
  	  PWMDTY3 = note >= 0 ? notes[note][1] : 0;
  	  if(songctr == 128) songctr = 0;
    }
    
  }else if(stopmusic == 2){
     if(pwmctr++ == 14) {
  	  pwmctr = 0;
  	  note = song2[songctr++];
  	  PWMPER3 = note >= 0 ? notes[note][0] : PWMPER3;
  	  PWMDTY3 = note >= 0 ? notes[note][1] : 0;
  	  if(songctr == 64) songctr = 0;
    }
  }
  else if(stopmusic == 3){
   if(pwmctr++ == 14) {
	  pwmctr = 0;
	  note = song3[songctr++];
	  PWMPER3 = note >= 0 ? notes[note][0] : PWMPER3;
	  PWMDTY3 = note >= 0 ? notes[note][1] : 0;
	  if(songctr == 106) songctr = 0;
   }
  }
  else{
    PWMPER3 = 0;
    PWMDTY3 = 0; 
  }
  
  count++;
  if(gameMode == 0 || gameMode == 1){
    if(count == 40 - 4*rowClear){
     count = 0;
     moveDown(p1_piece,1);
    
     if(numPlayers == 2){
       moveDown(p2_piece,2);
     }           
    }
  }else if(gameMode == 2 && !gameover){
    if(count == 25 && !gameover){
      count = 0;
      advancePlayer(p1,p1Vel,p1color);
      if(!gameover){
        advancePlayer(p2,p2Vel,p2color);
      }
    }
  }

  
}

void advancePlayer(char player[2],char vel,char color){
  if(vel == 0){
    if(data[player[0]][player[1]-1] != 0 || player[1] - 1 < 0){
      if(color == PURPLE){
        gameoverBoard(GREEN);
        gameover = 1;
      }else{
        gameoverBoard(PURPLE);
        gameover = 1;
      }
    }else{
      data[player[0]][player[1]-1] = color;
      if(color == p1color){
        p1[1] = p1[1] - 1;
      }else{
        p2[1] = p2[1] - 1;
      }
    }
  }if(vel == 1){
    if(data[player[0] - 1][player[1]] != 0 || player[0] - 1 < 0){
      if(color == PURPLE){
        gameoverBoard(GREEN);
        gameover = 1;
      }else{
        gameoverBoard(PURPLE);
        gameover = 1;
      }
    }else{
      data[player[0] - 1][player[1]] = color;
      if(color == p1color){
        p1[0] = p1[0] - 1;
      }else{
        p2[0] = p2[0] - 1;
      }
    }
  }if(vel == 2){
    if(data[player[0]][player[1] + 1] != 0 || player[1] + 1 > HEIGHT - 1){
      if(color == PURPLE){
        gameoverBoard(GREEN);
        gameover = 1;
      }else{
        gameoverBoard(PURPLE);
        gameover = 1;
      }
    }else{
      data[player[0]][player[1]+1] = color;
      if(color == p1color){
        p1[1] = p1[1] + 1;
      }else{
        p2[1] = p2[1] + 1;
      }
    }
  }if(vel == 3){
    if(data[player[0] + 1][player[1]] != 0 || player[0] + 1 > WIDTH - 1){
      if(color == PURPLE){
        gameoverBoard(GREEN);
        gameover = 1;
      }else{
        gameoverBoard(PURPLE);
        gameover = 1;
      }
    }else{
      data[player[0] + 1][player[1]] = color;
      if(color == p1color){
        p1[0] = p1[0] + 1;
      }else{
        p2[0] = p2[0] + 1;
      }
    }
  }
}


void displayT(void){
  char push = 0;
  char i;
  char j;
  char k;
  char color;
  char color2;
  char wait = 1;
  char points[] = "000000000";
  SSLED = 0;
  for(i = 0; i < WIDTH/2; i++){
    for(j = 0; j < HEIGHT; j++){
      push = 0;
      push = push | ((i & 0b00000100) >> 2);


      SPIDR = push;
      while(SPISR_SPTEF == 0){}
      for(k = 0; k < wait; k++){} // wait a few execute cycles to allow some timing issue to be resolved
 //Wait for SPI module
      push = 0; 
      color = data[i][j];
      color2 = data[i + WIDTH/2][j];
      
      push = push | ((i & 0b00000001) << 6);
      push = push | ((i & 0b00000010) << 6);
      
      push =  push | (color & 0b00000001);
      push =  push | (color & 0b00000010);
      push =  push | (color & 0b00000100);
      push =  push | ((color2 & 0b00000001) << 3);
      push =  push | ((color2 & 0b00000010) << 3);
      push =  push | ((color2 & 0b00000100) << 3);

      SPIDR = push;
      while(SPISR_SPTEF == 0){}
      for(k = 0; k < wait; k++){}



      LEDCLK = 1;
      LEDCLK = 0;
    }
    SSLED = 1;
    LAT = 1;
    LAT = 0;
    rowsel = i;
    for(k = 0; k < wait*15; k++){}
    SSLED = 0;
    for(k = 0; k < wait*3; k++){
    }
       
  }
  
}


/*
***********************************************************************                       
  SCI interrupt service routine   
***********************************************************************
*/


/*
***********************************************************************
  shiftout: Transmits the character x to external shift 
            register using the SPI.  It should shift MSB first.  
             
            MISO = PM[4]
            SCK  = PM[5]
***********************************************************************
*/
 
void shiftout(char x)

{
  int i;
  // read the SPTEF bit, continue if bit is 1
  // write data to SPI data register
  // wait for 30 cycles for SPI data to shift out
  while(SPISR_SPTEF == 0){  }
  SPIDR = x;
  for(i = 0; i < 10; i++){} //at least 30 execute cycles
}

/*
***********************************************************************
  lcdwait: Delay for approx 2 ms
***********************************************************************
*/

void lcdwait()
{
  int i;
  for(i = 0; i < 4800; i++){}    //wait 48,000 exectue cycles
}

/*
*********************************************************************** 
  send_byte: writes character x to the LCD
***********************************************************************
*/

void send_byte(char x)
{
     // shift out character
     // pulse LCD clock line low->high->low
     // wait 2 ms for LCD to process data
    shiftout(x);
    
    LCDCLK = 0;
    LCDCLK = 1;
    LCDCLK = 0;
    lcdwait();
}

/*
***********************************************************************
  send_i: Sends instruction byte x to LCD  
***********************************************************************
*/

void send_i(char x)
{
        // set the register select line low (instruction data)
        // send byte
        RS = 0;
        send_byte(x);
}

/*
***********************************************************************
  chgline: Move LCD cursor to position x
  NOTE: Cursor positions are encoded in the LINE1/LINE2 variables
***********************************************************************
*/

void chgline(char x)
{
  send_i(CURMOV);
  send_i(x);
  linecurr = x;
}

/*
***********************************************************************
  print_c: Print (single) character x on LCD            
***********************************************************************
*/
 
void print_c(char x)
{
  RS = 1;
  send_byte(x);
}

/*
***********************************************************************
  pmsglcd: print character string str[] on LCD
***********************************************************************
*/

void pmsglcd(char str[])
{
  int i = 0;
  while(str[i] != '\0'){
    send_i(CURMOV);
    send_i(linecurr+i);
    print_c(str[i]);
    i++;
  }
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
