#include "ntddk.h"

VOID Unload(PDRIVER_OBJECT DriverObject)
{
	DbgPrint("Driver unload \r\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	DriverObject->DriverUnload = Unload;

	DbgPrint("Hello World Driver \r\n");
	return STATUS_SUCCESS;
}