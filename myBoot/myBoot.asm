[BITS 16]  ; indique a nasm que l'on travaille en 16 bits
[ORG 0x0]

%define USE_CLEARSCR
;%define USE_DRAW_SPRITE
%define USE_DRAW_MONOCHROM_ICON
%define USE_VSYNC
;%define USE_SWAP_BUFFER
;%define USE_PRINTSTR
;%define USE_PRINTINT

%include '../mydrawM.inc'

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; initialisation des segments en 0x07C00
    mov ax, 0x07C0
    mov ds, ax
;   mov es, ax ; will be defined later ...
    mov ax, 0x8000
    mov ss, ax
    mov sp, 0xf000    ; stack de 0x8F000 -> 0x80000

%include '../mainInva.inc'
%include '../mydrawF.inc'

;--- NOP jusqu'a 510 ---
;    times 510-($-$$) db 0x90 ; NOP
    dw 0xAA55

; 414
; partitions : 446
