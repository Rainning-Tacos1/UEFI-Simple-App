#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>

// basicIO.c
//extern void ClearScreen(UINT32 bg);
//extern void SetTextBG(UINT32 color);
//extern void SetTextColor(UINT32 color);
//extern void Print(unsigned short* str);
extern void PrintC(unsigned short* str, UINT32 color);
//extern void PrintInt(INT32 num);
//extern void BasicIOInit(void);

void halt(void) {
	PrintC(u"Halted, press a key to continue ...\r\n", EFI_BLUE);

	// Wait until keypress, then return
	EFI_INPUT_KEY key;
	while (gST->ConIn->ReadKeyStroke(gST->ConIn, &key) != EFI_SUCCESS);
}