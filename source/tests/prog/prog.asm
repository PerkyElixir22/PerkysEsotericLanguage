main:
    load bx, 0x2  ; Set BX to 2
    zero VO       ; Set Variable Offset to 0
.loop:
    load AX, text ; Load next character into AX
    andlb AX      ; AND AX with 0x00FF
    test AX       ; Check if AX is NULL
    jnz ..notnull ; Jump if character is not NULL
    exit          ; Exit if character is NULL
..notnull:
    print AX      ; Print character
    add VO, BX    ; Add 2 to Variable Offset
    jmp .loop     ; Jump to start of loop

text: 
; Note : one def8 takes 2 bytes and one def16 takes 4 bytes
    def8 0x48 ; H
    def8 0x65 ; e
    def8 0x6C ; l
    def8 0x6C ; l
    def8 0x6F ; o
    def8 0x2C ; ,
    def8 0x20 ;  
    def8 0x77 ; w
    def8 0x6F ; o
    def8 0x72 ; r
    def8 0x6C ; l
    def8 0x64 ; d
    def8 0x21 ; !
    def8 0x00 ; <null>