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
#include <windows.h>


// Array Size For Read Buffer
extern const long BUF_SIZE;

extern int GPIB_Send(char *gdev, char * cmd);
extern int GPIB_SendReceive(char *gdev, char * cmd, char * rcv);
