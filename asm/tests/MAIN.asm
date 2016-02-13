
    ;
MAIN:

        CLS             ; comment
        RET
        JP    MAIN              ; debug
SUBA:   CALL  MAIN
        SE    V1,   0xFF                ; this here is a comment, see!
        SNE   V1,   0xFF                ; this here is a comment, see!
        SE    V1,   V2                  ; this here is a comment, see!
        LD    V1,   0xFF                ; this here is a comment, see!
        ADD   V1,   0xFF                ; this here is a comment, see!
        LD    V1,   V1                  ; this here is a comment, see!
        OR    V1,   V2                  ; this here is a comment, see!
        AND   V1,   V2                  ; this here is a comment, see!
        XOR   V1,   V2                  ; this here is a comment, see!
        ADD   V1,   V2                  ; this here is a comment, see!
        SUB   V1,   V2                  ; this here is a comment, see!
        SHR   V1,   V2                  ; this here is a comment, see!
        SUBN  V1,   V2                  ; this here is a comment, see!
        SHL   V1,   V2                  ; this here is a comment, see!
        SNE   V1,   V2                  ; this here is a comment, see!
        LD     I,   MAIN                ; this here is a comment, see!
        JP    V0,   MAIN                ; this here is a comment, see!
        RND   V1,   0xFF
        DRW   V1,   V2, 0xF
        SKP   V1,
        SKNP  V1,
        LD    V1,   DT                  ; write something here, ok!
        LD    V1,   K                   ; write something here, ok!
        LD    DT,   V1                  ; write something here, ok!
        LD    ST,   V1                  ; write something here, ok!
        ADD    I,   V1                  ; write something here, ok!
        LD     F,   V1                  ; write something here, ok!
        LD     B,   V1                  ; write something here, ok!
        LD   [I],   V1                  ; write something here, ok!
        LD    V1,  [I]                  ; write something here, ok!
