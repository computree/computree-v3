#!/usr/bin/env python

"""
Report the machine type of dlls and executables in the given directory.
"""

import os
import sys
import argparse
from struct import unpack, calcsize

def search(data, pattern):
    length = len(pattern)
    for off in range(len(data) - length):
        if data[off:off+length] == pattern:
            return off
    return None

def get_arch(path):
    fmt = '<ccccH'
    sz = calcsize(fmt)
    with open(path, "rb") as f:
        data = f.read(512)
        off = search(data, '\x50\x45\x00\x00')
        if not off:
            return None
        f.seek(off)
        data = f.read(sz)
        if data:
            s = unpack(fmt, data)
            if s[0] == 'P' and s[1] == 'E':
                if s[4] == 0x8664:
                    return "x86_64"
                elif s[4] == 0x14c:
                    return "x86"
                else:
                    return None
    return None

if __name__=="__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("pe")
    
    args = parser.parse_args()
    print(args.pe)
    if not os.path.isfile(args.pe):
        parser.print_usage()
        sys.exit(1)
    arch = get_arch(args.pe)
    print("arch: %s" % arch)