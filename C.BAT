@echo off
del *.pre
pre <norcpu.asm >norcpu.pre
pre <ftest.inc >ftest.pre
tasmx /la /m2 norcpu.pre
tlink /t norcpu
