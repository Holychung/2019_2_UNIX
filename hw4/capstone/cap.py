import binascii as b
import pwn
import capstone

conn = pwn.remote('aup.zoolab.org', 2530)
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
for i in range(10):
  data = pwn.recvuntil('Your answer: ').decode('utf-8').strip()
  data = data.split(">>> ")[1]
  code = b.a2b_hex(req)
  ans = ''
  for CsInsn in md.disasm(code, 0x1000):
       answer += f'{CsInsn.mnemonic} {CsInsn.op_str}\n'
   answer = b.b2a_hex(answer.encode('utf-8'))
   r.send(answer)
   r.send('\n')

r.interactive()
"""
context.arch = 'x86_64'
conn = pwn.remote('aup.zoolab.org', 2530)
conn.recvuntil('>>> ', drop=True).decode('utf-8')
first_trial = True
for i in range(10):
    req = conn.recvline().decode('utf-8').strip()
    if not first_trial:
        if req[-9:] == "Good job!":
            conn.recvuntil('>>> ', drop=True).decode('utf-8')
            req = conn.recvline().decode('utf-8').strip()
        else:
            break
    else:
        first_trial = False
    print(req)
    req_bytes = b.a2b_hex(req)
    raw_data = disasm(req_bytes)
    data = [str.strip() for str in raw_data.splitlines()]
    result_str = bytes('', encoding='utf-8')
    for line in data:
        counter = 0
        is_cmd = 0
        buf = ""
        for char in line:
            if counter > 10:
                is_cmd = 1
            if is_cmd == 1:
                buf += char
            else:
                if char == ' ':
                    counter = counter+1
                else:
                    counter = 0

        instruction = buf.split()
        asm_code = " ".join(instruction)
        # print(asm_code)
        if len(result_str) == 0:
            result_str += bytes(asm_code, 'utf-8')
        else:
            result_str += bytes("\n", 'utf-8')
            result_str += bytes(asm_code, 'utf-8')
    # print((result_str))
    s = result_str.decode('utf-8')
    result = b.b2a_hex(s.encode('utf-8'))
    conn.sendline(result)
    # print(result)

conn.interactive()
"""

# req = '48f7db4809da4809df'
# req_bytes = b.a2b_hex(req)
# disasm(req_bytes)
# r = b'nop\nxor rsi, rdi\nstc\ndec rbx\nxchg rdi, rsi\nstd\ninc rsi\n'
# s = r.decode('utf-8')
# print(s)
# b.b2a_hex(s.encode('utf-8'))

# raw_data = '   0:   90                      nop\n   1:   48 31 fe                xor    rsi, rdi\n   4:   f9                      stc    \n   5:   48 ff cb                dec    rbx\n   8:   48 87 f7                xchg   rdi, rsi'

# '''   
# 0:   90                      nop\n   
# 1:   48 31 fe                xor    rsi, rdi\n   
# 4:   f9                      stc    \n   
# 5:   48 ff cb                dec    rbx\n   
# 8:   48 87 f7                xchg   rdi, rsi'''

