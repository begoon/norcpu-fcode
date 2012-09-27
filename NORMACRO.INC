; ---------------------------------------------------------------------------
; The main NOR emulator's macros
; Copyright (c) 1996 by Alexander Demin
; ---------------------------------------------------------------------------

; Basic emulator's command - NOR

emNOR macro Src1, Src2, Dst
    dw offset Src1, offset Src2, offset Dst
endm

emNOT macro Src, Dst
    emNOR Src, Src, Dst
endm

emOR macro Src1, Src2, Dst
    emNOR Src1, Src2, emR1
    emNOT emR1, Dst
endm

emMOV macro Src, Dst
    emNOT Src, emR1
    emNOT emR1, Dst
endm

emJMP macro Lbl
    emMOV Lbl, emIP
endm

emJMPi macro Lbl
    local Src
    emJMP Src
    Src dw offset Lbl
endm

emMOVi macro Value, Dst
    local Src, Pass
    emMOV Src, Dst
    emJMPi Pass
    Src dw Value
  Pass:
endm

; [Src] -> Dst

emPeek macro Src, Dst
    local Copy
    emMOV Src, Copy
    emMOV Src, Copy+2
  Copy:
    emMOV Src, Dst
endm

; Src -> [Dst]

emPoke macro Src, Dst
    local Copy
    emMOV Dst, Copy+10
  Copy:
    emMOV Src, Dst
endm

Call86 macro Addr
    emMOVi Addr, Patch
    emMOV emCPU, CPUbytes
    emMOVi InCmdPatch, emCPU
endm

emADCr macro
    Call86 RealAdc
endm

; Compile ADC instruction. Crash Tmp1, Tmp2.
; If Src1 isn't "emAX" then AX is stored in Tmp1
; If Src2 isn't "emCX" then CX is stored in Tmp2

emADC macro Src1, Src2, Dst, Carry
    IfDif <&Src1>, <emAX>
      emMOV emAX, emTmp1
      emMOV Src1, emAX
    Endif
    IfDif <&Src2>, <emCX>
      emMOV emCX, emTmp2
      emMOV Src2, emCX
    Endif
    emADCr
    IfDif <&Src1>, <emAX>
      emMOV emAX, Dst
      emMOV emTmp1, emAX
    Endif
    IfDif <&Src2>, <emCX>
      emMOV emCX, Carry
      emMOV emTmp2, emCX
    Endif
endm

emADD macro Src1, Src2, Dst
    emADC Src1, Src2, Dst, emTmp1
endm

emADDi macro Src1, Src2, Dst
    local Pass, Cnst
    emADD Src1, Cnst, Dst
    emJMPi Pass
    Cnst dw Src2
  Pass:
endm

emPUSH macro Src
    emADD emStack, ConstMinus2, emStack          ; SP-2 -> SP
    emPoke Src, emStack
endm

emPUSHi macro Src
    local Pass, Value
    emPUSH Value
    emJMPi Pass
    Value dw Src
  Pass:
endm

emPOP macro Dst
    emPeek emStack, Dst
    emADD emStack, ConstPlus2, emStack           ; SP+2 -> SP
endm

emCALL macro Lbl
    local RetLabel
    emPUSHi offset RetLabel
    emJMP Lbl
  RetLabel:
endm

emCALLi macro Lbl
    local RetLabel
    emPUSHi RetLabel
    emJMPi Lbl
  RetLabel:
endm

emRet macro
    emPOP emIP
endm

; Compile up to four commands in x86 code

emCmd macro Cmd1, Cmd2, Cmd3, Cmd4
    local Pass, Routine
    CALL86 Routine
    emJMPi Pass
  Routine:
    Cmd1
    Cmd2
    Cmd3
    Cmd4
    retn
  Pass:
endm

emInt21 macro
    emCmd <Int 21h>
endm

emTrap macro
    emCmd <int 3>
endm

emTrap macro
    emCmd <int 3>
endm
