#include "blue_os.h"
#include "constants.h"
#include "gui.h"

void drawFrame();

void gui(){
	blueOsInitShellVt100();
	drawFrame(30, 10, 100);
	
}

void drawFrame(speed, rps, sollwert)
{
	while(1){
		/*blueOsEnterGraphic();
		blueOsDrawMainFrame(3, 1, 80, 24);
		blueOsLeaveGraphic();
		blueOsSetPosition(4, 10);
		blueOsWriteString("Speed:");
		blueOsSetPosition(10, 10);
		blueOsWriteString("Welcome");
		blueOsSetRollingArea(10, 20);
		blueOsDelay(250);*/
		
		blueOsSetPosition(1,1);
		blueOsClearScreen();
		blueOsEnterGraphic();
		//blueOsDrawMainFrame(10,10,10,10); // set gui frame structure
		blueOsDrawHorizLine(1,1,60);
		blueOsDrawVertLine(1,2,20);
		blueOsDrawHorizLine(1,23,60);
		blueOsDrawVertLine(62,2,20);
		blueOsDrawHorizLine(2,3,59);
		blueOsLeaveGraphic();
		//params(0,0,0); // initialise the parameters
		blueOsSetPosition(2,23);
		blueOsWriteString("Gui for BlueRider\r\n");
		blueOsSetPosition(4, 5);
		blueOsWriteString("Parameters \r\n");
		
		blueOsSetPosition(6, 5);
		blueOsWriteString("Speed:  ");
		blueOsWriteInt(speed,0);
		blueOsWriteString("\r\n");
		
		blueOsSetPosition(8, 5);
		blueOsWriteString("Rps:     ");
		blueOsWriteInt(rps,0);
		blueOsWriteString("\r\n");
		
		blueOsSetPosition(10, 5);
		blueOsWriteString("Sollwert:     ");
		blueOsWriteInt(sollwert,0);
		blueOsWriteString("\r\n");
		
		blueOsSetPosition(14, 20);
		blueOsWriteString("Button u: Increase RPS by one\r\n");
		blueOsSetPosition(16, 20);
		blueOsWriteString("Button d: Decrease RPS by one\r\n");
		
		blueOsSetPosition(18, 20);
		blueOsWriteString("Input: \r\n");
		blueOsDelay(250);
	}
	
}