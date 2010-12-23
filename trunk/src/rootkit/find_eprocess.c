/*
** File 	find_eprocess.c
** Author 	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		These two functions find process by PID or Name
**			and return address of _EPROCESS structure
*/

#include <ntddk.h>
#include "driver.h"

PEPROCESS			FindEPROCESSByPID(ULONG pidToFind)
{
	PEPROCESS		eproc;
	ULONG  			currentPID;
	ULONG  			startPID;
	ULONG  			i;
	PLIST_ENTRY		plActivProcs;
		
	if (pidToFind == 0)
		return (NULL);

	eproc = PsGetCurrentProcess();
	startPID = *((ULONG *)((ULONG) eproc + EPROCESS_PID_OFFSET));
	currentPID = startPID;
	
	i = 0;
	DbgPrintEx(DFLAGS, "Searching PID\n");
	while (42)
	{
		DbgPrintEx(DFLAGS, "> %d\n", currentPID);
		if (pidToFind == currentPID)
			return eproc;
		else if((i >= 1) && (startPID == currentPID))
			return (0);
		else
		{
			plActivProcs = (LIST_ENTRY *) ((ULONG) eproc + EPROCESS_FLINK_OFFSET);
			eproc = (PEPROCESS) plActivProcs->Flink;
			eproc = (PEPROCESS)((ULONG) eproc - EPROCESS_FLINK_OFFSET);
			currentPID = *((ULONG *)((ULONG) eproc + EPROCESS_PID_OFFSET));
			i++;
		}
	}
}

PEPROCESS			FindEPROCESSByName(PCHAR toFind)
{
	PEPROCESS		eproc;
	PLIST_ENTRY		PCurList;
	PEPROCESS		startValue;
	PCHAR			curProcName;

	DbgPrintEx(DFLAGS, "Trying to find process: %s\n", toFind);
    eproc = IoGetCurrentProcess();
    startValue = eproc;
	do 
	{
		curProcName = (PCHAR) eproc + EPROCESS_IMFILENAME_OFFSET;
		DbgPrintEx(DFLAGS, "> Process: %s\n", curProcName);
        if(strncmp(toFind, curProcName, strlen(toFind)) == 0)
            break;

        PCurList = (PLIST_ENTRY) ((PCHAR)eproc + EPROCESS_FLINK_OFFSET);
        eproc = (PEPROCESS) ((PCHAR)PCurList->Flink - EPROCESS_FLINK_OFFSET);
    } while(eproc != startValue);

    if(eproc == startValue)
        return ((ULONG) 0x00);

	DbgPrintEx(DFLAGS, "%s EPROCESS address: %x\n", toFind, eproc);
	return (eproc);
}
