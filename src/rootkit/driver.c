/*
** File 	driver.c
** Author 	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		This file contain DriverEntry 
**			and Unload function	DriverEntry 
**			is the entry point of the driver 
**			when starting service Unload is the 
**			function called when stopping the service.
*/

#include <ntddk.h>
#include "driver.h"
#include "hook.h"

NTSTATUS 			DriverEntry(
						IN PDRIVER_OBJECT pDriverObject,
						IN PUNICODE_STRING theRegistryPath)
{
    DbgPrintEx(DFLAGS, "Loading Driver...\n");
	if (initHooks() != STATUS_SUCCESS)
	{
		removeHooks();
		return (STATUS_UNSUCCESSFUL);
	}
    pDriverObject->DriverUnload = Unload;
	InitIRPFunctions(pDriverObject);
	return (InitDevice(pDriverObject));
}

NTSTATUS			InitDevice(IN PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS        ret;
    UNICODE_STRING  interfaceName;
    UNICODE_STRING  linkName;
    PDEVICE_OBJECT  pInterface;

    RtlInitUnicodeString(&interfaceName, DEVICE_NAME);   
    pInterface = NULL;
    ret = IoCreateDevice(pDriverObject, 
			0, 
			&interfaceName, 
			FILE_DEVICE_UNKNOWN, 
			FILE_DEVICE_UNKNOWN, 
			FALSE, 
			&pInterface);
    if (ret != STATUS_SUCCESS)
	{
        DbgPrintEx(DFLAGS, "Creating device failed\n");
		return (ret);
	}
	DbgPrintEx(DFLAGS, "Creating device succeed\n");
    RtlInitUnicodeString(&linkName, LINK_NAME);
    ret = IoCreateSymbolicLink(&linkName, &interfaceName);
    if (ret != STATUS_SUCCESS)
		DbgPrintEx(DFLAGS, "Creating symbolic link failed\n");
	else
		DbgPrintEx(DFLAGS, "Creating symbolic link succeed\n");
	return (ret);
}

VOID				Unload(IN struct _DRIVER_OBJECT *pDriverObject)
{
	UNICODE_STRING	linkName;
	NTSTATUS		ret;

	DbgPrintEx(DFLAGS, "Unload driver\n");
	removeHooks();
	RtlInitUnicodeString(&linkName, LINK_NAME);
	ret = IoDeleteSymbolicLink(&linkName);
	if (ret != STATUS_SUCCESS)
		DbgPrintEx(DFLAGS, "Deleting symbolic link failed\n");
	else
		DbgPrintEx(DFLAGS, "Deleting symbolic link succeed\n");
    IoDeleteDevice(pDriverObject->DeviceObject);
	DbgPrintEx(DFLAGS, "Deleting device succeed\n");
}
