/*
************************************************************************
 ECE 362 - Mini-Project C Source File - Spring 2017
***********************************************************************
	 	   			 		  			 		  		
 Team ID: < ? >
 Project Name: < ? >
 Team Members:
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

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);
void displayT(void);

char rowCnt = 0;
char make = 0;
char color = 0;
char row = 0;
int prev = 0;
int curr = 0;
char leftbutton = 0;
char rightbutton = 0;
char rotbutton = 0;
char leftprev = 1;
char rightprev = 1;
char rotprev = 1;
char count;
char random = 0;
char currcolor = 1;
const char pieces[7][4][2] = {
  {{0,0},{0,1},{1,0},{1,1}}, // Square
  {{1,0},{0,0},{2,0},{3,0}}, // Line
  {{1,0},{0,0},{2,0},{1,1}}, // T
  {{1,0},{0,1},{0,0},{2,0}}, // J
  {{1,0},{0,0},{2,0},{2,1}}, // L
  {{1,1},{1,0},{0,1},{2,0}}, // Z
  {{1,1},{1,0},{0,0},{2,1}}  // S
};
/*
const char rotations[7][4][4][2] = {
  {
    {{0,0},{0,1},{1,0},{1,1}}, // Square
    {{
      
      
  {{1,0},{0,0},{2,0},{3,0}}, // Line
  {{1,0},{0,0},{2,0},{1,1}}, // T
  {{1,0},{0,1},{0,0},{2,0}}, // J
  {{1,0},{0,0},{2,0},{2,1}}, // L
  {{1,1},{1,0},{0,1},{2,0}}, // Z
  {{1,1},{1,0},{0,0},{2,1}}  // S
};*/
  
char curr_piece[4][2] = { 
    {0,0}, 
    {0,1},
    {1,0},
    {1,1}
};
char create_new_piece = 1;
      
      
      
  
  
char data[8][32];


/* Variable declarations */

   	   			 		  			 		       

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

#define R1 PTT_PTT7
#define B1 PTT_PTT6
#define G1 PTT_PTT5
#define A PTT_PTT4
#define B PTT_PTT3
#define C PTT_PTT2
#define LEDCLK PTT_PTT1
#define LAT PTT_PTT0
#define OE PTM_PTM0

	 	   		
/*	 	   		
***********************************************************************
 Initializations
***********************************************************************
*/

