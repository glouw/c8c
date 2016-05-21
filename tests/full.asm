; Description:
;   Tests all mnemonics with crazy syntaxing

VARIABLE: UWORD 0x1000
ARRAY:  UWORD 0x1000
          UWORD 0x0001
MAIN:   ;notice how tabs don't really matter
CLS
RET
JP MAIN NOW
 CALL  MAIN
  SE     V1,   0xFF    ; random comment
        SNE   V1,   0xFF        ; another random comment (:)
        SE    V1,   V2
        LD    V1,       0xFF ;  some odd indentation
        ADD   V1,   0xFF
        LD    V1,   V1
        OR    V1     V2     ; I removed the comma here
        AND   V1     V2     ; and here
        XOR   V1      V2    ; and here
        ADD     V1,    V2
        SUB    V1,   V2
        SHR   V1,   V2
        SUBN  V1,   V2
        SHL   V1,   V2
        SNE   V1,   V2
        LD     I,       MAIN    ; some more odd indentation
            JP    V0,   MAIN
            RND   V1,   0xFF
            DRW    V1,   V2, 0xF
            SKP     V1,
            SKNP  V1,
            LD        V1,   DT
            LD    V1,   K
            LD    DT,   V1
            LD    ST,   V1
            ADD    I,   V1
            LD     F,   V1
            LD     B,   V1
            LD   [I],   V1
            LD    V1,  [I]
            END                 ; stress test complete!
