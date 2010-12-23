/*
** File 	irp_functions.c
** Author 	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		This file contain all the function binded on irp's
*/

#include "driver.h"

NTSTATUS	Fonction_IRP_MJ_CREATE(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    DbgPrintEx(DFLAGS, "IRP_MJ_CREATE\n");
    return (STATUS_SUCCESS);
}

NTSTATUS	Fonction_IRP_MJ_CLOSE(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    DbgPrintEx(DFLAGS, "IRP_MJ_CLOSE\n");
    return (STATUS_SUCCESS);
}

NTSTATUS				Fonction_IRP_DEVICE_CONTROL(
							IN PDEVICE_OBJECT DeviceObject,
							OUT PIRP Irp
)
{
    PIO_STACK_LOCATION  pIoStackLocation;
    PVOID				pBuf = Irp->AssociatedIrp.SystemBuffer;
	PCHAR				recStr;
	NTSTATUS			ret;

    DbgPrintEx(DFLAGS, "IRP_MJ_DEVICE_CONTROL\n");
    
	pIoStackLocation = IoGetCurrentIrpStackLocation(Irp);
	recStr = (PCHAR) pBuf;
    
	DbgPrintEx(DFLAGS, "Received : %s\n", recStr);
	ret = ExecCommand(recStr);
	RtlZeroMemory(pBuf, 
		pIoStackLocation->Parameters.DeviceIoControl.InputBufferLength);
	if (ret == STATUS_SUCCESS)
	{
		DbgPrintEx(DFLAGS, "Sending : %s\n", SUCCESS_STR);
		RtlCopyMemory(pBuf, SUCCESS_STR, strlen(SUCCESS_STR));
		Irp->IoStatus.Information = strlen(SUCCESS_STR);
	}
	else
	{
		DbgPrintEx(DFLAGS, "Sending : %s\n", FAILURE_STR);
		RtlCopyMemory(pBuf, FAILURE_STR, strlen(FAILURE_STR));
		Irp->IoStatus.Information = strlen(FAILURE_STR);
	}
	Irp->IoStatus.Status = STATUS_SUCCESS;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return (STATUS_SUCCESS);
}

VOID	InitIRPFunctions(OUT PDRIVER_OBJECT pDriverObject)
{
    pDriverObject->MajorFunction[IRP_MJ_CREATE] = Fonction_IRP_MJ_CREATE;
    pDriverObject->MajorFunction[IRP_MJ_CLOSE] = Fonction_IRP_MJ_CLOSE;
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Fonction_IRP_DEVICE_CONTROL;
}
