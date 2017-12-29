SUBROUTINE:
    ; Tests RET as well
    RET

CALL_TEST:
    ;----------------------------;
    ;        CALL addr           ;
    ;----------------------------;
    CALL SUBROUTINE
    RET

main:
    ; VE, if non-zero, indicates test failure
    LD VE, 0x00
    ; Test all
    CALL CALL_TEST
    ; Finish - Display VE: the test failure status
    LD   F, VE
    LD  V0, 0x01
    LD  V1, 0x01
    DRW V0, V1, 0x5
    ; All is well; stay here forever
END:
    JP END
