import idautils
import idaapi
import idc
import ida_ua

funcs = []
with open('Q:\\github\\xgmsv\\analysis\\gmsv_funcs_found.txt') as f:
    lines = f.read().splitlines()
    funcs = [[line.split(', ')[0], line.split(', ')[1]] for line in lines]

#print(funcs)

print("[+][lilpwny] script_begin")
func = None
addr = 0
cursor = 0
name_gen = idautils.Names()
names = []
ignore_list = [
'no_match_found',
'DB_proc_writeOffGuild',
]
for name in name_gen:
    names.append(name)

for func in funcs:
    #print(f"[+][lilpwny] {func}, names: {len(names)}")
    if func[1] == 'no_match_found':
        continue
    if func[0] in ignore_list:
        print(f'ignored {func[1]}')
        continue
    for n in names:
        #print(f"[+][lilpwny] {n}")
        if n[1] == func[0]:
            addr = n[0]
            print(f"[+][lilpwny] {func[0]} found at: {addr}")
            print(f"[+][lilpwny] added comment {func[1]}")
            idc.set_func_cmt(addr, func[1], 1)

    
            #cf = idaapi.decompile(addr)
            #tl = idaapi.treeloc_t()
            #tl.ea = addr
            #tl.itp = idaapi.ITP_EMPTY
            #cf.set_user_cmt(tl, "")
            #cf.set_user_cmt(tl, func[1])
            
            #cf.del_orphan_cmts()
            
            #cf.save_user_cmts()   
                        
print("[+][lilpwny] script_end")