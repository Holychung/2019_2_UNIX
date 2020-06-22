; test if registers are positive or negative.
cmp eax, 0
jl L1
mov eax, 1
mov [0x600000], eax
jmp L12
L1: 
mov eax, -1
mov [0x600000], eax

L12:
cmp ebx, 0
jl L2
mov ebx, 1
mov [0x600004], ebx
jmp L23
L2: 
mov ebx, -1
mov [0x600004], ebx

L23:
cmp ecx, 0
jl L3
mov ecx, 1
mov [0x600008], ecx
jmp L34
L3: 
mov ecx, -1
mov [0x600008], ecx

L34:
cmp edx, 0
jl L4
mov edx, 1
mov [0x60000c], edx
jmp L44
L4: 
mov edx, -1
mov [0x60000c], edx
L44:
done: