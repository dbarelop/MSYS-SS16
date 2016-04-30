#include "blue_os.h"
#include "blue_os_internals.h"
#include "../../comm.h"
#include "blue_rt_debug.h"



typedef struct _rt_state
{
	uint8_t rt_id : 5;
	uint8_t run	  : 1;
	uint8_t state : 2; 
}RT_state;

BlueOsTCB *last_running = 0;

void send_debugEvent(volatile BlueOsTCB *tcb)
{
	RT_state state;
	
	if(tcb)
	{
		if(_currentTask == tcb && tcb == last_running)
			return;
		
		if(_currentTask == tcb && _currentTask != last_running)
		{
			send_debugEvent(last_running);
			last_running = tcb;
		}
	
		state.rt_id = tcb->_taskId;

		state.run = (tcb == _currentTask)?1:0;
		state.state = tcb->_state;
	}
	else //time synchronization event
	{
		state.rt_id = 0x1F;
	}	
	
	//send via debug channel
	commSendRealtime(&state);
}