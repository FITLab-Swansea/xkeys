/***************************************************
 X-Keys Test Program

 The code may be used by anyone for any purpose,
 and can serve as a starting point for developing
 applications using the X-Keys libary.
 
 Alan Ott
 Signal 11 Software
***************************************************/

#include "PieHid32.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void clearBacklight(int bank, long handle) {

	/* bank: 0 = bank 1, 1 = bank 2
	*/

	unsigned int result;
	unsigned char buffer[33];
	memset(buffer, 0, sizeof(buffer));
	
	buffer[1] = 182; // Command: On/off bank of backlights
	buffer[2] = bank; //
	buffer[3] = 0;

	result = WriteData(handle, buffer);

	if (result != 0) {
		printf("Unable to write to Device.");
	}
}

void setBacklight(int index, int state, long handle) {

	/*
		index: 0-29
		state: 0 (off), 1, (on), 2 (flash)
	*/

	unsigned int result;
	unsigned char buffer[33];
	memset(buffer, 0, sizeof(buffer));
	buffer[1]=181; // Command: Index Based Set Backlights (Flash)
	buffer[2] = index; // Key Index
	buffer[3] = state;

	result = WriteData(handle, buffer);

	if (result != 0) {
		printf("Unable to write to Device.");
	}
}

char bitsToNum(char bits)
{
	switch(bits) {
		case 4: return 3;
		case 8: return 4;
		case 16: return 5;
		case 32: return 6;
	}	
	return bits;
}

int getIndexOfKey(char *data)
{
	printf("Char: %d\n",data[4]);
	if(data[3] > 0)
		return bitsToNum(data[3])-1;

	if(data[4] > 0)
		return bitsToNum(data[4])+7;

	if(data[5] > 0 )
		return bitsToNum(data[5])+15;

	if(data[6] > 0)
		return bitsToNum(data[6])+23;

	return -1; 

}


void print_buf(char *data, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		printf("%02hhx ", data[i]);
		if ((i+1) % 8 == 0)
			printf("  ");
		if ((i+1) % 16 == 0)
			printf("\n");
	}
	printf("\n\n");

}

int main(void)
{
	TEnumHIDInfo info[128];
	long count;
	int i;
	long handle = -1;
	
	unsigned res = EnumeratePIE(PI_VID, info, &count);
	
	for (i = 0; i < count; i++) {
		TEnumHIDInfo *dev = &info[i];
		printf("Found XKeys Device:\n");
		printf("\tPID: %04x\n", dev->PID);
		printf("\tUsage Page: %04x\n", dev->UP);
		printf("\tUsage:      %04x\n", dev->Usage);
		printf("\tVersion: %d\n\n", dev->Version);


		handle = dev->Handle;
		unsigned int res = SetupInterfaceEx(handle);
		if (res != 0) {
			printf("Unabe to open device. err: %d\n", res);
		}
		break;
	}
	
	if (handle < 0) {
		printf("Unable to open device\n");
		exit(1);
	}

	clearBacklight(0,handle);
	
	char data[33];
	int index;

	while (1) {
		
		unsigned int res = 0;
/*
		res  = ReadLast(handle, data);
		if (res == 0) {
			printf("LAST: \n");
			print_buf(data, 33);
			printf("ENDLAST\n\n");
		}
*/
		//res = 0;
		
		//while (res == 0) {
			res = BlockingReadData(handle, data, 100);
			if (res == 0) {
				print_buf(data, 33);
				index = getIndexOfKey(data);
				printf("Index: %d\n",index);
				if(index >= 0)
					setBacklight(index,1,handle);


			}
			 else if (res == PIE_HID_READ_INSUFFICIENT_DATA) {
			//	printf(".");
			//	fflush(stdout);
			}	
			else {
				printf("Error Reading\n");
			}
		//}
		

		//printf("Sleeping\n");
		//#if 1
		//if (res != 0) {
		//	usleep(10*1000); // Sleep 10 milliseconds.
		//	sleep(2); // 2 seconds
		//}
		//#endif
		
		//ClearBuffer(handle);
		
	}


	return 0;
}
