#!/usr/bin/env python3

from __future__ import annotations
import functools
import sys
import util.FileHelper as FileHelper
from typing import Any, Dict, Generator, List, Set, Union
from dataclasses import dataclass
import os
import enum
import re
import argparse as ap
import multiprocessing as mp
import glob
import itertools
from args import arguments
import TestDefinition
import shutil
import textwrap
import util.color as COLOR


def get_env(script: str) -> Dict[str, Any]:
    """set env vars based on this files path, relative to the current working directory
    this is usually sys.argv[0]"""
    # should be /path/to/ZirconSim/tests/driver/test-driver.py
    env = dict()
    env["SCRIPT_DIR"] = os.path.dirname(FileHelper.makeabs(script))
    env["PRIMARY_TEST_DIR"] = os.path.dirname(env["SCRIPT_DIR"])
    env["TEST_CONFIG"] = os.path.join(env["SCRIPT_DIR"], "config")
    env["PROJECT_HOME"] = os.path.dirname(env["PRIMARY_TEST_DIR"])

    env["BUILD_DIRECTORY"] = os.path.join(env["PROJECT_HOME"], "build")
    return env


def print_list(tests: List[TestDefinition.TestConfiguration]):
    for t in tests:
        print("[Test " + t.name() + "]")
        if t.compile_options.options:
            print(f"  [Build Options '{str(t.compile_options)}']")
        if t.exec_options.options:
            print(f"  [Exec Options '{str(t.exec_options)}']")
        if not t.expected_file.isGenerated():
            print(f"  [Expected File '{t.expected_file.prettyPath()}']")


def print_summary(
    test_results: List[TestDefinition.TestResult], env: Dict[str, str], use_color
):
    width = 80
    print()
    print("Test Summary".center(width, "="))
    passed = 0
    for t in sorted(test_results, key=lambda t: t.name()):
        if t.isSuccess():
            passed += 1
        name = t.name(True, env)
        res_str = "Passed" if t.isSuccess() else "Failed"
        s = name + " "
        color = (
            COLOR.GREEN(use_color, bright=True)
            if t.isSuccess()
            else COLOR.RED(use_color, bold=True)
        )
        s += color + res_str.rjust(width - len(s)) + COLOR.RESET(use_color)
        print(s)
    print()
    print(
        f"Passed {passed}/{len(test_results)} ({100.0*(float(passed)/len(test_results)):4.2f})"
    )
    print("=" * width)


def main(args: List[str], env: Dict[str, str]) -> int:
    a = arguments(args, env)

    test_names = TestDefinition.find_all_unique_test_names(a.paths, a.exclude)

    tests: List[TestDefinition.TestConfiguration] = []
    for test_name in test_names:
        tests.extend(TestDefinition.TestConfiguration.build(test_name, env))

    if a.list:
        print_list(tests)
    else:
        test_instances = [
            TestDefinition.TestInstance(t, a.diff, not a.keep, a.verbose) for t in tests
        ]

        env_run_func = functools.partial(TestDefinition.TestInstance.run, env=env)
        test_results: List[TestDefinition.TestResult]
        with mp.Pool() as pool:
            test_results = pool.map(env_run_func, test_instances)

        print_summary(test_results, env, a.color)

        if not a.keep:
            for r in test_results:
                r.clean()
    if not a.keep:
        for t in tests:
            t.expected_file.clean()

    return 0


if __name__ == "__main__":
    env = get_env(sys.argv[0])
    exit(main(sys.argv[1:], env))
