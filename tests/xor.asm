; Description:
;   Tests the "Exclusive OR" (XOR) mnemonic
MAIN: LD  V0, 0x99 ; 10011001 -> 01100110 (0x66)
      LD  V1, 0xFF
      XOR V0, V1
      END
