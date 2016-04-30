#ifndef BLUE_OS_CALLBACK_H
#define BLUE_OS_CALLBACK_H



typedef void (*CallBackFunc)(uint8_t* source, uint8_t value);

typedef volatile struct BlueOsCallBack
{
	volatile struct BlueOsCallBack* next;
	CallBackFunc func;	
}BlueOsCallBack;

#endif
