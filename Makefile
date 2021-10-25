# assemble with AVR Assembler (AVRA)

MCU=attiny85
# run FOSC at 1 MHz
#FUSE_L=0x62
# run FOSC at 16 kHz
FUSE_L=0x44
FUSE_H=0xDF
FUSE_E=0xFF
F_CPU=1200000
AS=avra
AVRDUDE=avrdude
TARGET=main
ASFLAGS=-I /usr/share/avra/

SRCS = endsensors.S

all:
		${AS} ${ASFLAGS} -o ${TARGET} ${SRCS}

flash:
		${AVRDUDE} -p ${MCU} -c usbasp -B250 -U flash:w:${TARGET}.hex:i -F -P usb

fuse:
		$(AVRDUDE) -p ${MCU} -c usbasp -B250 -U hfuse:w:${FUSE_H}:m -U lfuse:w:${FUSE_L}:m efuse:w:${FUSE_L}:m

clean:
		rm -f *.asm~ *.hex *.obj *.cof

