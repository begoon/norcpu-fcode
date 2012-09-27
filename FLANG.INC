
                include Macros.inc
                include ExpConds.inc

Program:        emMOVi  FCodeStart, IP$
                next

IP$             dw      offset FCodeStart
RP$             dw      offset RetStack
Tmp$            dw      0

; ---------------------------------------------------------------------------
; Address interpreter of F-Code

CALLR$:         emADD   RP$, ConstPlus2, RP$    ; RP+2 -> RP
                emPOKE  IP$, RP$
                emPOP   IP$
                next

RETR$:          emPEEK  RP$, IP$
                emADD   RP$, ConstMinus2, RP$   ; RP-2 -> RP
                next

NEXT$:          emPEEK  IP$, Tmp$
                emADD   IP$, ConstPlus2, IP$    ; IP+2 -> IP
                emJMP   Tmp$

osPush$:        emPEEK  IP$, Tmp$               ; [IP] -> Tmp
                emADD   IP$, ConstPlus2, IP$    ; IP+2 -> IP
                emPUSH  [Tmp$]
                next

; ---------------------------------------------------------------------------

                include Asmes.inc

                include FProc.inc

                include FTest.pre
