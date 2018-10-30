#include "ntddk.h"

VOID Unload(PDRIVER_OBJECT DriverObject)
{
	DbgPrint("\nDriver unload \r\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	DriverObject->DriverUnload = Unload;
	DbgPrint("\n=============== READ FILE DRIVER ===============\n");
	DbgPrint("Nhom phat trien driver: CHU QUOC QUAN - BACH VIET DUNG");
	DbgPrint("\nDriver thuc hien doc noi dung file va hien thi len man hinh");
	DbgPrint("\n=============== READ FILE DRIVER ===============\n");

	OBJECT_ATTRIBUTES objAttributes;
	NTSTATUS status;
	HANDLE fileHandle;
	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	UNICODE_STRING filePath;
	RtlInitUnicodeString(&filePath, L"\\??\\C:\\Users\\Quân\\Desktop\\Demo_LTHT\\ReadFile\\input.txt");

	InitializeObjectAttributes(&objAttributes, &filePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	// Open file
	status = ZwCreateFile(
		&fileHandle, GENERIC_READ, &objAttributes, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 
		FILE_SHARE_READ, FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

	if (!NT_SUCCESS(status))
	{
		DbgPrint("Error when open file %ls\r\n", filePath.Buffer);
	}
	else 
	{
		DbgPrint("Open file %ls is successfull\r\n", filePath.Buffer);
		DbgPrint("File content:\r\n");

		PVOID buff;
		ULONG buffSize = 1024, actualSize;
		LARGE_INTEGER offset = { 0 };

		buff = ExAllocatePoolWithTag(NonPagedPool, buffSize, 1);
		// Read file content
		if (buff != NULL)
		{
			while (1)
			{
				status = ZwReadFile(fileHandle, NULL, NULL, NULL, &ioStatusBlock, buff, buffSize-1, &offset, NULL);
				if (!NT_SUCCESS(status))
				{
					if (status == STATUS_END_OF_FILE)
					{
						DbgPrint("\nRead EOF file %ls\r\n", filePath.Buffer);
					}
					break;
				}
				actualSize = ioStatusBlock.Information;
				char* input = (char*)buff;
				input[actualSize] = '\0';
				DbgPrint("%s", input);

				offset.QuadPart += actualSize;
			}

			// Show basic file information
			DbgPrint("\nBasic file information:\r\n");

			FILE_STANDARD_INFORMATION fileStandardInfo = { 0 };
			status = ZwQueryInformationFile(
				fileHandle, &ioStatusBlock, &fileStandardInfo, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);
			ULONG fileSize = fileStandardInfo.EndOfFile.QuadPart;
			DbgPrint("File size: %d\r\n", fileSize);

			FILE_BASIC_INFORMATION fileBasicInfo = { 0 };
			status = ZwQueryInformationFile(
				fileHandle, &ioStatusBlock, &fileBasicInfo, sizeof(FILE_BASIC_INFORMATION), FileBasicInformation);
			DbgPrint("File created time: %d\r\n", fileBasicInfo.CreationTime);

			// Deallocate buffer
			if (buff != NULL)
				ExFreePool(buff);
		}
		else
		{
			DbgPrint("Error when allocate memory for buffer\r\n");
		}
	}

	// Close handle file
	if (fileHandle != NULL)
	{
		ZwClose(fileHandle);
		DbgPrint("\nClose handle file is done");
	}

	return STATUS_SUCCESS;
}