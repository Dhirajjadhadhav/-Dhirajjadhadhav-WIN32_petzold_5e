#include<windows.h>
#define MB_ICONHAND		0x00000010L

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	MessageBox(NULL, TEXT("Hello , windows 98!"), TEXT("HelloMsg"), MB_ICONHAND);

	return(0);
}