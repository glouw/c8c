SE_TEST:
    ;----------------------------;
    ;        SE VX, byte         ;
    ;----------------------------;
    LD V0, 0xFF
    ; Skip the raising the failure if equal
    SE V0, 0xFF
    ; Failure flag
    LD VE, 0x01
    ;----------------------------;
    ;        SE VX, VY           ;
    ;----------------------------;
    LD V0, 0xEE
    LD V1, 0xEE
    ; Skip the raising the failure if equal
    SE V0, V1
    ; Failure flag
    LD VE, 0x02
    RET

SNE_TEST:
    ;----------------------------;
    ;        SNE VX, byte        ;
    ;----------------------------;
    LD V0, 0xDD
    ; Skip the raising failure if not equal
    SNE V0, 0xCC
    ; Failure flag
    LD VE, 0x03
    ;----------------------------;
    ;        SNE VX, VY          ;
    ;----------------------------;
    LD V0, 0xAA
    LD V1, 0xBB
    ; Skip the raising failure if not equal
    SNE V0, 0xEE
    ; Failure flag
    LD VE, 0x04
    RET

MAIN:
    ; VE, if non-zero, indicates test failure
    LD VE, 0x00
    ; Test all
    CALL SE_TEST
    CALL SNE_TEST
    ; Finish - Display VE: the test failure status
    LD   F, VE
    LD  V0, 0x01
    LD  V1, 0x01
    DRW V0, V1, 0x5
    ; All is well; stay here forever
MAIN_DONE:
    JP MAIN_DONE
