#include "ntddk.h"

VOID Unload(PDRIVER_OBJECT DriverObject)
{
	DbgPrint("\nDriver unload \r\n");
}

NTSTATUS copyFile(HANDLE readFileHandle, HANDLE writeFileHandle)
{
	if (readFileHandle == NULL || writeFileHandle == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}

	NTSTATUS status = STATUS_UNSUCCESSFUL;
	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	PVOID buff;
	ULONG buffSize = 1024, actualSize;
	LARGE_INTEGER offset = { 0 };

	buff = ExAllocatePoolWithTag(NonPagedPool, buffSize, 1);
	// Read file content
	DbgPrint("\nSource file content:\r\n");
	if (buff != NULL)
	{
		while (1)
		{
			status = ZwReadFile(readFileHandle, NULL, NULL, NULL, &ioStatusBlock, buff, buffSize - 1, &offset, NULL);
			if (!NT_SUCCESS(status))
			{
				if (status == STATUS_END_OF_FILE)
				{
					status = STATUS_SUCCESS;
					break;
				}
			}
			actualSize = ioStatusBlock.Information;
			char* input = (char*)buff;
			input[actualSize] = '\0';
			DbgPrint("%s", input);

			status = ZwWriteFile(writeFileHandle, NULL, NULL, NULL, &ioStatusBlock, buff, buffSize - 1, &offset, NULL);
			
			if (!NT_SUCCESS(status))
			{
				break;
			}

			offset.QuadPart += actualSize;
		}

		// Deallocate buffer
		if (buff != NULL)
			ExFreePool(buff);

		if (!NT_SUCCESS(status))
		{
			return STATUS_UNSUCCESSFUL;
		}
	}
	else
	{
		DbgPrint("Error when allocate memory for buffer\r\n");
	}

	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	DbgPrint("\n=============== COPY FILE DRIVER ===============\n");
	DbgPrint("Nhom phat trien driver: CHU QUOC QUAN - BACH VIET DUNG");
	DbgPrint("\nDriver thuc hien copy noi dung file nguon sang file dich");
	DbgPrint("\n=============== COPY FILE DRIVER ===============\n");

	DriverObject->DriverUnload = Unload;
	OBJECT_ATTRIBUTES objAttributes;
	NTSTATUS status;
	HANDLE readFileHandle = NULL, writeFileHandle = NULL;
	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	UNICODE_STRING readFilePath, writeFilePath;

	RtlInitUnicodeString(&readFilePath, L"\\??\\C:\\Users\\Quân\\Desktop\\Demo_LTHT\\CopyFile\\source.txt");
	RtlInitUnicodeString(&writeFilePath, L"\\??\\C:\\Users\\Quân\\Desktop\\Demo_LTHT\\CopyFile\\dest.txt");

	InitializeObjectAttributes(&objAttributes, &readFilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	
	// Open source file
	status = ZwCreateFile(
		&readFileHandle, GENERIC_READ, &objAttributes, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

	if (!NT_SUCCESS(status))
	{
		DbgPrint("Error when open source file %ls\r\n", readFilePath.Buffer);
	}
	else
	{
		DbgPrint("Open file %ls is successfull\r\n", readFilePath.Buffer);

		// Open dest file
		InitializeObjectAttributes(&objAttributes, &writeFilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
		status = ZwCreateFile(
			&writeFileHandle, GENERIC_WRITE, &objAttributes, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ, FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("Error when open dest file %ls\r\n", writeFilePath.Buffer);
		}
		else
		{
			// Copy file content
			status = copyFile(readFileHandle, writeFileHandle);
			if (!NT_SUCCESS(status))
			{
				DbgPrint("\nCopy file failed");
			}
			else
			{
				DbgPrint("\nCopy file is successfull");
			}
		}

	}
	
	// Close file handle
	if (readFileHandle != NULL)
	{
		ZwClose(readFileHandle);
		DbgPrint("\nClose source file handle is done");
	}
	if (writeFileHandle != NULL)
	{
		ZwClose(writeFileHandle);
		DbgPrint("\nClose dest file handle is done");
	}

	return STATUS_SUCCESS;
}