; check that duplicates entries are not allowed
.entry LIST
.extern W
.define sz = 2
MAIN: mov r3, LIST[sz]
mov r1,r2
mov r2,r3
LOOP: jmp W
prn     #-5
mov STR[5], STR[2]
sub r1, r4
cmp K, #sz
bne W
mov r1,r2
mov r2,r3
mov r1,r2
mov r2,r3
L1: inc L3
.entry LOOP
bne LOOP
END: hlt
.define len = 4
STR: .string "abcdef"
jmp L1
LIST: .data 6, -9, len
.extern L3
jmp L1
K: .data 22
jmp L1
