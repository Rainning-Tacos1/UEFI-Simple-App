#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>

// basicIO.c
//extern void ClearScreen(UINT32 bg);
//extern void SetTextBG(UINT32 color);
//extern void SetTextColor(UINT32 color);
extern void Print(unsigned short* str);
extern void PrintC(unsigned short* str, UINT32 color);
//extern void PrintInt(INT32 num);
//extern void BasicIOInit(void);

void exit(unsigned short* msg) {

	PrintC(u"Exiting with error code : -1\r\n\r\n", EFI_RED);
	PrintC(msg, EFI_WHITE);


	PrintC(u"\r\nPress any key to shutdown ...", EFI_RED);
	// Wait until keypress, then return
	EFI_INPUT_KEY key;
	while (gST->ConIn->ReadKeyStroke(gST->ConIn, &key) != EFI_SUCCESS);

	// Shutdown, does not return
	gST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

}