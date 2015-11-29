%define SPRITE_WIDTH    0x0F

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
;--- Function definitions file
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------

%ifdef USE_VSYNC
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: draw a sprite on screen
;-------------------------------------------------------------------------------
waitVSync:
        mov     dx,0x03da
.vblEnd:    
        in      al,dx
        and     al,0x08
        jnz     .vblEnd
.vblRestart:
        in      al,dx
        and     al,0x08
        jz      .vblRestart

        ret
%endif

%ifdef USE_DRAW_MONOCHROM_ICON
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: draw a monochrome icon on screen
; Inputs  : [BP+0x04]   -> icon data address
;           [[BP+0x04]] -> address of the end of data
;           [BP+0x06]   -> icon x position
;           [BP+0x08]   -> icon y position
;           [BP+0x0A]   -> icon color id
;-------------------------------------------------------------------------------
draw_monochrom_icon:
    push bp
    mov  bp, sp
    pusha
.begin:
    mov si, [bp+0x04]                   ; get sprite data address parameter
    mov ax, [bp+0x08]                   ; get the y position parameter
    mov bx, GFX_SCREEN_WIDTH            ; set one line size
    mul bx                              ; use y
    mov dx, [bp+0x06]                   ; get the x position parameter
    add dx, ax                          ; use x, the global icon position to add is in dx
    mov bx, word [si]                   ; init the address of the end of the icon data in bx

    mov al, [bp+0x0A]                   ; set color id
    .loop:
        inc si                          ; move the pointer to the first pixel
        inc si                          ; Two inc is smaller than add
        cmp si, bx                      ; test if all icon lines displayed
        jge .end                        ; if not, display next pixel line

        mov cx, SPRITE_WIDTH            ; an icon line is 16bits
        mov di, dx                      ; init the start of the new line
        .loop_line:
            bt word [si], cx            ; check the bit with index cx
            jnc .next_bit               ; if the bit is not available, try the next one
            mov [es:di], al             ; use that color at position di

        .next_bit:
            inc di                      ; next pixel on line
            loop .loop_line             ; next byte

        add dx, GFX_SCREEN_WIDTH        ; update next line start to draw
        jmp .loop
.end:
    popa
    pop bp
    ret 8                               ; release the 4 params of 2 Bytes from the stack
%endif

%ifdef USE_DRAW_SPRITE
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: draw a sprite on screen
; Inputs  : DS:SI   -> sprite data address
;           BX      -> sprite x position
;           DX      -> sprite y position
;           [DS:SI] -> address of the end of data
;//TODO
; Inputs  : SP+2 -> sprite data address
;           SP+4 -> sprite x position
;           SP+6 -> sprite y position
;           SP+8 -> address of the end of data
;-------------------------------------------------------------------------------
draw_sprite:
    pusha
.begin:
    mov ax, GFX_SCREEN_WIDTH        ; set one line size
    mul dx                          ; use y
    add ax, bx                      ; use x
    mov word [.global_position], ax ; the sprite position to add
    mov bx, [si]                    ; init bx with the address of the end of the sprite data
    inc si                          ; move the pointer to the first pixel
    inc si                          ; Two inc is smaller than add
    .loop:
        cmp si, bx                  ; test if all sprite pixels displayed
        jge .end                    ; if not, display next pixel
        mov ax, GFX_SCREEN_WIDTH    ; set one line size
        mov dl, byte [si+1]         ; init low part of bx with local y byte
        xor dh, dh                  ; set to 0 the high part of bx
        mul dx                      ; use local y
        mov dl, byte [si]           ; init low part of bx with local y byte
        xor dh, dh                  ; set to 0 the high part of bx
        add ax, dx                  ; use local x
        add ax, [.global_position]  ; use the sprite position offset
        mov di, ax                  ; set the offset of the segment
        mov al, byte [si + 2]       ; set color id
        mov [es:di], al             ; use that that color at position di
        add si, 3                   ; move the pointer to the next pixel
        jmp .loop
.end:
    popa
    retn
.global_position: dw 0
%endif

%ifdef USE_CLEARSCR
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: fill the current screen with the background color
;-------------------------------------------------------------------------------
cleanScr:
    xor di,di         ; init di
    xor ax, ax        ; Put the color to load in here
    mov cx, GFX_SCREEN_WIDTH*GFX_SCREEN_HEIGHT/2 ; set increment count for stosd (all the screen)
    cld
    rep stosw         ; CX number of times, move AX to ES:DI while incrementing DI
    ret
%endif

%ifdef USE_PRINTSTR
;-------------------------------------------------------------------------------
;---------------------------------------------------------
; Synopsis: Display a string terminated by 0x0 char
; input: [SP+2] -> string to display
;---------------------------------------------------------
print_str:
    push bp
    mov  bp, sp
    mov si, [bp+4]
    pusha
    .begin:
        lodsb        ; ds:si -> al
        cmp al, 0    ; fin chaine ?
        jz .end
        mov ah, 0x0E ; appel au service 0x0e, int 0x10 du bios
        mov bx, 0x07 ; bx -> attribut, al -> caractere ascii
        int 0x10
        jmp .begin
    .end:
    popa
    pop bp
    ret 2            ; to release 1 parameter 2=>word
%endif

%ifdef USE_PRINTINT
;-------------------------------------------------------------------------------
;---------------------------------------------------------
; Synopsis: Display an int
; input: [SP+2] -> int to display
;---------------------------------------------------------
print_int:
    push bp
    mov  bp, sp
    pusha
    mov ax, [bp+4]
    mov si, 10
    xor cx, cx
    .CDC_rep1:
        xor dx, dx
        div si
        add dl, '0'
        push dx
        inc cx
        cmp ax, 0
        jne .CDC_rep1
    .CDC_until1:
    .CDC_for1:
        pop ax
        mov ah, 0x0E ; appel au service 0x0e, int 0x10 du bios
        mov bx, 0x07 ; bx -> attribut, al -> caractere ascii
        int 0x10
        loop .CDC_for1
    popa
    pop bp
    ret 2            ; to release 1 parameter 2=>word
%endif

%ifdef USE_SWAP_BUFFER
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Synopsis: swap the back buffer to the video memory //TODO check it
;-------------------------------------------------------------------------------
swap_buffer:
    mov ds,ax              ; ds = segment for buffer
    xor si,si              ; si:si = address for buffer
    mov ax,0xA000          ; ax = segment for display memory
    mov di,ax              ; di = segment for display memory
    mov es,ax              ; es:di = address for display memory
    mov cx,320*200/2       ; cx = number of words to copy
    cld                    ; Make sure direction flag is clear
    rep movsw              ; Copy 320*200/2 words from buffer to display memory
    ret
%endif



