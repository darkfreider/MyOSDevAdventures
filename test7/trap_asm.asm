
extern trap
all_traps:
    pusha

    push esp
    call trap
    add esp, 4

    popa
    add esp, 8
    
    iretd

; NOTE(max): 
;     Sweet code generation; 1 macro 255 interrup vector entries
%macro GENERATE_INT_VECTORS 0

    %assign i 0
    %rep 256
    %if ((i == 8) || (i >= 10 && i <= 14) || (i == 17) || (i == 21))
        global vector%+ i 
        vector%+ i:
            push i
            jmp all_traps      
    %else
        global vector%+ i
        vector%+ i:
            push 0
            push i
            jmp all_traps
    %endif
    
    %assign i i+1
    %endrep
%endmacro

; NOTE(max): actually generate code
GENERATE_INT_VECTORS

%macro GENERATE_INT_VECTORS_ARRAY 0
    global vectors_array 
    vectors_array:

    %assign i 0
    %rep 256
        dd vector%+ i
    %assign i i+1
    %endrep

%endmacro

; NOTE(max): actually generate array
GENERATE_INT_VECTORS_ARRAY











