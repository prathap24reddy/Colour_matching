#define BLUE 0x187F
#define RED 0xb800
#define YELLOW 0xf600
#define GREEN 0x07F0
#define BLACK 0x0000
#define TEST1 0x07fa
#define WHITE 0xFFFF
#define cyan 0x07FF
#define magenta 0xF81F
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define KEY_BASE              0xFF200050
volatile int * KEYS_ptr = (int *) KEY_BASE;
int keys;
// block size 20 X 12
// limiting size of the game is 260
int arr[30][30];
int delay = 6000;
int ct=0;
int Score=0;

// writing and clearing functions

void do_nothing( int delay){
	while(delay--){

	}
}

void write_pixel(int x, int y, short colour)
{
    volatile short *vga_addr = (volatile short *)(0x08000000 + (y << 10) + (x << 1));
    *vga_addr = colour;
}

void write_char(int x, int y, char c)
{
    volatile char *character_buffer = (char *)(0x09000000 + (y << 7) + x);
    *character_buffer = c;
}

void clear_screen_notchar()
{
    int x, y;
    for (x = 0; x < 320; x++)
    {
        for (y = 0; y < 240; y++)
        {
            write_pixel(x, y, BLACK);
        }
    }
}

void clear_screen_char()
{
    int x, y;
    for (x = 0; x < 80; x++)
    {
        for (y = 0; y < 60; y++)
        {
            write_char(x, y, 0);
        }
    }
}

void drawbrick(int cx, int cy, short color) // draw a square of given size at given position whith color
{
    int i, j;
    cx-=8;
    cy-=5;
    for (i = cx; i < cx + 16; i++)
    {
        for (j = cy; j < cy + 10; j++)
        {
            write_pixel(i, j, color);
        }
    }
}

int dx[]={-1,1,0,0};
int dy[]={0,0,1,-1};

int tell(int x, int y,int z){
	if(x<0 || x>=20 || y<0 || y>=20)return 0;
	if(z!=arr[x][y])return 0;
	return 1;
}

void popup(int c, int k , int p){
int i;
for(i=0;i<4;i++){
	if(tell(c+dx[i],k+dy[i],p)){
		drawbrick((c+dx[i])*16+8,(k+dy[i])*10+5,BLACK);
		delay = 100000;
		do_nothing(delay);
		arr[c+dx[i]][k+dy[i]]=0;
		ct++;
		popup(c+dx[i],k+dy[i],p);
	}
}
}

int play_game(){
	clear_screen_char();
	ct=0;
	Score=0;
	int p,i,j,ok=1;
	for(i=8;i<320;i+=16){
	for(j=5;j<160;j+=10){
		p=rand()%5;
		if(p==0){
			drawbrick(i,j,0xb800);
		}else if(p==1){
			drawbrick(i,j,0x187F);
		}else if(p==2){
			drawbrick(i,j,0xf600);
		}else if(p==3){
			drawbrick(i,j,0x07F0);
		}else{
			drawbrick(i,j,0x07FF);
		}
		arr[(i-8)/16][(j-5)/10]=p+1;
	}
}
// for(i=0;i<320;i++){
// 	write_pixel(i,200,0xFFFF);
// }
// intial position of slider center is 152x235
short colour1;
p=rand()%5;
		if(p==0){
			colour1=0xb800;
		}else if(p==1){
			colour1=0x187F;
		}else if(p==2){
			colour1=0xf600;
		}else if(p==3){
			colour1=0x07F0;
		}else{
			colour1=0x07FF;
		}
int c=152;
drawbrick(c,235,colour1);
while(1){
	for(i=0;i<20;i++){
		if(arr[i][20]>0){
			ok=1;
			break;
		}
	}
	keys = *(KEYS_ptr+3);
	if(keys & 0x1){
		*(KEYS_ptr+3)=keys;
		drawbrick(c,235,BLACK);
		c-=16;
		(c+=320);
		c%=320;
		drawbrick(c,235,colour1);
		delay=1000000;
		do_nothing(delay);
	}
	 if(keys & 0x2){
	 	*(KEYS_ptr+3)=keys;
		drawbrick(c,235,BLACK);
		c=c+16;
		(c+=320);
		c%=320;
		drawbrick(c,235,colour1);
		delay=1000000;	
		do_nothing(delay);	
	}
	if(keys & 0x8){
		*(KEYS_ptr+3)=keys;
		ct=0;
		// center is at 152 x 235
		int l=(c-8)/16,k=235;
		while((k-15)>0 && arr[l][(k-15)/10] == 0 ){
			drawbrick(c,k,BLACK);
			k-=10;
			delay=1000000;
			do_nothing(delay);
			drawbrick(c,k,colour1);
		}
			popup((c-8)/16,(k-5)/10,p+1);
			if(ct){
				drawbrick(c,k,BLACK);
			}
			int y=0;
			for(i=0;i<20;i++){
				for(j=0;i<20;j++){
					if(arr[i][j] > 0)y=1;
				}
			}
			if(y==0){
				ok=0;
				break;
			}
			delay=1000000;	
			do_nothing(delay);
			Score+=ct;	
        p=rand()%5;
		if(p==0){
			colour1=0xb800;
		}else if(p==1){
			colour1=0x187F;
		}else if(p==2){
			colour1=0xf600;
		}else if(p==3){
			colour1=0x07F0;
		}else{
			colour1=0x07FF;
		}
		c=152;		
	}
}
return ok;
}

void trail_game(){

}

void main_page(){
int i,j;
// intialzing the blocks
for(i=0;i<30;i++){
	for(j=0;j<30;j++)arr[i][j]=0;
}
	while(1){
	clear_screen_notchar();
	char *hw1="Welcome to the Block Shooter";
	char *hw2="Press key 0 for trial game";
	char *hw3="Press key 1 for main game";
	int x=16;

 while(*hw1){
 	write_char(x,30,*hw1);
 	x++;
 	hw1++;
 }
 x=30;
  while(*hw2){
 	write_char(x,30,*hw2);
 	x++;
 	hw2++;
 }	
 x=32;
  while(*hw3){
 	write_char(x,30,*hw3);
 	x++;
 	hw3++;
 }
  keys=*(KEYS_ptr);
 if(keys & 0x1){
 	*KEYS_ptr=keys;
 	trail_game();
 	delay =10000000;
 	do_nothing(delay);
 	break;
 }
 if(keys & 0x02){
 	*KEYS_ptr=keys;
 	play_game();
 	delay =10000000;
 	do_nothing(delay);
 	break;
 }
}
}

void fin_page(int ok){
clear_screen_notchar();
if(ok){
 char *hw = "You lost the game";
 int x=30;
 while(*hw){
 	write_char(x,30,*hw);
 	x++;
 	hw++;
 }
}else{
 char *hw = "You won the game";
 int x=30;
 while(*hw){
 	write_char(x,30,*hw);
 	x++;
 	hw++;
 }
}
char *hw ="You will be take back to the main page";
 int x=27;
 while(*hw){
 	write_char(x,38,*hw);
 	x++;
 	hw++;
 }
 main_page();
}

// 320 X 160 is upto the current block
// 220 slider is present
//200 the game is done means the game is exit

int main(){
clear_screen_notchar();
main_page();
int ok=play_game();
/*
ok -> 1 ==> lost the game
ok -> 0 ==> won the game
*/

}