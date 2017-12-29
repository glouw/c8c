LD_TEST:
    ;----------------------------;
    ;        LD Vx, byte         ;
    ;----------------------------;
    LD V0, 0x11
    ; Check load
    SE V0, 0x11 ; Skip next instruction if equal
    LD VE, 0x01
    ;----------------------------;
    ;         LD Vx, Vy          ;
    ;----------------------------;
    LD V1, 0x12
    LD V0, V1
    ; Check load
    SE V0, 0x12
    LD VE, 0x01
    RET

; VF goes high with overflows
ADD_TEST:
    ;----------------------------;
    ;       ADD Vx, byte         ;
    ;----------------------------;
    LD  V0, 0x13
    ADD V0, 0x01
    SE  V0, 0x14
    LD  VE, 0x01
    ;----------------------------;
    ;  ADD Vx, Vy without carry  ;
    ;----------------------------;
    LD  V0, 0x15
    LD  V1, 0x01
    ADD V0, V1
    ; Check addition
    SE V0, 0x16
    LD VE, 0x01
    ; Check carry does not occur
    SE VF, 0x00
    LD VE, 0x01
    ;----------------------------;
    ;    ADD Vx, Vy with carry   ;
    ;----------------------------;
    LD  V0, 0xFF
    LD  V1, 0x01
    ADD V0, V1
    ; Check addition (Overflowed occured)
    SE  V0, 0x00
    LD  VE, 0x01
    ; Check carry does occur
    SE  VF, 0x01
    LD  VE, 0x01
    RET

; VF goes low with borrows
SUB_TEST:
    ;----------------------------;
    ; SUB  Vx, Vy without borrow ;
    ;----------------------------;
    LD  V0, 0x15
    LD  V1, 0x01
    SUB V0, V1
    ; Check subtraction
    SE  V0, 0x14
    LD  VE, 0x01
    ; Check borrow does not occur
    SE  VF, 0x01
    LD  VE, 0x01
    ;----------------------------;
    ;   SUB Vx, Vy with borrow   ;
    ;----------------------------;
    LD  V0, 0x00
    LD  V1, 0x01
    SUB V0, V1
    ; Check subtraction
    SE  V0, 0xFF
    LD  VE, 0x01
    ; Check borrow does occur
    SE  VF, 0x00
    LD  VE, 0x01
    ;----------------------------;
    ; SUBN Vx, Vy without borrow ;
    ;----------------------------;
    LD   V0, 0x01
    LD   V1, 0x15
    SUBN V0, V1
    ; Check subtraction
    SE   V0, 0x14
    LD   VE, 0x01
    ; Check borrow does not occur
    SE   VF, 0x01
    LD   VE, 0x01
    ;----------------------------;
    ;   SUBN Vx, Vy with borrow  ;
    ;----------------------------;
    LD   V0, 0x01
    LD   V1, 0x00
    SUBN V0, V1
    ; Check subtraction
    SE   V0, 0xFF
    LD   VE, 0x01
    ; Check borrow does occur
    SE   VF, 0x00
    LD   VE, 0x01
    RET

AND_TEST:
    ;----------------------------;
    ;        AND  Vx, Vy         ;
    ;----------------------------;
    LD  V0, 0xFF
    LD  V1, 0x0F
    AND V0, V1
    ; Check AND
    SE  V0, 0x0F
    LD  VE, 0x01
    RET

OR_TEST:
    ;----------------------------;
    ;         OR Vx, Vy          ;
    ;----------------------------;
    LD  V0, 0xAA
    LD  V1, 0x0F
    OR  V0, V1
    ; Check OR
    SE  V0, 0xAF
    LD  VE, 0x01
    RET

XOR_TEST:
    ;----------------------------;
    ;         XOR Vx, Vy         ;
    ;----------------------------;
    LD  V0, 0xAA
    LD  V1, 0x0F
    XOR V0, V1
    ; Check XOR
    SE  V0, 0xA5
    LD  VE, 0x01
    RET

SHR_TEST:
    ;----------------------------;
    ; SHR Vx, Vy w/o outshifting ;
    ;----------------------------;
    LD  V0, 0x00
    LD  V1, 0x02
    SHR V0, V1
    ; Check SHR
    SE  V0, 0x01
    LD  VE, 0x01
    ; Check shifted is zero in VF
    SE  VF, 0x00
    LD  VE, 0x01
    ;----------------------------;
    ; SHR Vx, Vx w/ outshifting  ;
    ;----------------------------;
    SHR V0, V0
    ; Check SHR
    SE  V0, 0x00
    LD  VE, 0x01
    ; Check shifted is one in VF
    SE  VF, 0x01
    LD  VE, 0x01
    RET

SHL_TEST:
    ;----------------------------;
    ; SHL Vx, Vy w/o outshifting ;
    ;----------------------------;
    LD  V0, 0x00
    LD  V1, 0x40
    SHL V0, V1
    ; Check SHL
    SE  V0, 0x80
    LD  VE, 0x01
    ; Check shifted is zero in VF
    SE  VF, 0x00
    LD  VE, 0x01
    ;----------------------------;
    ; SHL Vx, Vx w/ outshifting  ;
    ;----------------------------;
    SHL V0, V0
    ; Check SHL
    SE  V0, 0x00
    LD  VE, 0x01
    ; Check shifted is one in VF
    SE  VF, 0x01
    LD  VE, 0x01
    RET

RND_TEST:
    ;----------------------------;
    ;        RND Vx, Byte        ;
    ;----------------------------;
    LD  V0, 0x00
    RND V0, 0x0F
    ; Manual check - Push any key to continue
    LD  VF, K
    LD  V0, 0x00
    RND V0, 0xF0
    ; Manual check - Push any key to continue
    LD  VF, K
    LD  V0, 0x00
    RND V0, 0xFF
    ; Manual check - Push any key to continue
    LD  VF, K
    RET

main:
    ; VE, if non-zero, indicates test failure
    LD VE, 0x00
    ; Test all
    CALL LD_TEST
    CALL ADD_TEST
    CALL SUB_TEST
    CALL AND_TEST
    CALL OR_TEST
    CALL XOR_TEST
    CALL SHR_TEST
    CALL SHL_TEST
    CALL RND_TEST
    ; Finish - Display VE: the test failure status
    LD   F, VE
    LD  V0, 0x01
    LD  V1, 0x01
    DRW V0, V1, 0x5
    ; All is well; stay here forever
END:
    JP END
