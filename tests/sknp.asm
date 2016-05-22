; Description:
;   Tests the "Skip Keyboard Not Press" (SKNP) mnemonic
MAIN: LD   V0, 0x01
      SKNP V0
      JP MAIN
      END
