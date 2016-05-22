; Description:
;   Tests the "Shift left" (SHL) mnemonic
MAIN: LD  V0, 0xA0 ; 1010 0000
      SHL V1, V0   ; VF high
      SHL V2, V1   ; VF low
      SHL V3, V2   ; VF high
      SHL V4, V3   ; VF low
      END
