LD_TEST:
    ;----------------------------;
    ;        LD DT, VX           ;
    ;----------------------------;
    LD V0, 0xFF
    LD DT, V0   ; 0xFF
    ;----------------------------;
    ;        LD VX, DT           ;
    ;----------------------------;
    LD V1, DT   ; 0xFE
    SE V1, 0xFE ; 0xFE
    ; Flag
    LD VE, 0x01
    ;----------------------------;
    ;        LD ST, VX           ;
    ;----------------------------;
    LD V2, 0xFF
    LD ST, V2 ; How to test this?
    RET

MAIN:
    ; VE, if non-zero, indicates test failure
    LD VE, 0x00
    ; Test all
    CALL LD_TEST
    ; Finish - Display VE: the test failure status
    LD   F, VE
    LD  V0, 0x01
    LD  V1, 0x01
    DRW V0, V1, 0x5
    ; All is well; stay here forever
MAIN_DONE:
    JP MAIN_DONE
