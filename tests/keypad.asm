LD_TEST:
    ;----------------------------;
    ;        LD VX, K            ;
    ;----------------------------;
    LD VA, K
LD_TEST_BACK:
    ;----------------------------;
    ;        SKP VX              ;
    ;----------------------------;
    SKP VA
    JP LD_TEST_BACK
LD_TEST_AGAIN:
    ;----------------------------;
    ;        SKNP VX             ;
    ;----------------------------;
    SKNP VA
    JP LD_TEST_OUT
    JP LD_TEST_AGAIN
LD_TEST_OUT:
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
