STACK:
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
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00
    DB 0x00

LD_TEST:
    LD V0, 0x00
    LD V1, 0x01
    LD V2, 0x02
    LD V3, 0x03
    LD V4, 0x04
    LD V5, 0x05
    LD V6, 0x06
    LD V7, 0x07
    LD V8, 0x08
    LD V9, 0x09
    LD VA, 0x0A
    LD VB, 0x0B
    LD VC, 0x0C
    LD VD, 0x0D
    LD VE, 0x0E
    LD VF, 0x0F
    ;----------------------------;
    ;        LD [I], VX          ;
    ;----------------------------;
    LD   I, STACK
    LD [I], VF
    ; Clear them all
    LD V0, 0x00
    LD V1, 0x00
    LD V2, 0x00
    LD V3, 0x00
    LD V4, 0x00
    LD V5, 0x00
    LD V6, 0x00
    LD V7, 0x00
    LD V8, 0x00
    LD V9, 0x00
    LD VA, 0x00
    LD VB, 0x00
    LD VC, 0x00
    LD VD, 0x00
    LD VE, 0x00
    LD VF, 0x00
    ;----------------------------;
    ;        LD VX, [I]          ;
    ;----------------------------;
    ; Load them back
    LD   I, STACK
    LD VF, [I]
    ; Check them all
    ; V0
    SE V0, 0x00
    LD VE, 0x01
    ; V1
    SE V1, 0x01
    LD VE, 0x01
    ; V2
    SE V2, 0x02
    LD VE, 0x01
    ; V3
    SE V3, 0x03
    LD VE, 0x01
    ; V4
    SE V4, 0x04
    LD VE, 0x01
    ; V5
    SE V5, 0x05
    LD VE, 0x01
    ; V6
    SE V6, 0x06
    LD VE, 0x01
    ; V7
    SE V7, 0x07
    LD VE, 0x01
    ; V8
    SE V8, 0x08
    LD VE, 0x01
    ; V9
    SE V9, 0x09
    LD VE, 0x01
    ; VA
    SE VA, 0x0A
    LD VE, 0x01
    ; VB
    SE VB, 0x0B
    LD VE, 0x01
    ; VC
    SE VC, 0x0C
    LD VE, 0x01
    ; VD
    SE VD, 0x0D
    LD VE, 0x01
    ; VE
    SE VE, 0x0E
    LD VE, 0x01
    ; VF
    SE VF, 0x0F
    LD VE, 0x01
    ; Done -- Need to reset VE back to 0x0 since this passed
    LD VE, 0x00
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
