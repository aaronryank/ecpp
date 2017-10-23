@echo off
out\uncomment %1 > %1.tmp
out\ecpp %1.tmp
del %1.tmp
