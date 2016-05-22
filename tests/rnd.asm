; Description:
;   Tests the "Random" (RND) mnemonic

MAIN:   RND V0, 0xF0
        RND V1, 0xF0
        RND V2, 0xF0
        RND V3, 0xF0
        RND V4, 0xF0
        RND V5, 0xF0
        RND V6, 0xF0
        RND V7, 0xF0
        RND V8, 0x0F
        RND V9, 0x0F
        RND VA, 0x0F
        RND VB, 0x0F
        RND VC, 0x0F
        RND VD, 0x0F
        RND VE, 0x0F
        RND VF, 0x0F
        END
