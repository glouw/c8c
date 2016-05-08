; Description:
;   Tests the "Skip if Not Equal" (SNE) mnemonic with register compare

MAIN: LD  V3, 0x00
      LD  V2, 0x01
      SNE V3, V2
      JP  MAIN     ; This instruction will be skipped
      SNE V3, 0x01
      JP  MAIN     ; This instruction will be skipped
      LD  V3, 0x00
      LD  V2, 0x00
      SNE V3, V2
      SNE V3, 0x00 ; This instruction will NOT be skipped
      END          ; This instruction will NOT be skipped
      JP  MAIN     ; If the above instruction was skipped, then repeat the test
