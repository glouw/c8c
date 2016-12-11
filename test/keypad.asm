LD_TEST:
    ;----------------------------;
    ;        LD VX, K            ;
    ;----------------------------;
    LD VA, K
BACK:
    ;----------------------------;
    ;        SKP VX              ;
    ;----------------------------;
    SKP VA
    JP BACK
AGAIN:
    ;----------------------------;
    ;        SKNP VX             ;
    ;----------------------------;
    SKNP VA
    JP OUT
    JP AGAIN
OUT:
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
END:
    JP END
