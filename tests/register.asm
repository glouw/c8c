;   Register Test
;       Tests all registers opcodes

register:
    DB 0x00
    DB 0x00
    DB 0x7C
    DB 0x66
    DB 0x60
    DB 0x60
    DB 0x60
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x3C
    DB 0x66
    DB 0x7E
    DB 0x60
    DB 0x3C
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x3E
    DB 0x66
    DB 0x66
    DB 0x3E
    DB 0x06
    DB 0x7C
    DB 0x00
    DB 0x18
    DB 0x00
    DB 0x38
    DB 0x18
    DB 0x18
    DB 0x3C
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x3E
    DB 0x60
    DB 0x3C
    DB 0x06
    DB 0x7C
    DB 0x00
    DB 0x00
    DB 0x18
    DB 0x7E
    DB 0x18
    DB 0x18
    DB 0x18
    DB 0x0E
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x3C
    DB 0x66
    DB 0x7E
    DB 0x60
    DB 0x3C
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x7C
    DB 0x66
    DB 0x60
    DB 0x60
    DB 0x60
    DB 0x00

okay:
    DB 0x3C
    DB 0x30
    DB 0x30
    DB 0x30
    DB 0x30
    DB 0x30
    DB 0x3C
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x3C
    DB 0x66
    DB 0x66
    DB 0x66
    DB 0x3C
    DB 0x00
    DB 0x00
    DB 0x60
    DB 0x60
    DB 0x6C
    DB 0x78
    DB 0x6C
    DB 0x66
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x3C
    DB 0x06
    DB 0x3E
    DB 0x66
    DB 0x3E
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x66
    DB 0x66
    DB 0x66
    DB 0x3E
    DB 0x0C
    DB 0x78
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x3C
    DB 0x0C
    DB 0x0C
    DB 0x0C
    DB 0x0C
    DB 0x0C
    DB 0x3C
    DB 0x00

fail:
    DB 0x3C
    DB 0x30
    DB 0x30
    DB 0x30
    DB 0x30
    DB 0x30
    DB 0x3C
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x0E
    DB 0x18
    DB 0x3E
    DB 0x18
    DB 0x18
    DB 0x18
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x3C
    DB 0x06
    DB 0x3E
    DB 0x66
    DB 0x3E
    DB 0x00
    DB 0x00
    DB 0x18
    DB 0x00
    DB 0x38
    DB 0x18
    DB 0x18
    DB 0x3C
    DB 0x00
    DB 0x00
    DB 0x38
    DB 0x18
    DB 0x18
    DB 0x18
    DB 0x18
    DB 0x3C
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x3C
    DB 0x0C
    DB 0x0C
    DB 0x0C
    DB 0x0C
    DB 0x0C
    DB 0x3C
    DB 0x00

PRINT:
    ; Requires:
    ;    I : String
    ;   V0 : x
    ;   V1 : y
    ;   V2 : String Length
    ; Uses:
    ;   V3 : Index
    ;   V4 : Character Width
    LD V3, 0x00
    LD V4, 0x08
    WHILE:
        SNE V2, V3
        JP BREAK
        DRW V0, V1, 0x8 ; Character Height
        ADD  I, V4
        ADD V0, V4
        ADD V3, 0x01
        JP WHILE
    BREAK:
        RET

