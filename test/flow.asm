AFTER_RESET:
    ;----------------------------;
    ;        JP V0, addr         ;
    ;----------------------------;
    LD V0, 0x02
    ; Jumps to the return of JP_TEST
    JP V0, JP_TEST

JP_TEST:
    ;----------------------------;
    ;        JP addr             ;
    ;----------------------------;
    JP AFTER_RESET
    ;----------------------------;
    ;        JP V0, addr         ;
    ;----------------------------;
    RET

main:
    ; VE, if non-zero, indicates test failure
    LD VE, 0x00
    ; Test all
    CALL JP_TEST
    ; Finish - Display VE: the test failure status
    LD   F, VE
    LD  V0, 0x01
    LD  V1, 0x01
    DRW V0, V1, 0x5
    ; All is well; stay here forever
END:
    JP END
