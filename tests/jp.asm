; Description:
;   Tests the "jump to label" (JP) mnemonic

MAIN:   LD V0, 0x04
        JP THERE
        END
THERE:  JP V0, MAIN
