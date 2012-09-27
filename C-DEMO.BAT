@echo off
pre <demo.asm >demo.pre
tasmx /m5 /la demo.pre
tlink /t demo