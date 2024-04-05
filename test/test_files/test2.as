; check that nested macros are not allowed
mcr mama
 mov r1,r2
 mov r2,r3
 mcr papa
 hlt
 endmcr
endmcr
MAIN: mov r3, LIST[sz]
mama
LOOP: jmp W
