;.....
;    File   FIR11792.s 
;.....

    .equ FIR11792NumTaps, 51

;.....
; Allocate and initialize filter taps

		.section .FIR11792const, "x"
		.align 128
FIR11792Taps:
.hword 0xff4a, 0xff39, 0xff38, 0xff52, 0xff9b, 0x001e, 0x00db, 0x01bd, 0x029a 
.hword 0x0338, 0x035d, 0x02d9, 0x019e, 0xffc3, 0xfd87, 0xfb4d, 0xf986, 0xf89b 
.hword 0xf8d7, 0xfa4f, 0xfcdd, 0x0022, 0x0395, 0x069c, 0x08af, 0x096b, 0x08af 
.hword 0x069c, 0x0395, 0x0022, 0xfcdd, 0xfa4f, 0xf8d7, 0xf89b, 0xf986, 0xfb4d 
.hword 0xfd87, 0xffc3, 0x019e, 0x02d9, 0x035d, 0x0338, 0x029a, 0x01bd, 0x00db 
.hword 0x001e, 0xff9b, 0xff52, 0xff38, 0xff39, 0xff4a 

;.....
; Allocate delay line in (uninitialized) Y data space

		.section .ybss,  "b"
		.align 128

FIR11792Delay:
		.space FIR11792NumTaps*2

; .....
; Allocate and intialize filter structure

		.section .data
		.global _FIR11792Filter

_FIR11792Filter:
.hword FIR11792NumTaps
.hword psvoffset(FIR11792Taps)
.hword psvoffset(FIR11792Taps)+FIR11792NumTaps*2-1
.hword psvpage(FIR11792Taps)
.hword FIR11792Delay
.hword FIR11792Delay+FIR11792NumTaps*2-1
.hword FIR11792Delay

; ....................................
; ....................................
