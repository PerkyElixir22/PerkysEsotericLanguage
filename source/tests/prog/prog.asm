main:
    load bx, 0x2
    zero VO
.loop:
    load AX, text
    andlb AX
    test AX
    jnz ..notnull
    exit
..notnull:
    print AX
    add VO, BX
    jmp .loop

text:
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