#bits 8

#subruledef register
{
    R0 => 0x3C`8
    R1 => 0x3E`8
    R2 => 0x7E`8
    R3 => 0x5E`8
    R4 => 0x3F`8
    R5 => 0x40`8
    R6 => 0x3D`8
    R7 => 0x7C`8

    AX => 0x3C`8
    BX => 0x3E`8
    CX => 0x7E`8
    DX => 0x5E`8

    JO => 0x3D`8
    VO => 0x7C`8
}

#subruledef rel8
{
    {reladr:s64} => {
        reladr = reladr - $
        assert(reladr < 0x80)
        assert(reladr >= -0x80)
        (0x20+reladr[7:4])`8 @ (0x20+reladr[3:0])`8
    }
}

#ruledef instructions
{
    ADD {reg:register}, {reg2:register}  => 0x20`8 @ reg @ reg2
    SUB {reg:register}, {reg2:register}  => 0x21`8 @ reg @ reg2
    TEST {reg:register}                  => 0x22`8 @ reg
    JNZ {rel:rel8}                       => 0x23`8 @ rel
    LOAD {reg:register}, {rel:rel8}      => 0x24`8 @ reg @ rel
    STORE {rel:rel8}, {reg:register}     => 0x25`8 @ rel @ reg
    INC {reg:register}                   => 0x26`8 @ reg
    DEC {reg:register}                   => 0x27`8 @ reg
    PRINT {reg:register}                 => 0x28`8 @ reg
    READ {reg:register}                  => 0x29`8 @ reg
    JMP {rel:rel8}                       => 0x2A`8 @ rel
    COPY {reg:register}, {reg2:register} => 0x2B`8 @ reg @ reg2
    ZERO {reg:register}                  => 0x2C`8 @ reg
    ANDLB {reg:register}                 => 0x2D`8 @ reg
    LOAD {reg:register}, {imm:u4}        => 0x2E`8 @ reg @ (0x20 + imm)`8
    EXIT                                 => 0x2F`8
}

#ruledef prepInstructions {
    def8 {val:i8} => (0x20+val[7:4])`8 @ (0x20+val[3:0])`8

    def16 {val:i16} => (0x20+val[7:4])`8 @ (0x20+val[3:0])`8 @ (0x20+val[15:12])`8 @ (0x20+val[11:8])`8
}

#bankdef program {
    #addr 0
    #bits 8
    #outp 0
}
#bank program