void  initializations(void) {
  char i;
  char j;

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
  
  for(i = 0; i < 8; i++){
    for(j = 0; j < 32; j++){
      data[i][j] = 0;
    }
    //make++;
    //make %= 8;
  }
/* Initialize peripherals */
  DDRT = 0xFF;
  DDRM = 0x01;
  DDRAD = 0;
  ATDDIEN = 0xC1;
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

   
}

	 		  			 		  		
/*	 		  			 		  		
***********************************************************************
Main
***********************************************************************
*/
void main(void) {
  char i;
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
      currcolor = random ? random : 1;
      for(i = 0;i < 4;i++){
        curr_piece[i][0] = pieces[random][i][0];
        curr_piece[i][1] = pieces[random][i][1];
      }
    }
    // Check Left Move
    if(leftbutton) {
      leftbutton = 0;
      // Remove current piece from data
      for(i = 0;i < 4;i++){
        data[curr_piece[i][0]][curr_piece[i][1]] = 0;
      }
      // Check if there is something to its left
      for(i = 0;i < 4;i++) {
        if(curr_piece[i][0] - 1 < 0 || data[curr_piece[i][0] - 1][curr_piece[i][1]] != 0){
          stop = 1;
          break;
        }
      }
      // If there's not, move it to the left
      if(!stop){
         //move over;
         for(i = 0;i < 4;i++) {
          data[--curr_piece[i][0]][curr_piece[i][1]] = currcolor;
         }
      }
      // If there is, keep it
    
      else {
        for(i = 0;i < 4;i++) 
          data[curr_piece[i][0]][curr_piece[i][1]] = currcolor;
      }
      stop = 0; 
   }
    if(rightbutton) {
      rightbutton = 0;
      // Remove current piece from data
      for(i = 0;i < 4;i++){
        data[curr_piece[i][0]][curr_piece[i][1]] = 0;
      }
      // Check if there is something to its right
      for(i = 0;i < 4;i++) {
        if(curr_piece[i][0] + 1 > 7 || data[curr_piece[i][0] + 1][curr_piece[i][1]] != 0){
          stop = 1;
          break;
        }
      }
      // If there's not, move it to the right
      if(!stop){
         //move over;
         for(i = 0;i < 4;i++) {
          data[++curr_piece[i][0]][curr_piece[i][1]] = currcolor;
         }
      }
      // If there is, keep it
    
      else {
        for(i = 0;i < 4;i++) 
          data[curr_piece[i][0]][curr_piece[i][1]] = currcolor;
      }
      stop = 0; 
   }
   /*if(rotbutton) {
      rotbutton = 0;
      // Remove current piece from data
      for(i = 0;i < 4;i++){
        data[curr_piece[i][0]][curr_piece[i][1]] = 0;
      }
      broken = 0;
      for(i = 0;i < 4;i++) {
          
        for(j = 0;j < 4;j++) {
          if(i == j) continue;
          // Calculate new values
          x = curr_piece[j][0] - curr_piece[i][0];
          y = curr_piece[j][1] - curr_piece[i][1];
          if((x >= 0) && (y > 0) || (x <= 0) && (y < 0)) {
            newx = y;
            newy = -1 * x;
          } else {
            newx = -1 * y;
            newy = x;
          }
          // Check if there is something there
          x = curr_piece[i][0] + newx;
          y = curr_piece[i][1] + newy;
          if((x < 0) || (x > 7) || (y > 31) || (y < 0) || data[x][y] != 0){
            broken = 1;
            break;
          } else {
            newdata[j][0] = x;
            newdata[j][1] = y;
          }
        }
        // If you can rotate, do so then break
        if(!broken) {
          for(j = 0;j < 4;j++) {
            if(i == j) continue;
            curr_piece[j][0] = newdata[j][0];
            curr_piece[j][1] = newdata[j][1];
          }
          break;
        }
      }
      // Put the piece back in
      for(i = 0;i < 4;i++) {
        data[curr_piece[i][0]][curr_piece[i][1]] = 1;
      }
   }*/

  
   } /* loop forever */
   
}   /* do not leave main */




/*
***********************************************************************                       
 RTI interrupt service routine: RTI_ISR
************************************************************************
*/

interrupt 7 void RTI_ISR(void)
{
  	// clear RTI interrupt flagt 
  	CRGFLG = CRGFLG | 0x80;
    random = (random + 1) % 8;
    
    if(PORTAD0_PTAD7 == 0) { // check left pushbutton
      if(leftprev == 1) {
        leftbutton = 1;
      }
    }
    leftprev = PORTAD0_PTAD7;
    if(PORTAD0_PTAD6 == 0) { // check right pushbutton
      if(rightprev == 1) {
        rightbutton = 1;
      }
    }
    rightprev = PORTAD0_PTAD6;
    if(!PTAD_PTAD0 && rotprev)  // check rotate button
      rotbutton = 1;
    rotprev = PORTAD0_PTAD0;
 

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

  	// clear TIM CH 7 interrupt flag
  	TFLG1 = TFLG1 | 0x80;
  	displayT();
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
          data[curr_piece[i][0]][++curr_piece[i][1]] = currcolor;
         }
      } 
      // If there is, keep it
      else {
        for(i = 0;i < 4;i++) 
          data[curr_piece[i][0]][curr_piece[i][1]] = currcolor;
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
 for(i = 0; i < 8; i++){
    for(j = 0; j < 32; j++){      
     	color = data[i][j];
     	OE = 0;
      R1 = (color & 0b00000001);
      B1 = (color & 0b00000010) >> 1;
      G1 = (color & 0b00000100) >> 2;
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