# assemble with AVR Assembler (AVRA)

MCU=attiny85
# run FOSC at 1 MHz
FUSE_L=0x62
# run FOSC at 16 kHz
#FUSE_L=0x44
FUSE_H=0xDF
FUSE_E=0xFF
F_CPU=800000
PICOUART=picoUART/src/
INCLUDES:=-I${PICOUART}
AS=avra
CC=avr-gcc
OBJCOPY=avr-objcopy
CXX=avr-c++
STRIP=avr-strip
LD=avr-ld
CXXFLAGS=-Os -g -flto -std=c++20 -DF_CPU=${F_CPU} -mmcu=${MCU}# -Wall -pedantic
CFLAGS=-Os -g -DF_CPU=${F_CPU} -mmcu=${MCU}# -Wall -pedantic
CPPFLAGS=${INCLUDES}
AVRDUDE=avrdude
TARGET=endsensors
VPATH=${PICOUART}

OBJS := endsensors.o picoUART.o pu_print.o rxISR.o

all: ${OBJS}
	${CC} ${OBJS} -o ${TARGET}
	${STRIP} ${TARGET}


${TARGET}.hex: ${TARGET}
		${OBJCOPY} -O ihex ${TARGET} ${TARGET}.hex	

flash: ${TARGET}.hex
		${AVRDUDE} -p ${MCU} -c avrisp -B250 -U flash:w:${TARGET}.hex:i -F -P /dev/ttyUSB0

fuse:
		$(AVRDUDE) -p ${MCU} -c avrisp -P /dev/ttyUSB0 -B250 -U hfuse:w:${FUSE_H}:m -U lfuse:w:${FUSE_L}:m efuse:w:${FUSE_L}:m

clean:
		rm -f *.o *.hex ${TARGET}

