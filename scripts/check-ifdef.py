#!/usr/bin/env python3
import os

script_path = os.path.abspath(__file__)
script_dir = os.path.dirname(script_path)
project_root = os.path.dirname(script_dir)

ifdef_prefix = "SAPPHIRE_"
ifdef_suffix = "_H_"
src_root = os.path.join(project_root, "src")


def allHeaders(path: str):
    for dirpath, dirnames, filenames in os.walk(path):
        for filename in filenames:
            p = os.path.join(dirpath, filename)
            if os.path.splitext(p)[1] == ".h":
                yield p


return_code = 0

for h in allHeaders(src_root):
    local_path = os.path.relpath(h, os.path.commonpath([src_root, h]))
    local_path = os.path.normpath(local_path)
    parts = local_path.split(os.sep) + [os.path.splitext(os.path.basename(h))[0]]
    ifdef_name = ifdef_prefix + "_".join(parts) + ifdef_suffix
    with open(h, "r") as f:
        lines = f.readlines()
        if len(lines) < 2:
            print(f"'{local_path}' may be missing an include guard")
            return_code = 1
        else:
            ifdef = lines[0]
            define = lines[1]
            if ifdef == f"#ifndef {ifdef_name}" and define == f"#define {ifdef_name}":
                # this is fine
                pass
            elif not ifdef.startswith("#ifndef"):
                print(f"'{local_path}' missing include guard")
                return_code = 1
            else:
                print(f"'{local_path}' has a bad include guard")
                return_code = 1

exit(return_code)
