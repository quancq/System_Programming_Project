Hướng dẫn cài đặt
==============
# Giới thiệu
* Đề tài bài tập lớn: Lập trình driver cho chức năng đọc, ghi dữ liệu trên window
* Khi lập trình driver, ta cần sử dụng hai máy tính
	* Máy thứ nhất đóng vai trò là host, là máy để hỗ trợ lập trình
	* Máy thứ hai đóng vai trò là target, là máy để debug, thử nghiệm cài đặt, đánh giá kết quả thực hiện của chương trình driver. Thông thường nên chọn máy ảo làm máy target vì khi chương trình driver viết ra có lỗi thì có thể gây lỗi cho hệ điều hành.
* Phần tiếp theo sẽ hướng dẫn cài đặt môi trường lập trình (trên máy host) và môi trường debug (trên máy target). Chi tiết hướng dẫn cài đặt tham khảo ở [Windows Kernel Programming Tutorial](https://www.youtube.com/playlist?list=PLZ4EgN7ZCzJx2DRXTRUXRrB2njWnx1kA2)
* Sau đó sẽ giới thiệu một số hàm cơ bản cần dùng trong lập trình

# 1. Cài đặt môi trường lập trình
* Cài đặt Visual Studio 2017 (hỗ trợ lập trình driver), trong đó Software Development Kit (SDK) sẽ được cài đặt tự động
* Cài đặt Windows Driver Kit (WDK)
* [Link hướng dẫn](https://docs.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk) 

# 2. Cài đặt môi trường debug
* Cài đặt phần mềm tạo máy ảo. Có thể cài [VirtualBox](https://www.virtualbox.org/wiki/Downloads) hoặc [VMWare](https://www.vmware.com/products/workstation-pro/workstation-pro-evaluation.html). Phần mềm này sẽ chạy trên máy host, tạo ra máy ảo để dùng việc debug driver
	* Sau khi khởi động được máy ảo (cần cài đặt hệ điều hành vào máy ảo) thì cần cài thêm chương trình Guest Addition để có thể kéo thả các tệp từ máy host vào thẳng máy ảo
* Cài đặt phần mềm [Driver Loader](https://www.osronline.com/article.cfm?article=157) để load driver vào máy target
* Cài đặt tool hỗ trợ debug lập trình kernel driver trên máy ảo [VirtualKD](http://virtualkd.sysprogs.org/)

# 3. Một số hàm trong lập trình driver
* ``NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)``: Hàm được gọi đầu tiên khi driver được load vào hệ thống
	* DriverObject là con trỏ trỏ đến đối tượng biểu diễn cho driver
* ``DbgPrint(char* msg)``: Hiển thị xâu lên màn hình debug
* ``RtlInitUnicodeString(UNICODE_STRING* us, char* msg)``: Hàm này khởi tạo một đối tương thuộc kiểu UNICODE_STRING, trong đó chứa nội dung xâu msg, độ dài xâu, độ dài tối đa
* ``InitializeObjectAttributes(OBJECT_ATTRIBUTES* oa, UNICODE_STRING* us, Flag f, NULL, NULL)``: Khởi tạo đối tương thuộc kiểu OBJECT_ATTRIBUTES
* ``ZwCreateFile(Handle* h, DesiredAccess da, OBJECT_ATTRIBUTES* oa, IO_STATUS_BLOCK* isb, NULL, NULL)``: Khởi tạo một handle để thực hiện đọc hoặc ghi file
* ``ZwClose(Handle h)``: đóng handle
* ``NT_SUCCESS(NTSTATUS status)``: kiểm tra status có thành công không
* ``ZwReadFile(Handle h, NULL, NULL, NULL, IO_STATUS_BLOCK* isb, PVOID buff, ULONG buffSize, PLARGE_INTEGER offset, NULL)``: Đọc file được chỉ định bởi handle. Vị trí đọc bắt đầu từ offset. Số byte đọc là buffSize. Kết quả đọc được ghi vào buff
* ``ZwWriteFile(Handle h, NULL, NULL, NULL, IO_STATUS_BLOCK* isb, PVOID buff, ULONG buffSize, PLARGE_INTEGER offset, NULL)``: Ghi file được chỉ định bởi handle. Vị trí ghi bắt đầu từ offset. Số byte ghi là buffSize. Nội dung ghi lấy từ buff
* ``ZwQueryInformationFile(Handle h, IO_STATUS_BLOCK* isb, PVOID fileInfo, ULONG length, FILE_INFORMATION_CLASS fic)``: Truy vấn một số thông tin cơ bản của file (thời gian tạo file, thời gian sửa file lần cuối, kích thước file). Kết quả được lưu trong ô nhớ mà biến fileInfo trỏ đến
* Tài liệu chi tiết các hàm: [Windows Kernel](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/_kernel/)