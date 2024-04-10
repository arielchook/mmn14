; .entry and .extern with a label (doesn't get recorded as label)
ENTLBL: .entry RESHIMA
reshima: .data 1,1,1,0,0
jmp ENTLBL
jmp EXTLBL
jmp RESHIMA
EXTLBL: .extern RESHIMAEXT
.extern     RESHIMA