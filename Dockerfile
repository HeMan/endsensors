FROM alpine:3.17
RUN apk add --no-cache gcc-avr avr-libc make avrdude
