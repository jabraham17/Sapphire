#!/usr/bin/env python3
import os
import argparse as ap

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


def filterName(name: str):
    name = name.replace("-", "_")
    name = name.upper()
    return name


def main():
    args = ap.ArgumentParser()
    args.add_argument("--fix", action="store_true")
    args = args.parse_args()

    fix = args.fix

    return_code = 0

    for h in allHeaders(src_root):
        local_path = os.path.relpath(h, os.path.commonpath([src_root, h]))
        local_path = os.path.normpath(local_path)
        local_dir = os.path.dirname(local_path)
        parts = local_dir.split(os.sep) + [os.path.splitext(os.path.basename(h))[0]]
        ifdef_name = ifdef_prefix + "_".join(parts) + ifdef_suffix
        ifdef_name = filterName(ifdef_name)
        lines = open(h, "r").readlines()
        if len(lines) < 2:
            print(f"'{local_path}' may be missing an include guard")
            return_code = 1
        else:
            ifdef = lines[0].strip()
            define = lines[1].strip()
            if ifdef == f"#ifndef {ifdef_name}" and define == f"#define {ifdef_name}":
                # this is fine
                pass
            elif not ifdef.startswith("#ifndef"):
                print(f"'{local_path}' missing include guard")
                return_code = 1
            else:
                print(f"'{local_path}' has a bad include guard")
                if fix:
                    with open(h, "w") as f:
                        lines[0] = f"#ifndef {ifdef_name}\n"
                        lines[1] = f"#define {ifdef_name}\n"
                        l = "".join(lines)
                        f.write(l)
                else:
                    return_code = 1

    return return_code


if __name__ == "__main__":
    exit(main())
