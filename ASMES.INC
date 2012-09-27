; Adc  ( a b -> c isCarry ) Crash AX, CX
; if a+b>FFFF isCarry = FFFF else isCarry=0

osAdc$:         emPOP   emAX            ; -> a b
                emPOP   emCX
                emADCr
                emPUSH  emAX
                emPUSH  emCX            ; c isCarry ->
                next

; osSwap ( a b -> b a ) Crash AX, CX

osSwap$:        emPOP   emAX
                emPOP   emCX
                emPUSH  emAX
                emPUSH  emCX
                next

; osRot ( a b c -> b c a ) Crash AX, BX, CX

osRot$:         emPOP    emAX
                emPOP    emBX
                emPOP    emCX
                emPUSH   emBX
                emPUSH   emAX
                emPUSH   emCX
                next

osInt21$:       emCmd   <Int 21h>
                next

osInt29$:       emCmd   <Int 29h>
                next

osPut$:         emADD   RP$, ConstPlus2, RP$
                emPOP   emAX
                emPOKE  emAX, RP$
                next

osGet$:         emPEEK  RP$, emAX
                emPUSH  emAX
                emADD   RP$, ConstMinus2, RP$
                next

osDrop$:        emPOP   Tmp$
                next

; osNor ( a b -> a NOR b ) Crash AX, BX

osNor$:         emPOP   emAX
                emPOP   emBX
                emNOR   emAX, emBX, emAX
                emPUSH  emAX
                next

osTrap$:        emCmd  <int 3>
                next

; osPeek ( addr -> value ) Crash BX

osPeek$:        emPOP   emBX
                emPEEK  emBX, emAX
                emPUSH  emAX
                next

; osPoke ( Value Addr -> ) Crash BX

osPoke$:        emPOP   emBX           ; -> Value
                emPOP   emAX           ; ->
                emPOKE  emAX, emBX
                next
