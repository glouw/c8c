; Description:
;   Tests the "LOAD" (SNE) mnemonic with register compare

LIST: UWORD 0x0000    ; V0, V1
      UWORD 0x0000    ; V2, V3
      UWORD 0x0000    ; V4, V5
      UWORD 0x0000    ; V6, V7
      UWORD 0x0000    ; V8, V9
      UWORD 0x0000    ; VA. VB
      UWORD 0x0000    ; VC, VD
      UWORD 0x0000    ; VE, VF
MAIN: LD  V0,  K      ; Keyboard input
      LD  V1,  K      ; Keyboard input
      LD  V2,  K      ; Keyboard input
      LD  V3,  K      ; Keyboard input
      LD  V4,  K      ; Keyboard input
      LD  V5,  K      ; Keyboard input
      LD  V6,  K      ; Keyboard input
      LD  V7,  K      ; Keyboard input
      LD  V8,  K      ; Keyboard input
      LD  V9,  K      ; Keyboard input
      LD  VA,  K      ; Keyboard input
      LD  VB,  K      ; Keyboard input
      LD  VC,  K      ; Keyboard input
      LD  VD,  K      ; Keyboard input
      LD  VE,  K      ; Keyboard input
      LD  VF,  K      ; Keyboard input
      LD  V0,  0x0F  ; 0x0F
      LD  V1,  V0    ; 0x0F
      LD  DT,  V1    ; 0x0F
      LD  V2,  DT    ; 0x0E because decrement via cycle
      LD  ST,  V2    ; 0x0E
      LD   F,  V1    ; 0x0F
      LD   I,  LIST  ; 0x202
      ;
      LD   B,  V1    ; 0x202: 0x00, 0x203: 0x01, 0x204: 0x04
      LD  V3, [I]
      ; Populate buffer for mem transfer
      LD  V0, 0x00
      LD  V1, 0x01
      LD  V2, 0x02
      LD  V3, 0x03
      LD  V4, 0x04
      LD  V5, 0x05
      LD  V6, 0x06
      LD  V7, 0x07
      LD  V8, 0x08
      LD  V9, 0x09
      LD  VA, 0x0A
      LD  VB, 0x0B
      LD  VC, 0x0C
      LD  VD, 0x0D
      LD  VE, 0x0E
      LD  VF, 0x0F
      LD   I,  LIST  ; 0x202
      LD  [I], VF    ;
      ; Modify buffer
      ADD V0, 0x01
      ADD V1, 0x01
      ADD V2, 0x01
      ADD V3, 0x01
      ADD V4, 0x01
      ADD V5, 0x01
      ADD V6, 0x01
      ADD V7, 0x01
      ADD V8, 0x01
      ADD V9, 0x01
      ADD VA, 0x01
      ADD VB, 0x01
      ADD VC, 0x01
      ADD VD, 0x01
      ADD VE, 0x01
      ADD VF, 0x01
      ; Read from mem back into buffer
      LD   I,  LIST  ; 0x202
      LD  VF, [I]    ;
      END            ;