MAIN:
    ; Print test title
    LD  I, register ; String
    LD V0, 0x00     ; x
    LD V1, 0x00     ; y
    LD V2, 0x08     ; String length
    CALL   PRINT
    ;   notes:
    ;       VE indicates test failure
    LD   VE, 0x00
    ; LD Vx, byte
    LD   V0, 0x11
    SE   V0, 0x11
    LD   VE, 0x01
    ; LD Vx, Vy
    LD   V1, 0x12
    LD   V0, V1
    SE   V0, 0x12
    LD   VE, 0x01
    ; ADD Vx, byte
    LD   V0, 0x13
    ADD  V0, 0x01
    SE   V0, 0x14
    LD   VE, 0x01
    ; ADD Vx, Vy without carry
    LD   V0, 0x15
    LD   V1, 0x01
    ADD  V0, V1
    SE   V0, 0x16 ; Check addition
    LD   VE, 0x01
    SE   VF, 0x00 ; Check carry does not occur
    LD   VE, 0x01
    ; ADD Vx, Vy with carry
    LD   V0, 0xFF
    LD   V1, 0x01
    ADD  V0, V1
    SE   V0, 0x00 ; Check addition (Overflowed)
    LD   VE, 0x01
    SE   VF, 0x01 ; Check carry does occur
    LD   VE, 0x01
    ; SUB  Vx, Vy without borrow
    LD   V0, 0x15
    LD   V1, 0x01
    SUB  V0, V1
    SE   V0, 0x14 ; Check subtraction
    LD   VE, 0x01
    SE   VF, 0x01 ; Check borrow does not occur
    LD   VE, 0x01
    ; SUB  Vx, Vy with borrow
    LD   V0, 0x00
    LD   V1, 0x01
    SUB  V0, V1
    SE   V0, 0xFF ; Check subtraction
    LD   VE, 0x01
    SE   VF, 0x00 ; Check borrow does occur
    LD   VE, 0x01
    ; SUBN Vx, Vy without borrow
    LD   V0, 0x01
    LD   V1, 0x15
    SUBN V0, V1
    SE   V0, 0x14 ; Check subtraction
    LD   VE, 0x01
    SE   VF, 0x01 ; Check borrow does not occur
    LD   VE, 0x01
    ; SUBN Vx, Vy with borrow
    LD   V0, 0x01
    LD   V1, 0x00
    SUBN V0, V1
    SE   V0, 0xFF ; Check subtraction
    LD   VE, 0x01
    SE   VF, 0x00 ; Check borrow does occur
    LD   VE, 0x01
    ; AND  Vx, Vy
    LD   V0, 0x0F
    LD   V1, 0xFF
    AND  V0, V1
    SE   V0, 0x0F ; Check AND
    LD   VE, 0x01
    ; OR Vx, Vy
    LD   V0, 0x0F
    LD   V1, 0xAA
    OR   V0, V1
    SE   V0, 0xAF ; Check OR
    LD   VE, 0x01
    ; XOR  Vx, Vy
    LD   V0, 0x0F
    LD   V1, 0xAA
    XOR  V0, V1
    SE   V0, 0xA5 ; Check XOR
    LD   VE, 0x01
    ; SHR  Vx, Vy without outshifting
    LD   V0, 0x00
    LD   V1, 0x02
    SHR  V0, V1
    SE   V0, 0x01 ; Check SHR
    LD   VE, 0x01
    SE   VF, 0x00 ; Check shifted is zero in VF
    LD   VE, 0x01
    ; SHR Vx, Vx with outshifting
    SHR  V0, V0
    SE   V0, 0x00 ; Check SHR
    LD   VE, 0x01
    SE   VF, 0x01 ; Check shifted is one in VF
    LD   VE, 0x01
    ; SHL Vx, Vy without outshifting
    LD   V0, 0x00
    LD   V1, 0x40
    SHL  V0, V1
    SE   V0, 0x80 ; Check SHL
    LD   VE, 0x01
    SE   VF, 0x00 ; Check shifted is zero in VF
    LD   VE, 0x01
    ; SHL Vx, Vx with outshifting
    SHL  V0, V0
    SE   V0, 0x00 ; Check SHL
    LD   VE, 0x01
    SE   VF, 0x01 ; Check shifted is one in VF
    LD   VE, 0x01
    ; RND  Vx, Byte
    LD   V0, 0x00
    RND  V0, 0x0F
    LD   VF, K    ; Check with the eyes
    LD   V0, 0x00
    RND  V0, 0xF0
    LD   VF, K    ; Check with the eyes
    LD   V0, 0x00
    RND  V0, 0xFF
    LD   VF, K    ; Check with the eyes
    ; Print test status
    LD    I, fail ; String
    SE   VE, 0x01
    LD    I, okay ; String
    LD   V0, 0x00 ; x
    LD   V1, 0x08 ; y
    LD   V2, 0x08 ; String length
    CALL PRINT
    ; Push any key to exit
    LD VF, K
    END
