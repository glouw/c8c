; Counts up with 0x0F mask
SPRITE_0:
    DB 0x01
    DB 0x02
    DB 0x03
    DB 0x04
    DB 0x05

; Counts down with 0xF0 mask
SPRITE_1:
    DB 0x50
    DB 0x40
    DB 0x30
    DB 0x20
    DB 0x10

; Storage for binary coded decimal numbers
BCD:
    DB 0x00
    DB 0x00
    DB 0x00

; Binary Coded Decimal drawing test
DRW_BCD_TEST:
    CLS
    LD  I, BCD
    LD V3, 0xFF
    ;----------------------------;
    ;        LD B, VX            ;
    ;----------------------------;
    ; Loads BCD of VX into I, I+1, I+2
    LD   B, V3
    ; Populate registers
    LD  V2, [I]
    ; Prepare for drawing
    LD  V3, 0x01
    LD  V4, 0x01
    ; Sprite width
    LD  V5, 0x08
    ; Draw first
    LD   F, V0
    DRW V3, V4, 0x5
    ; Draw second
    LD   F, V1
    ADD V3, V5
    DRW V3, V4, 0x5
    ; Draw third
    LD   F, V2
    ADD V3, V5
    DRW V3, V4, 0x5
    ; Wait
    LD VA, K
    ; Done
    RET

; Display and wait for each
DRW_FONT_TEST:
    LD  V3, 0x00
WHILE:
    CLS
    LD   F, V3
    LD  V0, 0x01
    LD  V1, 0x01
    DRW V0, V1, 0x5
    ; Wait here
    LD  VD, K
    ; Loop
    ADD V3, 0x01
    SE  V3, 0x10
    JP WHILE
    ; Done
    RET

DRW_XOR_TEST:
    ;----------------------------;
    ;        LD I, addr          ;
    ;----------------------------;
    CLS
    LD   I, SPRITE_0
    LD  V0, 0x01
    LD  V1, 0x01
    ; Draw and wait
    DRW V0, V1, 0x5
    LD  VA, K
    ; Draw and wait
    DRW V0, V1, 0x5
    LD  VA, K
    ; Check flag is high for XOR
    SE  VF, 0x01
    LD  VE, 0x01
    RET

DRW_TEST:
    ;----------------------------;
    ;        CLS                 ;
    ;----------------------------;
    CLS
    ;----------------------------;
    ;        LD I, addr          ;
    ;----------------------------;
    LD   I, SPRITE_0
    LD  V0, 0x01
    LD  V1, 0x01
    DRW V0, V1, 0x5
    ; Check flag is low for XOR
    SE  VF, 0x00
    LD  VE, 0x01
    ; Wait here
    LD  VA, K
    ;----------------------------;
    ;        ADD I, VX           ;
    ;----------------------------;
    CLS
    LD  VB, 0x05
    ADD  I, VB
    LD  V0, 0x01
    LD  V1, 0x01
    DRW V0, V1, 0x5
    ; Check flag is low for XOR
    SE  VF, 0x00
    LD  VE, 0x01
    ; Wait here
    LD VA, K
    ; Done
    RET

MAIN:
    ; VE, if non-zero, indicates test failure
    LD VE, 0x00
    ; Test all
    CALL DRW_TEST
    CALL DRW_XOR_TEST
    CALL DRW_FONT_TEST
    CALL DRW_BCD_TEST
    ; Finish - Display VE: the test failure status
    CLS
    LD   F, VE
    LD  V0, 0x01
    LD  V1, 0x01
    DRW V0, V1, 0x5
    ; All is well; stay here forever
END:
    JP END
