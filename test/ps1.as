; file ps.as
.entry LIST
.extern W
.define sz = 2  
mcr avi
 mov r1,r2
 mov r2,r3
endmcr
MAIN: mov r3, LIST[sz]
avi
mcr papa
endmcr
LOOP: jmp W
 prn #-5
 mov STR[5], STR[2]
 sub r1, r4
 cmp K, #sz
 bne W
 avi
 avi
 papa
L1: inc L3
.entry LOOP
 bne LOOP
 papa
END: hlt
.define len = 4
STR: .string “abcdef”
LIST: .data 6, -9, len
K: .data 22
.extern L3 