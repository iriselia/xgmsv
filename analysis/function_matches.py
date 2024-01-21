import os
from glob import glob

SA_PATH = 'Q:\\github\\StoneAge\\Source'

source_file_names = [y for x in os.walk(SA_PATH) for y in glob(os.path.join(x[0], '*.c'))]
header_file_names = [y for x in os.walk(SA_PATH) for y in glob(os.path.join(x[0], '*.h'))]

source_files = [[file_name.replace('\\', '/').replace('Q:/github/', ''), open(file_name, 'r', encoding='utf-8').read()] for file_name in source_file_names]
header_files = [[file_name.replace('\\', '/').replace('Q:/github/', ''), open(file_name, 'r', encoding='utf-8').read()] for file_name in header_file_names]

def searchText2(text):
    for file_name, file_content in source_files + header_files:
        if text in file_content:
            print(text + " word found in " + file_name)
            return text, file_name
        #else:
            #print("No match found in " + file_name)
    
    #print("No match found for " + text)
    return text, 'no_match_found'


def searchSAText():
    funcs = []
    with open('Q:\\github\\xgmsv\\analysis\\gmsv_funcs.txt') as f:
        lines = f.readlines()
        funcs = [line.split('\t')[0] for line in lines]
    
    with open('Q:\\github\\xgmsv\\analysis\\gmsv_funcs_found.txt', 'w+') as f:
        for func in funcs:
            func, file_name = searchText2(func)
            f.write(f'{func}, {file_name}\n')

searchSAText()
searchText2('Q:\\github\\StoneAge\\Source', 'MAP_getfloorY')