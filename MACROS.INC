; ---------------------------------------------------------------------------
; The main macros for F-Code w/using the NOR emulator
; Version 2.0
; Copyright (c) 1996 by Alexander Demin
; ---------------------------------------------------------------------------

; Call user's subroutine

ofCUS           macro   Addr:REQ
                dw      Addr
                endm

; Compile the jump into Next-entry

next            macro
                emJMPi  NEXT$
                endm

Callr           macro
                emCALLi CALLR$
                endm

RETR            MACRO   Value
                ErrIfNb <&Value> "Invalid argument"
                dw      RETR$
                ENDM

OneArg          MACRO   ID:REQ, isOf
os&ID           MACRO   Value
                ErrIfNB <&Value> "Non-empty argument in &ID"
                dw      os&ID&$
                ENDM
                IfIdn   <&isOf>, <of>
of&ID             MACRO  Value:REQ
                    ofPush Value
                    os&ID
                  ENDM
                EndIf
                ENDM

TwoArg          MACRO   ID:REQ, isOf
os&ID           MACRO   Value
                ErrIfNB <&Value> "Non-empty argument in &ID"
                dw      os&ID&$
                ENDM
                IfIdn   <&isOf>, <of>
of&ID             MACRO  First:REQ, Second:REQ
                    IfDif <&First>, <*>
                      ofPush First
                      ifDif <&Second>, <*>
                        ofPush Second
                      Else
                        osSwap
                      Endif
                    Else
                      IfDif <&Second>, <*>
                        ofPush Second
                      EndIf
                    EndIf
                    os&ID
                  ENDM
                EndIf
                ENDM

; ---------------------------------------------------------------------------
; Push the word to the data stack

ofPush          MACRO   AWord:REQ
                dw      osPush$
                dw      AWord
                ENDM

; ---------------------------------------------------------------------------
; Create the calling entries for standart procedures
; If 'of' is present, then create of-call macros, otherwise create only
; os-call macros.

                OneArg   ToAX,  of
                OneArg   ToDX,  of
                OneArg   Pick,  of
                OneArg   Dup
                OneArg   2Dup
                OneArg   Swap
                OneArg   Over
                OneArg   Drop
                OneArg   Adc,   of
                OneArg   Add,   of
                OneArg   Nor,   of
                OneArg   Xor,   of
                OneArg   And,   of
                OneArg   Or,    of
                OneArg   Not
                OneArg   Neg
                OneArg   Dos
                OneArg   Put,   of
                OneArg   Get
                OneArg   Sub,   of
                OneArg   Inc
                OneArg   Is0
                OneArg   Trap
                OneArg   Peek,  of
                OneArg   Peekb, of
                OneArg   I
                OneArg   Char,  of
                OneArg   Str,   of
                OneArg   Int21
                OneArg   Int29
                OneArg   Shl,   of      ; Value -> Value*2 isCarry
                OneArg   Rcl,   of      ; Value isCarry -> Value*2 isCarry
                OneArg   Rot
                OneArg   Exch
                OneArg   Cmp
                TwoArg   Do,    of
                TwoArg   Poke,  of
                TwoArg   Pokeb, of

osIP            macro
                ofPeek  IP$
                endm

osDP            macro
                ofPeek  emStack
                endm

osRP            macro
                ofPeek  RP$
                endm

osJmp           MACRO   Value           ; Addr ->
                ErrIfNb <&Value> "Invalid argument"
                ofPush  0001h           ; Addr Cond=1
                osSwap                  ; Cond=1 Addr
                osJnz
                ENDM

ofJmp           MACRO   Addr:REQ
                ofPush  Addr
                osJmp
                ENDM

osJnz           MACRO   Value           ; Cond Addr ->
                ErrIfNb <&Value> "Invalid argument"
                dw      osJnz$
                ENDM

ofJnz           MACRO   Addr:REQ        ; Cond ->
                ofPush  Addr
                osJnz
                ENDM

osJz            MACRO   Value           ; Cond Addr ->
                ErrIfNb <&Value> "Invalid argument"
                osSwap                  ; Addr Cond
                osIs0                   ; Addr Norm
                osNot                   ; Addr !Norm
                osSwap                  ; !Norm Addr
                osJnz                   ; ->
                ENDM

ofJz            MACRO   Addr:REQ        ; Cond ->
                ofPush  Addr            ; Cond Addr
                osJz                    ; ->
                ENDM

osStep          MACRO   Value           ;
                ErrIfNb <&Value> "Invalid argument"
                dw      osStep$
                osSwap
                osJnz
                osGet
                osDrop
                osGet
                osDrop
                ENDM

ofStep          MACRO   Addr:REQ, Delta:REQ
                ofPush  Delta
                dw      osStep$
                ofJnz   Addr
                osGet
                osDrop
                osGet
                osDrop
                ENDM

osLoop          MACRO   Value           ;
                ErrIfNb <&Value> "Invalid argument"
                ofPush  1
                osStep
                ENDM

ofLoop          MACRO   Addr:REQ
                ofStep  Addr, 1
                ENDM

; ofJe ( A B -> ) Jump if A and B are equal

ofJe            MACRO   Addr:REQ        ; a b ->
                osSub                   ; is0?
                ofJz    Addr            ; ->
                ENDM

ofJne           MACRO   Addr:REQ        ; a b ->
                osSub                   ; is0?
                ofJnz   Addr            ; ->
                ENDM

ofCond          MACRO   Addr:REQ        ; isZero isCarry ->
                osNot                   ; isZero !isCarry
                osAnd                   ; is0?
                ofJnz   Addr            ; ->
                ENDM

ofJge           MACRO   Addr:REQ        ; a b ->
                osCmp                   ; isCarry isZero
                osSwap                  ; isZero isCarry
                ofCond  Addr            ; ->
                ENDM

ofJlt           MACRO   Addr:REQ        ; a b ->
                osCmp                   ; isCarry isZero
                ofCond  Addr            ; ->
                ENDM

ofJgt           MACRO   Addr:REQ        ; a b ->
                osInc                   ; a b+1 ->
                ofJge   Addr            ; ->
                ENDM

ofJle           MACRO   Addr:REQ        ; a b ->
                osInc                   ; a b+1
                ofJlt   Addr            ; ->
                ENDM

; Compile up to four commands together

os              MACRO   Cmd1, Cmd2, Cmd3, Cmd4
                Cmd1
                Cmd2
                Cmd3
                Cmd4
                ENDM
