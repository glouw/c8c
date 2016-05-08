; Description:
;   Tests the "Skip if Equal" (SE) mnemonic with register compare
MAIN: LD V3, 0x00
      LD V2, 0x00
      SE V3, V2
      JP MAIN     ; This instruction will be skipped
      SE V3, 0x00
      JP MAIN     ; This instruction will be skipped
      LD V3, 0x00
      LD V2, 0x01
      SE V3, V2
      SE V3, 0x01 ; This instruction will NOT be skipped
      END         ; This instruction will NOT be skipped
      JP MAIN     ; If the above instruction was skipped, then repeat the test
