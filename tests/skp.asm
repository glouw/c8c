; Description:
;   Tests the "Skip Keyboard Press" (SKP) mnemonic
MAIN: LD  V0, 0x01
      SKP V0
      JP MAIN
      END
