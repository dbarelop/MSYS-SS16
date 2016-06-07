#include "blue_os.h"
#include "constants.h"
#include "gui.h"

void gui()
{
	// TODO: finish GUI
	blueOsInitShellVt100();
	while (1)
	{
		blueOsEnterGraphic();
		blueOsDrawMainFrame(3, 1, 80, 24);
		blueOsLeaveGraphic();
		blueOsSetPosition(4, 10);
		blueOsWriteString("Speed:");
		blueOsSetPosition(10, 10);
		blueOsWriteString("Welcome");
		blueOsSetRollingArea(10, 20);
		blueOsDelay(250);
	}
}