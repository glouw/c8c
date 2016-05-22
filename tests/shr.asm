; Description:
;   Tests the "Shift Right" (shr) mnemonic
MAIN: LD  V0, 0x05 ; 0000 0101
      SHR V1, V0   ; VF high
      SHR V2, V1   ; VF low
      SHR V3, V2   ; VF high
      SHR V4, V3   ; VF low
      END
