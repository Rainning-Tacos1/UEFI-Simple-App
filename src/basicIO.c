#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>

/* Internal */
UINT32 EFI_FG_COLOR = EFI_WHITE;
UINT32 EFI_BG_COLOR = EFI_BLACK;

/* Internal */
void set_efi_text_attr(UINT32 fg, UINT32 bg) {
	EFI_FG_COLOR = fg;
	EFI_BG_COLOR = bg;
	gST->ConOut->SetAttribute(gST->ConOut, (UINTN)(EFI_TEXT_ATTR(fg, bg))); 
}

/* Internal */
void get_efi_text_attr(UINT32* fg, UINT32* bg) {
	*fg = EFI_FG_COLOR;
	*bg = EFI_BG_COLOR;
}

/* External */
void ClearScreen(UINT32 bg) {
	UINT32 curr_fg, curr_bg;
	get_efi_text_attr(&curr_fg, &curr_bg);

	set_efi_text_attr(curr_fg, bg);
	gST->ConOut->ClearScreen(gST->ConOut);
	set_efi_text_attr(curr_fg, curr_bg);
}

/* External */
void SetTextBG(UINT32 color) {
	UINT32 curr_fg, curr_bg;
	get_efi_text_attr(&curr_fg, &curr_bg);
	set_efi_text_attr(curr_fg, color);
}

/* External */
void SetTextColor(UINT32 color) {
	UINT32 curr_fg, curr_bg;
	get_efi_text_attr(&curr_fg, &curr_bg);
	set_efi_text_attr(color, curr_bg);
}

/* External */
void Print(unsigned short* str) {
	gST->ConOut->OutputString(gST->ConOut, str);
}

/* External */
void PrintC(unsigned short* str, UINT32 color) {
	UINT32 curr_fg, curr_bg;
	get_efi_text_attr(&curr_fg, &curr_bg);

	SetTextColor(color);
	Print(str);
	SetTextColor(curr_fg);
	//set_efi_text_attr(curr_fg, curr_bg);
}

/* External */
void PrintInt(INT32 num) {
	CHAR16 buf[32]; // Hold the string
	buf[31] = u'\0';

	if(num == 0) {
		Print(u"0");
		return;
	}

	BOOLEAN is_negative = (num < 0);
	if(is_negative) num = -num;


	UINT32 i=30;
	while(num > 0 && i >= 0) {
		buf[i--] = u'0' + (num % 10);
		num /= 10;
	}
	if(is_negative) buf[i] = u'-'; // i points to one char before the number string
	else ++i; // Place i at the start of the string
	Print(buf+i);

}

/* External */
void BasicIOInit(void) {
	UINT32 curr_fg, curr_bg;
	get_efi_text_attr(&curr_fg, &curr_bg);
	set_efi_text_attr(curr_fg, curr_bg);
}