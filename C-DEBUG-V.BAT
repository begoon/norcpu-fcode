@echo off
del *.pre
pre <norcpu.asm >norcpu.pre
pre <ftest.inc >ftest.pre
tasmx /dDEBUG /zi /la /m5 norcpu.pre
tlink /v norcpu
