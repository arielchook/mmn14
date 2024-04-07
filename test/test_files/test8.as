; test with long statements (longer than 80)
TEST: jmp                                                                   LOOP       
mcr mama 
mov r1,r2   
mov r2,r3
endmcr 
MAIN: mov r3, LIST[sz]
LOOP: jmp W
