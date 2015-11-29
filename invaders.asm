;*******************************************************************************
; Assemble with nasm -o invader.com -f bin invader.asm
;*******************************************************************************

org 0100h

%define USE_CLEARSCR
;%define USE_DRAW_SPRITE
%define USE_DRAW_MONOCHROM_ICON
%define USE_VSYNC
;%define USE_PRINTSTR
%define USE_PRINTINT

%include 'mydrawM.inc'
%include 'mainInva.inc'
%include 'mydrawF.inc'

end:
    mov ax, 0x4C00
    int 0x21
