/*
** File		process.c
** Author	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		about process
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winioctl.h>
#include "client.h"

void		sendToDevice(char *cmd)
{
	HANDLE	hDevice;
    DWORD	nbByte;
	char	*out[256];

	ZeroMemory(out, sizeof(out));
	hDevice = CreateFile(
		SYMLINK,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (!hDevice)
	{
		printf("Connecting device failed\n");
		return;
	}

	printf("Sending: %s\n", cmd);
	DeviceIoControl(
		hDevice,
		IOCTL_CODE,
		cmd,
		strlen(cmd) + 1,
		out,
		sizeof(out),
		&nbByte,
		NULL);

	printf("Received: %s\n", out);
	CloseHandle(hDevice);
	return;
}
