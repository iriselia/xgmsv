import idautils
import idaapi
import idc
import ida_ua
import ida_hexrays

print("[+][lilpwny] script_begin")
func_name = 'nrproto_ServerDispatchMessage'
func = None
addr = 0
cursor = 0
names = idautils.Names()
for n in names:
    if n[1] == func_name:
        addr = n[0]
        cursor = addr
        func = idaapi.get_func(addr)
        print(f"[+][lilpwny] {func_name} found at: {addr}")
        print(f"[+][lilpwny] {func}")

opcode_str_addrs = []

cf = idaapi.decompile(addr)
num_cmts = idaapi.user_cmts_size(cf.user_cmts)
it = idaapi.user_cmts_begin(cf.user_cmts)

i = 0
while (i < num_cmts):
    t = idaapi.user_cmts_first(it)  #treeloc_t
    c = idaapi.user_cmts_second(it) #user_cmts_t
    #print(f"Comment: {c.c_str()} at addr: {hex(t.ea)} itp: {t.itp}")
    #d[f_ea][i] = {"ea" : t.ea, "comment": c.c_str(), "itp": t.itp}
    c = None
    i += 1
    it = idaapi.user_cmts_next(it)

while cursor < func.end_ea:
    if idc.print_insn_mnem(cursor) == 'lea':
        next3 = []
        new_cursor = cursor
        for i in range(0, 3):
            new_cursor = idc.next_head(new_cursor, func.end_ea)
            next3.append(idc.print_insn_mnem(new_cursor))
        if next3[0] == 'mov' and next3[1] == 'mov' and next3[2] == 'call':
            str_addr = idc.get_operand_value(cursor, 1)
            str_val = idc.get_strlit_contents(idc.get_operand_value(cursor, 1)).decode('utf-8')
            enc_str_val = idc.get_strlit_contents(idc.get_operand_value(cursor, 1) + 3800).decode('utf-8')
            
            tl = idaapi.treeloc_t()
            tl.ea = new_cursor
            tl.itp = idaapi.ITP_BRACE2
            cf.set_user_cmt(tl, "")
            cf.set_user_cmt(tl, f'"{enc_str_val}" -> "{str_val}"')
            #cf.del_orphan_cmts()
            cf.save_user_cmts()                        
            
            idc.set_name(idc.get_operand_value(cursor, 1), f"op_{enc_str_val}_{str_val}")
            #print(f"{idc.get_name(idc.get_operand_value(cursor, 1))}, {idc.print_operand(cursor, 1)} = {str_val}")
            #print(f"{idc.GetDisasm(cursor)}")
            print(f"{enc_str_val} : {str_val},")
    cursor = idc.next_head(cursor, func.end_ea)

print("[+][lilpwny] script_end")