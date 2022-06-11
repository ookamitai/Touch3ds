#include <3ds.h>
#include <stdio.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	gfxInitDefault();
	
	PrintConsole topScreen, bottomScreen;
	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);//init both consoles
	consoleSelect(&bottomScreen);//pretty self-explanatory, i think
	
	printf("\x1b[14;11HPress Start to exit.");
	printf("\x1b[15;13HPress A to clear.");
	printf("\x1b[16;5HPress B to toggle display mode.");
	char visual[12][16] = {0};//be careful beacause it formats like name[y][x]
	int toggle = 0;
	
	// Main loop
	while (aptMainLoop())
	{
		
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu
		if (kDown & KEY_A){
			for(int i = 0; i < 12; i++) {
      			for(int j = 0; j < 16;j++) {
      				visual[i][j] = 0;
      			}
      		}
		}
		if (kDown & KEY_B) {toggle = 1 - toggle;}
		
		touchPosition touch;
		//Read the touch screen coordinates
		
		hidTouchRead(&touch);
		consoleSelect(&topScreen);
		//Print the touch screen coordinates
		printf("Touch Screen Test by ookamitai\nPos: ");
		visual[(touch.py - touch.py % 10) / 20][(touch.px - touch.px % 10) / 20] += 1;
		printf("%03d, %03d (%02X, %02X)\n\n", touch.px, touch.py, (touch.px - touch.px % 10) / 20, (touch.py - touch.py % 10) / 20);
		
		printf("  ");
		for(int w = 0; w < 16; w++){
			printf("\x1b[41;37m%02X \x1b[0m", w);
		}
		
		for(int i = 0; i < 12; i++) {
      		for(int j = 0; j < 16;j++) {
      			if((j) % 16 == 0){
            		printf("\x1b[41;37m  \n%02X\x1b[0m", i);
         		}
         		if(i == (touch.py - touch.py % 10) / 20 && j == (touch.px - touch.px % 10) / 20){
         			printf("\x1b[44;37m%02X \x1b[0m", visual[i][j]);
				} else if(visual[i][j] != 0){
					if (toggle){
						printf("\x1b[44;37m   \x1b[0m");
					} else {
						printf("%02X ", visual[i][j]);	
					}
				
				} else {
					printf("   ");
				}
      		}
   		}		
		
		printf("\nMode: %s", (toggle) ? "Color" : "Hex  ");
		printf("\x1b[0;0H");
		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
		//Wait for VBlank
		gspWaitForVBlank();
	}

	// Exit services
	gfxExit();
	return 0;
}
