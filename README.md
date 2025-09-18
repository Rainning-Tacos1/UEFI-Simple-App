`sudo apt install clang lld`

`make build && make disk && make qemu`

Ia32:
`qemu-system-i386.exe -bios ovmf32.bin -hda disk.img`

x86_64:
`qemu-system-x86_64.exe -bios ovmf64.bin -hda disk.img`

Gop:
`https://wiki.osdev.org/GOP#Detecting_GOP`

MAKE A README LATER
