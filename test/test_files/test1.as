; check that duplicates macros are not allowed
mcr mama
 mov r1,r2
 mov r2,r3
endmcr
MAIN: mov r3, LIST[sz]
avi
mcr papa
endmcr
LOOP: jmp W
mcr mama
 bne W
endmcr
