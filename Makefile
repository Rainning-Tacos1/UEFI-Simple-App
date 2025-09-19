

# Compiler
CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -ffreestanding -mno-stack-arg-probe -fno-stack-protector -fshort-wchar -mno-red-zone
TARGET := i686-pc-win32 #x86_64-unknown-windows #i686-pc-win32

# List of source files
SRCS := src/efi.c \
	src/basicIO.c \
	src/exit.c \
	src/halt.c


LIBS := Include \
	Include/Ia32 \
	Include/Protocol \
	Include/IndustryStandard \
	Include/Library

LIBFLAGS := $(foreach dir,$(LIBS),-I $(dir))

# Turn each .c into a .o under build/
OBJS := $(patsubst %.c,build/%.o,$(SRCS))

# Linker settings
ENTRY := UefiMain
OUTPUT := build/BOOTIA32.efi
LDFLAGS := -nostdlib \
			-fuse-ld=lld-link \
			-Wl,-subsystem:efi_application \
			-Wl,-entry:$(ENTRY)

DISK := build/disk.img
BIOS := ovmf32.bin

# Default target
all: build

# Phony build target
.PHONY: build
build: $(OUTPUT)

# Linking rule
$(OUTPUT): $(OBJS)
	$(CC) -target $(TARGET) $(CFLAGS) $(LIBFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Rule to build .o from .c
# $< = source file
# $@ = target file
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -target $(TARGET) $(LIBFLAGS) $< -o $@

# Rule: disk
disk: $(OUTPUT)
	./write_gpt -i $(DISK) -ae /EFI/BOOT/ $(OUTPUT)
	@rm FILE.TXT


qemu: $(DISK)
	qemu-system-i386.exe -bios $(BIOS) -hda $(DISK)

clean:
	rm -rf build