;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
;--- Macro definitions file
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: init palette from address
; Inputs  : arg1 -> palette address to load
;           arg2 -> palette size to load
;-------------------------------------------------------------------------------
%macro initPalette 2
    mov dx,03c8h    ; set the palette colors id
    xor al,al       ; init al to 0
    out dx,al       ; select the background color (0 is the background color id)
    mov dx,03c9h    ; set the palette colors data
    lea si,[%1]     ; select the palette data buffer
    mov cx,%2       ; set the palette data buffer size
    rep outsb       ; copy all the palette data into the palette buffer
%endmacro
%define BLACK_COLOR_ID           0
%define WHITE_COLOR_ID           1
%define RED_COLOR_ID             2
%define GREEN_COLOR_ID           3
%define BLUE_COLOR_ID            4
%define CYAN_COLOR_ID            5
%define YELLOW_COLOR_ID          6
%define PINK_COLOR_ID            7
%define LIGHT_RED_COLOR_ID       8
%define LIGHT_GREEN_COLOR_ID     9
%define LIGHT_BLUE_COLOR_ID     10
%define GRAY_COLOR_ID           11
%define LIGHT_CYAN_COLOR_ID     12
%define LIGHT_YELLOW_COLOR_ID   13
%define LIGHT_PINK_COLOR_ID     14
%define BACK_GROUND_COLOR_ID    BLACK_COLOR_ID

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: init VGA graphic 320x200x256 and screen address
;-------------------------------------------------------------------------------
%macro initVGAMode 0
    mov ax, 0x0013     ; video mode definition (00h) in ah 
                       ; (13h) VGA graphical 320x200x256 in al
    int 10h            ; bios call => return ax = 00
    mov ax, 0A000h
    mov es, ax
%endmacro
%define GFX_SCREEN_WIDTH  320
%define GFX_SCREEN_HEIGHT 200     

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: init to text mode 80x25x16
;-------------------------------------------------------------------------------
%macro initTextMode 0
    mov ah, 0         ; video mode definition
    mov al, 3         ; text mode 80x25x16
    int 10h           ; bios call
%endmacro
%define TXT_SCREEN_WIDTH     80

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: draw a monochrome icon on screen
; Inputs  : arg1 -> icon x position
;           arg2 -> icon y position
;           arg3 -> icon color id
;           arg4 -> icon data address
;-------------------------------------------------------------------------------
%macro drawMonochromIcon 4
    push %3                   ; the color id param [BP+0x0A]
    push %2                   ; icon y position param [BP+0x08]
    push %1                   ; icon x position param [BP+0x06]
    push %4                   ; icon data address param [BP+0x04]
    call draw_monochrom_icon ; display icon
%endmacro

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: draw a sprite on screen
; Inputs  : arg1 -> sprite x position
;           arg2 -> sprite y position
;           arg3 -> sprite data address
;-------------------------------------------------------------------------------
%macro drawSprite 3
    mov si, %3       ; sprite data address
    mov bx, %1       ; get the x position
    mov dx, %2       ; get the y position
    call draw_sprite ; display sprite
%endmacro

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: print an int
; Inputs  : arg1 -> nb to print
;-------------------------------------------------------------------------------
%macro printInt 1
    push %1
    call print_int
%endmacro

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: print a string
; Inputs  : arg1 -> string to print
;-------------------------------------------------------------------------------
%macro printStr 1
    push %1
    call print_str
%endmacro


