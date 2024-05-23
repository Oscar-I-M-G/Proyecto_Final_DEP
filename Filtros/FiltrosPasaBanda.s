;.....
;    File   PasaBanda.s 
;.....

    .equ PasaBandaNumTaps, 51

;.....
; Allocate and initialize filter taps

		.section .PasaBandaconst, "x"
		.align 128
PasaBandaTaps:
.hword 0x0099, 0x00bf, 0x007f, 0xffd5, 0xfefc, 0xfe79, 0xfede, 0x004e, 0x0228 
.hword 0x032e, 0x0251, 0xff8f, 0xfc4d, 0xfaae, 0xfc31, 0x007b, 0x053a, 0x076b 
.hword 0x0546, 0xffa1, 0xf9aa, 0xf710, 0xf9a8, 0x0024, 0x06bd, 0x097e, 0x06bd 
.hword 0x0024, 0xf9a8, 0xf710, 0xf9aa, 0xffa1, 0x0546, 0x076b, 0x053a, 0x007b 
.hword 0xfc31, 0xfaae, 0xfc4d, 0xff8f, 0x0251, 0x032e, 0x0228, 0x004e, 0xfede 
.hword 0xfe79, 0xfefc, 0xffd5, 0x007f, 0x00bf, 0x0099 

;.....
; Allocate delay line in (uninitialized) Y data space

		.section .ybss,  "b"
		.align 128

PasaBandaDelay:
		.space PasaBandaNumTaps*2

; .....
; Allocate and intialize filter structure

		.section .data
		.global _PasaBandaFilter

_PasaBandaFilter:
.hword PasaBandaNumTaps
.hword psvoffset(PasaBandaTaps)
.hword psvoffset(PasaBandaTaps)+PasaBandaNumTaps*2-1
.hword psvpage(PasaBandaTaps)
.hword PasaBandaDelay
.hword PasaBandaDelay+PasaBandaNumTaps*2-1
.hword PasaBandaDelay

; ....................................
; ....................................
