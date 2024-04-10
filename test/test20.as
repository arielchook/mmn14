; second pass invalid addressing types
MAIN: mov r2,r3
.define x=3
mov x,MAIN
mov #2,MAIN      [2]       
mov #3,MAIN [        -4]
mov r1[2], r4