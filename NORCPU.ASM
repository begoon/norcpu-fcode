                .model tiny
                .code
                .386

DataStkSize     equ     1024
RetStkSize      equ     256

                IfNDef DEBUG
                   org 100h
                Endif

                .Startup

                IfDef  DEBUG
                  push cs cs
                  pop  ds es
                Endif

; ---------------------------------------------------------------------------
; Main emulator's loop

                cld
emCPU:          mov     si, [ emIP ]
                lodsw
                xchg    di, ax
                mov     di, [di]
                lodsw
                xchg    bx, ax
                or      di, [bx]
                lodsw
                xchg    di, ax
                not     ax
                mov     [ emIP ], si
                stosw
                jmp     emCPU

InCmdPatch      equ     0EBh + (offset InCmd-offset emCPU-2)*256

; ---------------------------------------------------------------------------
; Start the execution of x86 subroutine
; This routine also switches the set of registers between emulated and x86's

InCmd:          pusha
                mov     ax, CPUbytes
                mov     word ptr emCPU, ax
                cli
                mov     emSP, sp
                mov     sp, offset Registers
                popa
                mov     sp, emSP
                sti
                push    offset OutCmd
                push    Patch
                retn

; ---------------------------------------------------------------------------
; Finish the execution of x86 subroutine
; This routine also switches the set of registers between emulated and x86's
; Out: em-registers from x86 subroutine ( include flags )

OutCmd:         cli
                mov     Patch, sp
                mov     sp, offset Registers+8*2+2
                pushf
                pusha
                mov     sp, Patch
                mov     emSP, sp
                sti
                popa
                jmp     emCPU

; ---------------------------------------------------------------------------
; ADD/CHECK routine in x86 code

RealAdc:        add     ax, cx
                sbb     cx, cx
                retn

emIP            dw      offset Program

                include NorMacro.inc

                include FLang.inc

Registers       label   word
emDI            dw      0
emSI            dw      0
emBP            dw      0
emSP            dw      0
emBX            dw      0
emDX            dw      0
emCX            dw      0
emAX            dw      0
emFlags         dw      0

emStack         dw      offset emDataStack      ; Emulator's stack pointer

emR1            dw      0
emR2            dw      0
emTmp1          dw      0
emTmp2          dw      0
Patch           dw      0                       ; Address of x86 routine
CPUBytes        dw      0                       ; Saved emulator's bytes

; There are some very useful constants

ConstPlus1      dw      1                       ; Const = 1
ConstMinus1     dw      -1                      ; Const = -1
ConstPlus2      dw      2                       ; Const = 2
ConstMinus2     dw      -2                      ; Const = -2

RetStack        label   word
                org     $+RetStkSize*2
StackLimit      label   word
                org     $+(DataStkSize-1)*2
emDataStack     label   word
                org     $+2
Buffer          label   byte

                end
