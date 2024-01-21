def addDecompilerComment(self, loc, comment):
        cfunc = idaapi.decompile(loc)
        eamap = cfunc.get_eamap()
        decompObjAddr = eamap[loc][0].ea
        tl = idaapi.treeloc_t()
        tl.ea = decompObjAddr
        commentSet = False
        for itp in range (idaapi.ITP_SEMI, idaapi.ITP_COLON):
            tl.itp = itp
            cfunc.set_user_cmt(tl, comment)
            cfunc.save_user_cmts()
            unused = cfunc.__str__()
            if not cfunc.has_orphan_cmts():
                commentSet = True
                cfunc.save_user_cmts()
                break
            cfunc.del_orphan_cmts()
        if not commentSet:
            print ("pseudo comment error at %08x" % loc) 