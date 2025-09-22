#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>

#include "67.h"

EFI_HANDLE gImageHandle;
EFI_SYSTEM_TABLE* gST;
EFI_BOOT_SERVICES* gBS;
EFI_GUID gEfiGraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;


// basicIO.c
extern void ClearScreen(UINT32 bg);
extern void SetTextBG(UINT32 color);
extern void SetTextColor(UINT32 color);
extern void Print(unsigned short* str);
extern void PrintC(unsigned short* str, UINT32 color);
extern void PrintInt(INT32 num);
extern void BasicIOInit(void);

// exit.c
extern void exit(unsigned short* msg);

// halt.c
extern void halt(void);

// Entry point for UEFI applications
EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
	gST = SystemTable; gImageHandle = ImageHandle; gBS = SystemTable->BootServices;
	EFI_STATUS Status;
	
	// Basic IO
	BasicIOInit();
	
	ClearScreen(EFI_BLACK);
	SetTextColor(EFI_WHITE);

	Print(u"Starting ...\r\n");

	Print(u"Locating Gop ...\r\n");
	EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
	Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&Gop);

	if (EFI_ERROR(Status)) exit(u"Could not locate Gop!\r\n");
	else Print(u"Found Gop\r\n");

	PrintC(u"\r\nGop Modes:\r\n", EFI_LIGHTGRAY);

	// List all GOPs
	for (UINT32 i = 0; i < Gop->Mode->MaxMode; i++) {

		Print(u"[");
		if(Gop->Mode->Mode == i) PrintC(u"*", EFI_RED);
		else Print(u" ");
		Print(u"] ");

		UINT32 SizeOfInfo;
		EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* Info;
		Gop->QueryMode(Gop, i, &SizeOfInfo, &Info);
		Print(u"Mode "); PrintInt(i); Print(u" | ");
		PrintInt(Info->HorizontalResolution);  Print(u" x "); PrintInt(Info->VerticalResolution);
		Print(u" Format: ");

		UINT16* pixel_formats[4] = {u"0xRRGGBB--", u"0xBBGGRR--", u"0xMMMMMMMM", u"0x--------"};
		Print( Info->PixelFormat < PixelFormatMax ?
				pixel_formats[Info->PixelFormat] :
				u"Invalid"
		);
		Print(u"\r\n");
	}

	EFI_PHYSICAL_ADDRESS Addr = 0;
	UINTN Pages = EFI_SIZE_TO_PAGES(640 * 480 * 4); // macro rounds up to nearest page
	Status = gBS->AllocatePages(
		AllocateAnyPages,
		EfiLoaderData,
		Pages,
		&Addr
	);
	if (EFI_ERROR(Status)) exit(u"Could not Allocate frame buffer!\r\n");

	halt();
	Status = Gop->SetMode(Gop, 1);
	//gST->ConOut->Reset(gST->ConOut, FALSE);
	if(EFI_ERROR(Status)) exit(u"Could not set Gop mode\r\n");


	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *fb = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)(UINTN)Addr;
	for(int i=0; i<640*480; ++i) {
		UINTN pix = 3 * i;
		fb[i].Red = __src_67_bin[pix];
		fb[i].Green = __src_67_bin[pix+1];
		fb[i].Blue = __src_67_bin[pix+2];
	}

	Status = Gop->Blt(
		Gop,
		fb,
		EfiBltBufferToVideo,
		0, 0,
		0, 0,
		640, 480,
		0
	);


	EFI_INPUT_KEY key;
	while (gST->ConIn->ReadKeyStroke(gST->ConIn, &key) != EFI_SUCCESS);

	// Shutdown, does not return
	gST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	//exit(u"End of program, no errors\r\n");
    return EFI_SUCCESS;
}
