; first pass - test with out of bounds values in .define
MAIN: mov #-4096, LIST[1]
.define ind=-1
.define ind2=5000
mov #3, LIST[ind]
jmp MAIN
LIST: .data 3,-1,09999
hlt