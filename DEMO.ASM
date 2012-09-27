.model tiny
.code
;.386
org 100h


     include Conds.inc
start:
     mov ax, 0F002h
     cmp ax, 0F001h

     xor ax, ax
     $If <=
       daa
       int 20h
     $else
       daa
       daa
     $Endif

     $if <
        daa
;        db 500 dup (0)
         xlat
        daa
     $endif

     hlt

     $Do
       $ExitDo =
       $ContDo
     $EndDo Loop

     xor ax, ax
     $If <=
       daa
       int 20h
     $else
       daa
       daa
     $Endif

     lea dx, msg2
     $if <
       db 256 dup (0)
       lea dx, msg1
     $else
       nop
       $if =0
         hlt
       $endif
       nop
     $endif

     $if =0
     $endif

     mov cx, 2
     $Do
       nop
       $Do
       $EndDo =
       dec cx
       dw 256 dup (0)
     $EndDo Loop
     $Do
       $ExitDo
       $ContDo
     $EndDo Loop

     Store ds, si, ax
       Store ax, bx,cx, es, bp
       Restore
     Restore

     mov ah, 9
     int 21h
     int 20h

msg2  db 'False!$'
msg1  db 'True!$'

end start
