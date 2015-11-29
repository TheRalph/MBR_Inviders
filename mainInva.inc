%define KEY_DOWN_ARROW     0x50
%define KEY_LEFT_ARROW     0x4B
%define KEY_RIGHT_ARROW    0x4D
%define KEY_UP_ARROW       0x48
%define KEY_ESC            0x01
%define KEY_SPACE          0x39
; NB_INVADERS can not be greater than 0x0F
%define NB_INVADERS        0x0F
%define DIST_INVADERS      0x14
%define INVADER_SIZE       0x0B
%define INITIAL_INVA_X_POS 5
%define INITIAL_INVA_Y_POS 10
%define FIRE_POS_ON_SHIP   8
%define INITIAL_SHIP_X_POS 160
%define SHIP_Y_POS         180
%define SHIP_HEIGHT        10
; small is high speed, minimum is 1
%define INVADER_SPEED      6


;*******************************************************************************
;*******************************************************************************
;********* code *********
start:
    initVGAMode

    initPalette basicPalette, basicPaletteSize

.main_loop:
    mov ax, word [invadersStatus]   ; end of game if no more invaders
    test ax, ax
    jz end_intro

    call waitVSync                  ; wait for screen vertical syncro
    call cleanScr                   ; clear the screen

    rdtsc                           ; get the current clock
    mov bx, INVADER_SPEED
    div bx
    test dx, dx
    jnz .drawInvaders
;    inc word [invadersPositionX] ; BUG
    inc word [invadersPositionY]
    cmp word [invadersPositionY], SHIP_Y_POS - SHIP_HEIGHT
    jl .drawInvaders
    mov word [invadersPositionY], INITIAL_INVA_Y_POS

.drawInvaders:
    ; draw invaders; TODO: use loop
    xor cx, cx
    mov ax, word [invadersPositionX]
    .draw_invaders_loop:
        bt word [invadersStatus], cx
        jnc .next_invader
        drawMonochromIcon ax, word [invadersPositionY], WHITE_COLOR_ID, invader01Data
        .next_invader:
        add ax, DIST_INVADERS
        inc cx
        cmp cx, NB_INVADERS
    jle .draw_invaders_loop

    ; draw ship
    drawMonochromIcon word [shipPosition], SHIP_Y_POS, WHITE_COLOR_ID, shipData

    ; draw fire
    cmp word [firePositionY], 5
    jb .checkKeys
    drawMonochromIcon word [firePositionX], word [firePositionY], WHITE_COLOR_ID, fireData
    sub word [firePositionY], 4

    mov ax, word [invadersPositionY]
    cmp word [firePositionY], ax
    jb .destroyInvader
    jmp .checkKeys

.destroyInvader:
     mov ax, word [firePositionX]
     add ax, FIRE_POS_ON_SHIP
     sub ax, word [invadersPositionX]
     mov bl, DIST_INVADERS
     div bl
     cmp ah, INVADER_SIZE
     jae .checkKeys
     xor ah, ah
     btr word [invadersStatus], ax

.checkKeys:
    ; check if a key is pressed ?
    mov ah, 11h     ; check keyboard buffer ?
    int 16h         ; bios call
    jz .main_loop    ; no key pressed, restart loop
    ; a key is pressed, extract it
    mov ah,10h       ; one key pressed, extract it
    int 16h          ; bios call
    cmp ah, KEY_LEFT_ARROW   ; is left arrow pressed ?
    jne .isItRightArrow
    sub word [shipPosition], 10
    jmp .main_loop
.isItRightArrow:
    cmp ah, KEY_RIGHT_ARROW  ; is right arrow pressed ?
    jne .isItSpace
    add word [shipPosition], 10
    jmp .main_loop
.isItSpace:
    cmp ah, KEY_SPACE       ; is space bar pressed ?
    jne .isItEsc
    mov ax, word [shipPosition]
    mov word [firePositionX], ax
    mov word [firePositionY], SHIP_Y_POS
    jmp .main_loop
.isItEsc:
    cmp ah, KEY_ESC  ; is esc pressed ?
    jne .main_loop   ; yes => end main loop

end_intro:

    initTextMode
    ; end intro
    ret

;*******************************************************************************
;*******************************************************************************
;********* data *********
data:
;loopTime:       dw 0                 ; the 
shipData:       dw endShipData       ; address of the end of data
                dw 0000000100000000b ; line 01
                dw 0000001110000000b ; line 02
                dw 0111111111111100b ; line 03
                dw 1111111111111110b ; line 04
                dw 1111111111111110b ; line 05
                dw 1111111111111110b ; line 06
                dw 1111111111111110b ; line 07
endShipData:
shipPosition:   dw INITIAL_SHIP_X_POS

invader01Data:  dw endInvader01Data  ; address of the end of data
                dw 0010000010000000b ; line 01
                dw 0001000100000000b ; line 02
                dw 0011111110000000b ; line 03
                dw 0110111011000000b ; line 04
                dw 1111111111100000b ; line 05
                dw 1011111110100000b ; line 06
                dw 1010000010100000b ; line 07
                dw 0001101100000000b ; line 08
endInvader01Data:
invadersPositionX:  dw INITIAL_INVA_X_POS
invadersPositionY:  dw INITIAL_INVA_Y_POS

fireData:       dw endFireData       ; address of the end of data
                dw 0000000100000000b ; line 01
                dw 0000000100000000b ; line 02
endFireData:
firePositionX:  dw GFX_SCREEN_WIDTH
firePositionY:  dw 0
invadersStatus: dw 0xFFFF            ; by default all invaders are ok   

basicPalette: db 0,0,0,   63,63,63 ;, 63,0,0, 0,63,0,   0,0,63, 0,63,63, 63,63,0 ; 7 first colors
;              db 63,0,63, 31,0,0,   0,31,0, 0,0,31, 31,31,31, 0,31,31, 31,31,0 ; 7 second colors
;              db 31,31,0, 31,0,31                                              ; 2 last colors
basicPaletteSize:  equ     $ - basicPalette
