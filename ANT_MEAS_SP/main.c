#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "gpib_ops.h"


int __cdecl main(int argc, char **argv)
{
	char gdev_mf[20], gdev_sp[20];
	int npoints;
	double step;

	char cmd[80];
	char str[1024];
	FILE *fp;

	if (argc != 4) {
		printf("\nUseage: ANT_MEAS_SP.exe GPIBx:MF7802 GPIBx:N9020A NumberofTestpoints\n"
			"Example: ANT_MEAS_SP.exe GPIB0:9 GPIB0:13 101\n\n"
			);
		return 0;
	}
	
	strcpy(gdev_mf, argv[1]);
	strcpy(gdev_sp, argv[2]);
	npoints = atoi(argv[3]);

	fp = fopen("ANT_MEAS_SP.txt", "w");

	GPIB_SendReceive(gdev_mf, "*IDN?", str);
	printf("%s\n", str);
	fprintf(fp, "%s\n", str);
	GPIB_SendReceive(gdev_sp, "*IDN?", str);
	printf("%s\n", str);
	fprintf(fp, "%s\n", str);
	fflush(fp);

	// TEST BEGIN
	step = 240.0 / (double)npoints;

	for (int i = 0; i <= npoints; i++) {
		// MF7802
		sprintf(cmd, "TG%.1f", -120.0 + i * step);
		printf("%.3f, ", 1.5 * (-120.0 + i * step));
		fprintf(fp, "%.3f, ", 1.5 * (-120.0 + i * step));
		fflush(fp);

		GPIB_Send(gdev_mf, cmd);
		Sleep(100);
		GPIB_Send(gdev_mf, "SK");
		Sleep(1000);
		do {
			Sleep(500);
			GPIB_SendReceive(gdev_mf, "*OPC?", str);
		} while (str[0] == '0');
		
		// N9020A
		GPIB_SendReceive(gdev_sp, "CALC:MARK:Y?", str);

		printf("%s\n", str);
		fprintf(fp, "%s\n", str);
		fflush(fp);
	}

	fclose(fp);

	GPIB_Send(gdev_mf, "TG0");
	Sleep(100);
	GPIB_Send(gdev_mf, "SK");
	Sleep(100);

	return 0;
}
