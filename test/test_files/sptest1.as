; second pass - test with negative array index and immediate value too big
MAIN: mov #-4096, LIST[1]
.define ind=-1
mov #3, LIST[-1]
mov #3, LIST[ind]
LIST: .data 3,-1,0
jmp MAIN
hlt