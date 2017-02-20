#!/usr/bin/env python

description = """
Computree release script: creates a zip archive with executables and libraries.

The options for the release are in the win_release.json. Copy and modify this 
setting file to customize the release.

  - The Windows path seperator must be escaped (\\\\), otherwise the
    json parser may complains about invalid espace character.
  - The "params" key indicates the necessary paths for the release (build,
    executables, dependencies, output directory, Qt and MSVS).
  - The "deps" key is a list of files to copy in the release directory.
    The file is searched under "src_dir" and copied to "dst_dir". The paths are
    regular expressions passed to glob.  
  - All "params" key can be used in paths.
  - The script assumes its location is inside the source tree.
"""

import os
import sys
import glob
import json
import shutil
import subprocess
import argparse
import pprint
import datetime
from zipfile import ZipFile

import win_check

# NOTE: some debug dlls have a "d" name suffix: osg100-osg.dll => osg100-osgd.dll
# For now, let's support only release build.
# 

# Reference: http://stackoverflow.com/questions/1094841/reusable-library-to-get-human-readable-version-of-file-size
def sizeof_fmt(num, suffix='B'):
    for unit in ['','Ki','Mi','Gi','Ti','Pi','Ei','Zi']:
        if abs(num) < 1024.0:
            return "%3.1f%s%s" % (num, unit, suffix)
        num /= 1024.0
    return "%.1f%s%s" % (num, 'Yi', suffix)

# find the project root
def find_srcdir(path, folder):
    # os.listdir() returns a list of str, while folder is a unicode object.
    # Comparison operator requires both be the same type.
    s = str(folder)
    while(True):
        lst = os.listdir(path)
        if s in lst:
            return path
        temp = os.path.dirname(path)
        if temp == path:
            raise IOError("Cannot find the parent directory containing folder %s" % repr(folder))
        path = temp

def expand_files(params, deps):
    for dep in deps:
        dep["files"] = []
        for pattern in dep.get("globs", []):
            path = os.path.join(dep["src_dir"], pattern)
            path = path.format(**params)
            res = glob.glob(path)
            if len(res) == 0:
                print("warning: glob captured no files: {}".format(path))
            for file in res:
                dep["files"].append(file)

def compute_file_size(files):
    size = 0
    for file in files:
        s = os.stat(file)
        size += s.st_size
    return size

def copy_files(files, dst_dir):
    for src in files:
        name = os.path.basename(src)
        dst = os.path.join(dst_dir, name)
        shutil.copy(src, dst)

def check_keys(lst1, lst2):
    return list(set(lst2).difference(set(lst1)))

def check_arch(exe, base, log):
    main_arch = win_check.get_arch(exe)
    if not main_arch:
        raise Exception("Failed to detect arch of file {}".format(exe))
    bad = []
    print("{}: {}".format(os.path.basename(exe), main_arch))
    for root, dirs, files in os.walk(base):
        for file in files:
            if file.endswith(".exe") or file.endswith(".dll"):
                p = os.path.join(root, file)
                arch = win_check.get_arch(p)
                ok = arch == main_arch
                
                if ok:
                    msg = "PASS"
                else:
                    msg = "FAIL"
                    bad.append(file)
                log.write("{0: <20}: {1: <7} {2: <7}\n".format(file, arch, msg))
    return bad

mandatory_keys = [ "params", "deps", "archive_format" ]

mandatory_params = [
    "main_dir",
    "out_dir",
    "build_dir",
    "deps_dir",
    "main_exe",
    "qt_dir",
    "msvc_dir",
]

def main(log):
    
    dir = os.path.dirname(os.path.abspath(__file__))
    parser = argparse.ArgumentParser(description=description, 
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("options", metavar="FILE", nargs='?',
                        default=os.path.join(dir, "win_release.json"),
                        help="JSON file with release options")
    
    args = parser.parse_args()
    if not os.path.isfile(args.options):
        parser.print_usage()
        sys.exit(1)
    
    with open(args.options, "r") as f:
        try:
            opts = json.load(f)
        except Exception as e:
            print("Failed to parse options file: {}".format(args.options))
            print(e.message)
            sys.exit(1)
    
    # FIXME: what we really want is to check against a dictionnary
    missing = check_keys(opts.keys(), mandatory_keys)
    missing += check_keys(opts["params"].keys(), mandatory_params)
    
    if len(missing) > 0:
        print("Error: missing options: {}".format(missing))
        print("Define missing options to fix this issue.")
        sys.exit(1)
    
    params = opts["params"]
    deps = opts["deps"]
    
    try:
        top_srcdir = find_srcdir(dir, params["main_dir"])
    except Exception as e:
        print("Failed to find the file indicating the top src directory: {}".format(opts["params"]["main_dir"]))
        sys.exit(1)
    
    params["top_srcdir"] = top_srcdir
    # expand top_srcdir
    for k, v in params.items():
        params[k] = v.format(top_srcdir=top_srcdir)
    pprint.pprint(params, log)

    out_path = os.path.join(top_srcdir, params["out_dir"])
    if os.path.exists(out_path):
        if not os.path.isdir(out_path):
            raise IOError("Output path already exists: {}".format(out_path))
    else:
        os.mkdir(out_path)

    expand_files(params, deps)
    
    size = 0
    num_files = 0
    for dep in deps:
        files = dep["files"]
        num_files += len(files)
        size += compute_file_size(files)
    print("found %d files %s" % (num_files, sizeof_fmt(size)))

    pprint.pprint(deps, log)

    for dep in deps:
        dst_dir = dep["dst_dir"].format(**params)
        if not os.path.exists(dst_dir):
            os.makedirs(dst_dir)
        copy_files(dep["files"], dst_dir)

    # assert that the libraries and the main program have the same arch
    exe = params["main_exe"]
    main_arch = win_check.get_arch(exe)
    bad = check_arch(exe, out_path, log)
    if len(bad) > 0:
        print("Architecture mismatch detected. See the log file for details.")
        sys.exit(1)
    
    fmt = opts["archive_format"].format(arch=main_arch)
    fmt = datetime.datetime.utcnow().strftime(fmt)
    out_zip = os.path.join(top_srcdir, fmt + ".zip")
    with ZipFile(out_zip, "w") as zippy:
        for root, dirs, files in os.walk(out_path):
            for file in files:
                rel = os.path.relpath(root, out_path)
                path = os.path.join(root, file)
                zip_name = os.path.join(fmt, rel, file)
                print("{} -> {}".format(path, zip_name))
                zippy.write(path, zip_name)

    zip_size = s = os.stat(out_zip).st_size
    print("archive ready: {} {}".format(os.path.basename(out_zip), sizeof_fmt(zip_size)))
    
if __name__=="__main__":
    with open("win_release_log.txt", "w") as log:
        main(log)

#    FIXME: is windeployqt really important?
#    windeployqt = r"C:\Qt\5.8\msvc2013_64\bin\windeployqt.exe"
#    cmd_env = os.environ.copy()
#    cmd_env["VCINSTALLDIR"] = msvc_dir
#    subprocess.check_call([windeployqt, os.path.join(out_path, main_exe)], env=cmd_env)