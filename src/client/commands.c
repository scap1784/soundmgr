/*
** File		commands.c
** Author	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		command executing
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "client.h"

t_command	gCommandList[] = {
	{"--start", &startService, 0},
	{"--stop", &stopService, 0},
	{"--restart", &restartService, 0},
	{"--help", &printUsage, 0},
	{"--hp", &sendToDevice, 1},
	{"--hf", &sendToDevice, 1},
	{"--root", &sendToDevice, 1},
	{"--reboot", &surviveReboot, 1},
	{NULL, NULL, 0}
};

int			getNbParams(char *toFind)
{
	int		i;
	
	for (i = 0; gCommandList[i].cmdStr != NULL; i++)
	{
		if (strcmp(gCommandList[i].cmdStr, toFind) == 0)
			return (gCommandList[i].nbParams);
	}
	return (-1);
}

char		*concatParams(char *argv[], int NbParams)
{
	char	*res;
	int		size;
	int		i;

	res = 0;
	if (NbParams > 0)
	{
		size = 0;
		for (i = 0; i < NbParams; i++)
			size += strlen(argv[i]) + 1;
		
		res = malloc(size * sizeof(*res) + 1);
		if (res != NULL)
		{
			res[0] = 0;
			for (i = 0; i < NbParams; i++)
			{
				res = strcat(res, argv[i]);
				res = strcat(res, " ");
			}
			res[strlen(res) - 1] = 0;
		}
	}
	return (res);
}

void		execCommand(char *cmd, char *params)
{
	int		i;

	for (i = 0; gCommandList[i].cmdStr != NULL; i++)
	{
		if (strcmp(gCommandList[i].cmdStr, cmd) == 0)
			(*gCommandList[i].execCmd)(params);
	}
}
