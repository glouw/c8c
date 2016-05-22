; Description:
;   Tests the "draw" (DRW) mnemonic
SPRITE: UWORD 0xAAAA
        UWORD 0xAAAA
MAIN:   CLS
        LD  V0, 0x02
        LD  V1, 0x01
        LD   I, SPRITE
        DRW V0, V1, 0x4
        ADD V0, 0x06
        DRW V0, V1, 0x4
        ADD V0, 0x0F
        ADD V1, 0x0F
        DRW V0, V1, 0x4
        LD V0, K
        END
