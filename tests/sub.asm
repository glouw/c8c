; Description:
;   Tests the "Subtraction" (SUB) mnemonic
MAIN: LD  V0, 0x02
      LD  V1, 0x01
      SUB V0, V1 ; VF high
      SUB V0, V1 ; VF high
      SUB V0, V1 ; VF low!
      SUB V0, V1 ; VF high
      END
