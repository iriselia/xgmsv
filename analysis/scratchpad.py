import idautils
import idaapi
import idc
import ida_ua
import ida_hexrays

print("[+][lilpwny] script_begin")

func_name = 'printConfig'

for n in idautils.Names():
    if n[1] == func_name:
        ea = n[0]
        cursor = ea
        func = idaapi.get_func(ea)
        print(f"[+][lilpwny] {func_name} found at: {ea}")
        print(f"[+][lilpwny] {func}")

'''
cf: ida_hexrays.cfunc_t
cf = idaapi.decompile(ea)
for i, line in enumerate(cf.pseudocode):
    phead = idaapi.ctree_item_t()
    pitem = idaapi.ctree_item_t()
    ptail = idaapi.ctree_item_t()
    ret = cf.get_line_item(line.line, 0, True, phead, pitem, ptail)
    
    #expression = phead.it.to_specific_type
    print(f'{i}: {pitem.get_ea()}')
'''

cursor += 0x6

config_name = ''
data_address = 0
while cursor < func.end_ea:
    counter = 0
    mnem = idc.print_insn_mnem(cursor)
    
    if mnem == 'mov' or mnem == 'movzx':
        op0 = idc.get_operand_value(cursor, 0)
        op1 = idc.get_operand_value(cursor, 1)
        print(f'{mnem} {op0}, {op1}')
        if op1 != 0 and op1 != 10:
            str_val = idc.get_strlit_contents(op1)
            if str_val:
                str_val = str_val.decode('utf-8')
                if str_val != 'stderr@@GLIBC_2.0':
                    config_name = str(str(str_val).split(':')[0].strip().replace(' ', '_'))
                    print(f'configname: {config_name}')
            else:
                found = False
                for n in idautils.Names():
                    if n[0] == op1:
                        found = True
                        #print(f'n[1] {n[1]}')
                        #stderr
                if not found:
                    insn = ida_ua.insn_t()
                    inslen = ida_ua.decode_insn(insn, cursor)
                    data_address = op1
                    print(f'{op1} not found')

    if config_name and data_address:
        print(f'idc.MakeName({data_address}, {config_name})')
        idc.set_name(data_address, config_name)
        config_name = ''
        data_address = 0
        print(f'renamed {data_address} into {config_name}')
    #print(f'{idc.print_insn_mnem(cursor)}')
    #for i in range(5):
    #    if idc.print_insn_mnem(cursor) == 'lea':
    #        print('lol')
    cursor = idc.next_head(cursor, func.end_ea)

print("[+][lilpwny] script_end")
