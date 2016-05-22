; Description:
;   Tests the "Addition" (ADD) mnemonic
MAIN: LD  V0, 0x01
      ADD V0, 0x01
      LD  V0, 0xF0
      LD  V1, 0x0E
      LD  V2, 0x01
      ADD V0, V1
      ADD V0, V2 ; Check for carry
      ADD V0, V2
      ADD V0, V2 ; Flag goes low
      LD  I, MAIN
      ADD I, V2
      END
