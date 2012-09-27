
osPick$:        CALLR                   ; -> Index
                osDup                   ; Index Index
                osAdd                   ; Index*2
                osDP                    ; Index*2 SP
                osAdd                   ; Index*2+SP
                ofAdd   2               ; Index*2+SP+2
                osPeek                  ; Value ->
                RETR

osDup$:         CALLR                   ;
                osDP
                osPeek
                RETR

os2Dup$:        CALLR
                osOver
                osOver
                RETR

osOver$:        CALLR
                ofPick  1
                RETR

osAdd$:         CALLR
                osAdc
                osDrop
                RETR

osInc$:         CALLR
                ofPush  1
                osAdd
                RETR

osXor$:         CALLR                   ; a b
                os2Dup                  ; a b a b
                osNot                   ; a b a !b
                osAnd                   ; a b a&!b
                osSwap                  ; a a&!b b
                ofPick  2               ; a a&!b b a
                osNot                   ; a a&!b b !a
                osAnd                   ; a a&!b b&!a
                osOr                    ; a&!b|b&!a
                osSwap                  ; a&!b|b&!a a
                osDrop                  ; a&!b|b&!a
                RETR

osNot$:         CALLR
                osDup
                osNor
                RETR

osAnd$:         CALLR                   ; -> a b
                osNot                   ; a !b
                osSwap                  ; !b a
                osNot                   ; !b !a
                osOr                    ; !b|!a
                osNot                   ; ->
                RETR

osOr$:          CALLR
                osNor
                osNot
                RETR

osNeg$:         CALLR
                osNot
                osInc
                RETR

osDos$:         CALLR
                ofToAX  4C00h
                osInt21
                RETR

osSub$:         CALLR
                osNeg
                osAdd
                RETR

; osIs0? ( value -> isCarry )
;          if value=0 isCarry=0 else isCarry=FFFF

osIs0$:         CALLR                   ; -> Value
                ofAdc   0FFFFh          ; Value-1 isCarry
                osSwap                  ; isCarry Value-1
                osDrop                  ; isCarry ->
                RETR

; Jnz - ( Cond Addr -> )

osJnz$:         CALLR                   ; -> Cond Addr
                osSwap                  ; Addr Cond
                osIs0                   ; Addr Sign
                osDup                   ; Addr Sign Sign
                osNot                   ; Addr Sign !Sign
                osRP                    ; Addr Sign !Sign RP
                osPeek                  ; Addr Sign !Sign IP
                osAnd                   ; Addr Sign !Sign&IP
                osSwap                  ; Addr !Sign&IP Sign
                ofPick  2               ; Addr !Sign&IP Sign Addr
                osAnd                   ; Addr !Sign&IP Sign&Addr
                osOr                    ; Addr !Sign&IP|Sign&Addr
                osSwap                  ; !Sign&IP|Sign&Addr Addr
                osDrop                  ; !Sign&IP|Sign&Addr
                osRP                    ; !Sign&IP|Sign&Addr RP
                osPoke                  ; ->
                RETR

osDo$:          CALLR                   ; -> Start End
                osGet                   ; Start End RetAddr
                osSwap                  ; Start RetAddr End
                osPut                   ; Start RetAddr
                osSwap                  ; RetAddr Start
                osPut                   ; RetAddr
                osPut                   ; ->
                RETR

osI$:           CALLR                   ; ->
                osGet                   ; RetAddr
                osGet                   ; RetAddr Start
                osDup                   ; RetAddr Start Start
                osPut                   ; RetAddr Start
                osSwap                  ; Start RetAddr
                osPut                   ; Start ->
                RETR

osStep$:        CALLR                   ; -> Step
                osGet                   ; Step RetAddr
                osSwap                  ; RetAddr Step
                osGet                   ; RetAddr Step Start
                osAdd                   ; RetAddr Start+Step
                osGet                   ; RetAddr Start End
                os2Dup                  ; RetAddr Start End Start End
                osPut                   ; RetAddr Start End Start
                osPut                   ; RetAddr Start End
                osCmp                   ; RetAddr isCarry isZero
                osNot                   ; RetAddr isCarry !isZero
                osAnd                   ; RetAddr ?
                osSwap                  ; ? RetAddr
                osPut                   ; ? ->
                RETR

osChar$:        CALLR                   ; -> Char
                osToAX                  ;
                osInt29                 ; ->
                RETR

osStr$:         CALLR
                osToDX
                ofToAX  0900h
                osInt21
                RETR

osPeekb$:       CALLR                   ; -> Addr
                osPeek                  ; Value
                ofPush  0FFh            ; Value Mask
                osAnd                   ; Lo ->
                RETR

osPokeb$:       CALLR                   ; -> Value Addr
                osDup                   ; Value Addr Addr
                osPeek                  ; Value Addr Old
                ofPush  0FF00h          ; Value Addr Old 0FF00
                osAnd                   ; Value Addr Masked
                ofPick  2               ; Value Addr Masked Value
                ofPush  0FFh            ; Value Addr Masked Value 0FF
                osAnd                   ; Value Addr Masked Byte
                osOr                    ; Value Addr Word
                osSwap                  ; Value Word Addr
                osPoke                  ; Value
                osDrop                  ; ->
                RETR

osExch$:        CALLR                   ; -> Value
                osDP                    ; Value SP
                osDup                   ; Value SP SP
                osPeekb                 ; Value SP L
                osOver                  ; Value SP L SP
                osInc                   ; Value SP L SP+1
                osPeekb                 ; Value SP L H
                ofPick  2               ; Value SP L H SP
                osPokeb                 ; Value SP L
                osSwap                  ; Value L SP
                osInc                   ; Value L SP+1
                osPokeb                 ; Value ->
                RETR

osRcl$:         CALLR                   ; -> Value Carry
                osIs0                   ; Value TrueCY
                osNeg                   ; Value !TrueCY
                osSwap                  ; !TrueCY Value
                osShl                   ; TrueCY Value*2 isCarry
                osRot                   ; Value*2 isCarry !TrueCY
                osRot                   ; isCarry TrueCY Value*2
                osOr                    ; isCarry TrueCY|Value*2
                osSwap                  ; !TrueCY|Value*2 isCarry
                RETR

osShl$:         CALLR                   ; -> Value
                osDup                   ; Value Value
                osAdc                   ; Value*2 isCY ->
                RETR

; osCmp$ ( a b -> a b isCarry isZero) - compare
;   if a=b isZero=FFFF else (a<>b) isZero=0
;   if a<b isCarry=FFFF else (a>=b) isCarry=0

osCmp$:         CALLR                   ; a b ->
                osNeg                   ; a !b
                osSwap                  ; !b a
                osAdc                   ; Sum isCarry
                osNot                   ; Sum !isCarry
                osSwap                  ; !isCarry Sum
                osIs0                   ; !isCarry isZero ->
                osNot
                RETR

osToAX$:        CALLR
                ofPoke  *, emAX
                RETR

osToDX$:        CALLR
                ofPoke  *, emDX
                RETR
