;-------------------------------------------;
;                static data                ;
;-------------------------------------------;
MAIN.data:   UINT    0x1424                 ;
;-------------------------------------------;
;                global data                ;
;-------------------------------------------;
data:        UINT    0x2000                 ;
;-------------------------------------------;
;              program space                ;
;-------------------------------------------;
SUBB:        SE      V3,     0xFE           ; HI, SARAH!
SUBA:        SUBN    V3,     V4             ;
             SHL     V3,     V4             ;
             SNE     V4,     V4             ;
             LD       I,     SUBA           ;
             JP      V0,     MAIN           ; another comment about jumping
             RND     V3,     0xFF           ;
             DRW     V3,     V4, 0xF        ; draw to screen
             SKP     V3                     ;
             SKNP    V3                     ;
             LD      V3,     DT             ;
             LD      V3,     K              ;
             LD      DT,     V3             ;
             LD      ST,     V3             ;
             ADD      I,     V3             ;
             LD       F,     V3             ;
             LD       B,     V3             ;
             LD     [I],     V0             ;
             LD      V0,    [I]             ;
MAIN:        CLS                            ;
             RET                            ;
             JP      SUBA                   ;
             CALL    MAIN                   ;
             SE      V3,     0xFF           ;
             SNE     V3,     0xFF           ;
             SE      V3,     V4             ; comment
             LD      V3,     0xFF           ; comment
             ADD     V3,     0xFF           ; comment
             LD      V3,     V4             ;
             OR      V3,     V4             ;
             AND     V3,     V4             ;
             XOR     V3,     V4             ;
             ADD     V3,     V4             ;
             SUB     V3,     V4             ;
             SHR     V3,     V4             ;
