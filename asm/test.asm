DATA:
    DB 0xAB ;202
    ; comment
; comment

MAIN:
    ADD VE, VF  ; comment 203
    ADD I, VF   ; 205
    ADD V0, 0xFF;207
    AND V0, V1              ; comment 209
    CALL MAIN   ; 20B
    DRW V0, V1, 0xA ;20D
TH: JP MAIN ;20F
    LD DT, VF
    LD ST, VF
    LD F, VF
    LD B, VF
    LD VF, DT
    LD VF, [I]
    LD VF, K
    LD VF, VF
    LD VF, 0xFF
    LD [I], VF
    END
