MAIN:   CLS
        RET
        JP    MAIN
        CALL  MAIN
        SE    V1,   0xFF
        SNE   V1,   0xFF
        SE    V1,   V2
        LD    V1,   0xFF
        ADD   V1,   0xFF
        LD    V1,   V1
        OR    V1,   V2
        AND   V1,   V2
        XOR   V1,   V2
        ADD   V1,   V2
        SUB   V1,   V2
        SHR   V1,   V2
        SUBN  V1,   V2
        SHL   V1,   V2
        SNE   V1,   V2
        LD     I,   MAIN
        JP    V0,   MAIN
        RND   V1,   0xFF
        DRW   V1,   V2, 0xF
        SKP   V1,
        SKNP  V1,
        LD    V1,   DT
        LD    V1,   K
        LD    DT,   V1
        LD    ST,   V1
        ADD    I,   V1
        LD     F,   V1
        LD     B,   V1
        LD   [I],   V1
        LD    V1,  [I]
        END
