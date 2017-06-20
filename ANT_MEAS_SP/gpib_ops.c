//***************************************************************************
//   File name        :   
//   Module name      :   
//   Author           :   Lyu Yang
//   Email            :   
//   Date             :   
//   Version          :   v1.0
//
//   Abstract         :   
//
//   Modification history
//   ------------------------------------------------------------------------
// Version       Date(yyyy/mm/dd)   name
// Description
//
// $Log$
//***************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "ni4882.h"

#define TIMEOUT               T10s  // Timeout value = 10 seconds
#define EOTMODE               1     // Enable the END message
#define EOSMODE               0     // Disable the EOS mode

// Array Size For Read Buffer
const long BUF_SIZE = 1024L;

static void GPIB_DevParse(char *dev, int *bd_index, int *gpib_dev_adr)
{
	int i = 0, j = 0;
	char bd_str[4], gpib_adr[4];

	for (i = 4; dev[i] != ':'; ++i)
		bd_str[i - 4] = dev[i];
	bd_str[i - 4] = '\0';

	for (j = i + 1; dev[j] != '\0'; ++j)
		gpib_adr[j - i - 1] = dev[j];
	gpib_adr[j - i - 1] = '\0';

	*bd_index = atoi(bd_str);
	*gpib_dev_adr = atoi(gpib_adr);
	return;
}


// GPIB Send Command Only
int GPIB_Send(char *gdev, char * cmd)
{
	int bdindex, gpib_dev_adr;
	int dev;

	GPIB_DevParse(gdev, &bdindex, &gpib_dev_adr);

	dev = ibdev(bdindex, gpib_dev_adr, 0, TIMEOUT, EOTMODE, EOSMODE);

	if (Ibsta() & ERR)
	{
		printf("Unable to open device\nibsta = 0x%x iberr = %d\n", Ibsta(), Iberr());
		return 1;
	}

	ibclr(dev);
	if (Ibsta() & ERR)
		return 1;

	ibwrt(dev, cmd, strlen(cmd));
	if (Ibsta() & ERR)
		return 1;

	Sleep(100);

	ibonl(dev, 0);

	return 0;
}

// GPIN SendCommand and Receive Data
int GPIB_SendReceive(char *gdev, char * cmd, char * rcv)
{
	int bdindex, gpib_dev_adr;
	int dev;

	GPIB_DevParse(gdev, &bdindex, &gpib_dev_adr);

	dev = ibdev(bdindex, gpib_dev_adr, 0, TIMEOUT, EOTMODE, EOSMODE);
	if (Ibsta() & ERR)
	{
		printf("Unable to open device\nibsta = 0x%x iberr = %d\n", Ibsta(), Iberr());
		return 1;
	}

	ibclr(dev);
	if (Ibsta() & ERR)
		return 1;

	ibwrt(dev, cmd, strlen(cmd));
	if (Ibsta() & ERR)
		return 1;

	Sleep(100);

	ibrd(dev, rcv, BUF_SIZE);
	if (Ibsta() & ERR)
		return 1;
	rcv[Ibcnt() - 1] = '\0';

	ibonl(dev, 0);

	return 0;
}

