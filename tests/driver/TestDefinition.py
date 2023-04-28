from __future__ import annotations
import functools
import shlex
import sys
import textwrap
import util.FileHelper as FileHelper
from typing import Any, Dict, Generator, List, Set, Tuple, Union
from dataclasses import dataclass, field
import os
import enum
import re
import argparse as ap
import multiprocessing as mp
import glob
import itertools
from args import arguments
import util.util as util
from util.StringMatcher import StringMatcher
import subprocess as sp
import util.color as color


@util.static_vars(
    filename_matcher=StringMatcher(
        r"([a-zA-Z0-9_\-\./ ]+)\.((spp)|(exp)|([a-zA-Z0-9_\-\.]+\.exp)|(compopt)|(execopt))$",
        StringMatcher.Flags.REGEX,
    )
)
def isTestFile(
    filepath: str, exclude_list: List[StringMatcher] = []
) -> Union[str, None]:
    """
    if this file is a valid test file
    returns the basename of the test, including the full path

    criteria to be a valid file
    - file is not in the exclude list
    - file is of the test filename format
      - /path/to/TESTNAME.EXT where EXT is one of the valid source file extensions
      - /path/to/TESTNAME.exp
      - /path/to/TESTNAME.SUBNAME.exp
      - /path/to/TESTNAME.compopt
      - /path/to/TESTNAME.execopt
    """

    # check if excluded
    for e in exclude_list:
        if e.match(filepath):
            return None

    m = isTestFile.filename_matcher.match(filepath)
    if not m:
        return None
    else:
        test_name: str = m[0]  # should be only one match
        if os.path.basename(test_name).startswith("._"):
            return None
        return test_name


class TestFile:
    """wrapper class for a path, keeps state of whether this file was generated or not"""

    def __init__(self, path: str, generated: bool = False, prefix=None):
        self._path = path
        self._generated = generated
        self._filemode = "r"
        self._fd = None
        if prefix:
            self._path = TestFile.prefix_temp(self._path, prefix=prefix)

    def __repr__(self):
        return f"TestFile({self.getPath()})"

    def isGenerated(self):
        return self._generated

    def getPath(self):
        return self._path

    def prettyPath(self):
        cwd = os.getcwd()
        pre = os.path.relpath(self._path, cwd)
        return pre

    def __call__(self, filemode: str):
        self._filemode = filemode
        return self

    def __enter__(self):
        self._fd = open(self._path, self._filemode)
        return self._fd

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self._fd.close()
        self._fd = None

    def remove(self):
        """unchecked remove of TestFile"""
        os.remove(self._path)

    def clean(self):
        """checked remove of file
        dont remove non-generated files and dont remove it if doesn't exist (obviously)
        """
        if self.isGenerated() and FileHelper.fileExists(self._path):
            self.remove()

    @classmethod
    def default_prefix(cls) -> str:
        return "._"

    @classmethod
    def prefix_temp(cls, path: str, prefix=None) -> str:
        """return the path with a temp prefix of '._'"""
        if not prefix:
            prefix = cls.default_prefix()
        d, basename = os.path.split(path)
        basename = prefix + basename
        path = os.path.join(d, basename)
        return path

    @classmethod
    def create(
        cls, path: str, copy_path: str = None, content: List[str] = []
    ) -> TestFile:
        """create a generated file, filename is prefixed as a temp"""
        path = TestFile.prefix_temp(path)

        # copy content from copy to here
        if copy_path:
            if not FileHelper.fileExists(copy_path):
                raise ConfigurationError(
                    "Specified copy path '" + copy_path + "' unknown"
                )
            with open(copy_path, "r") as copy:
                with open(path, "w") as f:
                    f.writelines(copy.readlines())

        # then append the content to it
        with open(path, "a") as f:
            f.writelines(content)
        return TestFile(path, generated=True)


def yieldTestSourceFile(test_name: str) -> Generator[TestFile, None, None]:
    """given a base test_name, return all next possible source files with the base name"""
    valid_src_ext = [".spp"]
    for e in valid_src_ext:
        path = test_name + e
        if FileHelper.fileExists(path):
            yield TestFile(path)


def skipDirectory(directory_path: str) -> bool:
    """
    if this directory should be skipped

    criteria to be skipped
    - directory contains a SKIP file
    """
    p = os.path.join(directory_path, "SKIP")
    return os.path.exists(p)


def find_all_unique_test_names(paths: List[str], exclude: List[str] = []) -> List[str]:
    """find all testcases within a given path"""
    test_names = set()
    for path in paths:
        test_names.update(_find_all_unique_test_names(path, exclude))
    return list(test_names)


def _is_excluded(path: str, exclude: List[str]) -> bool:
    for e in exclude:
        if re.search(e, path):
            return True
    return False


# TODO: refactor this method name
# TODO: maybe make this a generator for perf?
def _find_all_unique_test_names(path: str, exclude: List[str]) -> Set[str]:
    # check a single file and return its match immediately
    # since its a file dont need to walk its dir, there is none :)
    if (
        not _is_excluded(path, exclude)
        and FileHelper.fileExists(path)
        and (test_name := isTestFile(path))
    ):
        return {test_name}

    test_names = set()
    for p in os.listdir(path):
        full_p = os.path.join(path, p)

        # skip directories not included
        if skipDirectory(full_p):
            continue

        if (
            not _is_excluded(path, exclude)
            and os.path.isfile(full_p)
            and (test_name := isTestFile(full_p))
        ):
            test_names.add(test_name)

        if os.path.isdir(full_p):
            test_names.update(_find_all_unique_test_names(full_p, exclude))

    return test_names


def parse_variable_file(config_file: str, env: Dict[str, Any]) -> Dict[str, str]:
    variables = dict()

    class ContinueOuterLoop(Exception):
        pass

    with open(config_file, "r") as f:
        for idx, l in enumerate(f.readlines()):
            try:
                l = l.strip()
                # skip if no test defined
                if not l or len(l) == 0 or l.startswith("#"):
                    raise ContinueOuterLoop
                elms = l.split("=", maxsplit=1)
                if len(elms) != 1 and len(elms) != 2:
                    print(
                        "Invalid line '"
                        + l
                        + "' in '"
                        + config_file
                        + "' on line '"
                        + (idx + 1)
                        + "'"
                    )
                    raise ContinueOuterLoop

                name = elms[0].strip()
                cmd = elms[1].strip() if len(elms) == 2 else ""

                def do_var_replace(match: re.Match):
                    env_name = match.group(1)
                    if env_name in env:
                        return env[env_name]
                    else:
                        print(
                            "Unknown variable name '"
                            + env_name
                            + "' in '"
                            + config_file
                            + "' on line '"
                            + (idx + 1)
                            + "'"
                        )
                        raise ContinueOuterLoop

                cmd = re.sub(r"\$\((.+)\)", do_var_replace, cmd)

                variables[name] = cmd

            except ContinueOuterLoop:
                pass
    return variables


class ConfigurationError(Exception):
    """raised when a configuration is invalid"""

    def __init__(self, msg: str):
        self.msg = msg

    def __str__(self):
        return str(__class__) + ": " + self.msg


@dataclass
class Configuration:
    """represents a set of compile/exec options"""

    options: str = ""
    exp_name: Union[str, None] = None

    def __post_init__(self):
        # if there is a subtest name, make sure it is valid
        self.exp_name = (
            self.exp_name if self.exp_name and len(self.exp_name) != 0 else None
        )

    def __str__(self):
        s = self.options
        return s

    @classmethod
    def _parse_config_line(cls, line: str) -> Configuration:
        elms = [e.strip() for e in line.split("#", maxsplit=1)]
        if len(elms) == 0:
            return None
        if len(elms) == 1:
            c = Configuration(elms[0])
        elif len(elms) == 2:
            c = Configuration(elms[0], elms[1])
        else:
            raise ConfigurationError("Invalid configuration for line - '" + line + "'")
        return c

    @classmethod
    def build(cls, path: TestFile) -> List[Configuration]:
        """build a series of configurations based on a path"""
        with path("r") as fd:
            configs = [
                c for l in fd.readlines() if (c := Configuration._parse_config_line(l))
            ]
            return configs

    @classmethod
    def build_or_default(cls, path: str, default: str = "") -> List[Configuration]:
        """build a series of configurations based on a path, if path is invalid return an empty config with default args"""
        configs: List[Configuration] = []
        if FileHelper.fileExists(path):
            configs = Configuration.build(TestFile(path))
        else:
            configs = [Configuration(options=default)]
        return configs

    def __hash__(self):
        return hash(self.options + self.exp_name)


@dataclass
class TestConfiguration:
    """represents a combination of compile options, exec options, tool options"""

    test_name: str
    source_file: TestFile
    expected_file: TestFile
    compile_options: Configuration
    exec_options: Configuration
    sub_name: Union[str, None] = None

    def name(self, pretty=False, env: Dict[str, Any] = dict()):
        name = self.test_name
        if pretty:
            name = os.path.relpath(
                name,
                os.path.commonprefix(
                    [
                        name,
                        env.get("PROJECT_HOME", ""),
                    ]
                ),
            )
        if self.sub_name:
            return f"{name}.{self.sub_name}"
        else:
            return name

    @classmethod
    def build(cls, test_path: str, env: Dict[str, Any]) -> List[TestConfiguration]:
        """build a series of test configurations, given a basename"""

        # test_name = os.path.relpath(test_path, os.path.commonprefix([test_path, env["PROJECT_HOME"]]))

        # get all possible source files for the base name, if there is more than one its an error
        src_files: Generator[TestFile, None, None] = yieldTestSourceFile(test_path)
        src_file: TestFile = None
        try:
            src_file = next(src_files)
        except StopIteration:
            # No src file
            return []
        # try and next() the generator one more time, if it yields something we have a conflict, config error
        try:
            next(src_files)
            raise ConfigurationError("Too many source files for '" + test_path + "'")
        except StopIteration:
            pass

        # load the exec opt and comp opt if they exists, if they dont exist use empty configs
        comp_configs = Configuration.build_or_default(test_path + ".compopt")
        exec_configs = Configuration.build_or_default(test_path + ".execopt")

        test_configurations = []

        configs = list(itertools.product(comp_configs, exec_configs))
        if len(configs) == 1:
            config = configs[0]
            (comp_config, exec_config) = config
            exp_path = test_path + ".exp"  # default name
            if comp_config.exp_name:
                exp_path = os.path.join(
                    os.path.dirname(test_path), comp_config.exp_name
                )
            if exec_config.exp_name:
                exp_path = os.path.join(
                    os.path.dirname(test_path), exec_config.exp_name
                )
            t = TestConfiguration(
                test_name=test_path,
                source_file=src_file,
                expected_file=TestFile(exp_path),
                compile_options=comp_config,
                exec_options=exec_config,
                sub_name=None,
            )
            test_configurations.append(t)
        else:
            idx = 1
            for config in configs:
                sub_name = str(idx)
                idx += 1

                (comp_config, exec_config) = config
                exp_path = test_path + "." + sub_name + ".exp"  # default name
                if not FileHelper.fileExists(exp_path):
                    exp_path = test_path + ".exp"
                if comp_config.exp_name:
                    exp_path = os.path.join(
                        os.path.dirname(test_path), comp_config.exp_name
                    )
                if exec_config.exp_name:
                    exp_path = os.path.join(
                        os.path.dirname(test_path), exec_config.exp_name
                    )
                t = TestConfiguration(
                    test_name=test_path,
                    source_file=src_file,
                    expected_file=TestFile(exp_path),
                    compile_options=comp_config,
                    exec_options=exec_config,
                    sub_name=sub_name,
                )
                test_configurations.append(t)
        return test_configurations


@dataclass
class TestInstance:
    """instance of test, continues information to run and check the output of a test"""

    config: TestConfiguration
    cleanup: bool = False
    verbose: bool = False
    color: bool = False

    _log: List[str] = field(default_factory=list, init=False)

    @classmethod
    def _execute(cls, cmd: List[str], output_file: TestFile = None) -> int:
        cmd = [c.getPath() if isinstance(c, TestFile) else c for c in cmd]
        if output_file:
            with output_file("a") as output_fd:
                p = sp.Popen(cmd, stdout=output_fd, stderr=output_fd)
                p.communicate()
                return p.returncode
        else:
            p = sp.Popen(cmd, stdout=sp.DEVNULL, stderr=sp.DEVNULL)
            p.communicate()
            return p.returncode

    def _build_testcase(
        self, log_file: TestFile, env: Dict[str, Any]
    ) -> Tuple[bool, Union[TestFile, None]]:
        """if build success, returns (success, object)
        object on success is executable
        object on failure is reason"""
        test_name = self.config.name()
        compile_options = self.config.compile_options.options
        source_file = self.config.source_file
        executable_file = TestFile(
            test_name + ".out", generated=True, prefix=TestFile.default_prefix()
        )

        compiler_build_path = env["BUILD_DIRECTORY"]

        compiler = os.path.join(compiler_build_path, "bin", "sapphire")
        compiler = TestFile(compiler, generated=False)

        cmd = (
            [compiler]
            + shlex.split(compile_options)
            + [source_file, "-o", executable_file]
        )
        self._verbose_print_cmd("Building", cmd)

        ret = TestInstance._execute(cmd, log_file)

        if ret == 0:
            return (True, executable_file)
        else:
            return (False, None)

    def _execute_testcase(
        self, executable: TestFile, log_file: TestFile, env: Dict[str, Any]
    ) -> Tuple[bool, Union[str, None]]:
        """runs executable and returns on success bool
        Note this function does NOT check return code of the testcase"""
        exec_options = self.config.exec_options.options

        cmd = [executable] + shlex.split(exec_options)
        self._verbose_print_cmd("Executing", cmd)

        # intentionally not checking return code, this has no bearing on test case result
        TestInstance._execute(cmd, log_file)

        return True

    def _check_testcase(self, output: TestFile) -> Tuple[bool, TestFile]:
        exp = self.config.expected_file
        cmd = ["diff", "-y", exp, output]
        self._verbose_print_cmd("Checking", cmd)
        diff_file = TestFile.create(self.config.expected_file.getPath() + ".diff")
        ret = TestInstance._execute(cmd, diff_file)
        succ = ret == 0
        return (succ, diff_file)

    def _verbose_print_cmd(self, prefix: str, cmd: List):
        cmd_s = [c.prettyPath() if isinstance(c, TestFile) else c for c in cmd]
        s = (
            "["
            + color.YELLOW(self.color, bright=True)
            + prefix
            + color.RESET(self.color)
            + " '"
            + " ".join(cmd_s)
            + "']"
        )
        self._verbose_print(s)

    def _verbose_print(self, text: str):
        if self.verbose:
            self._log.append(text)

    def _dump_log(self):
        if self.verbose:
            s = os.linesep.join(self._log)
            print(s)

    # TODO: needs a serious refactor
    def _run(self, env: Dict[str, Any]) -> TestResult:
        """run a test case and generate the result"""

        test_name = self.config.name()
        output = TestFile.create(test_name + ".output")

        (build_success, executable) = self._build_testcase(output, env)
        if build_success:
            # if we can build, try and execute
            exec_success = self._execute_testcase(executable, output, env)
            if self.cleanup:
                executable.clean()

        (success, diff_output) = self._check_testcase(output)

        return TestResult(
            self.config,
            success,
            output,
            diff_output,
        )

    def run(self, env: Dict[str, Any], lock=None) -> TestResult:
        r = self._run(env)

        if not r.isSuccess():
            self._verbose_print(
                color.RED(self.color, bold=True) + "Failure:" + color.RESET(self.color)
            )
            # if we have a diff, print that. else just print the output
            if r.diff:
                self._verbose_print("  " + "Diff".center(78, "~"))
                lines = open(r.diff.getPath()).readlines()
                for l in lines:
                    self._verbose_print("  " + l.rstrip())
                self._verbose_print("  " + "~" * 78)
            else:
                self._verbose_print("  " + "Output".center(78, "~"))
                lines = open(r.output.getPath()).readlines()
                for l in lines:
                    self._verbose_print("  " + l.rstrip())
                self._verbose_print("  " + "~" * 78)

        self._verbose_print("=" * 80)

        # use lock if available to make coherent printing
        if lock:
            lock.acquire()
        self._dump_log()
        if lock:
            lock.release()

        return r


@dataclass
class TestResult:
    test: TestConfiguration
    success: bool
    output: Union[TestFile, None]
    diff: Union[TestFile, None]

    def name(self, pretty=False, env: Dict[str, Any] = dict()):
        return self.test.name(pretty=pretty, env=env)

    def isSuccess(self):
        return self.success

    def clean(self):
        if self.output:
            self.output.clean()
        if self.diff:
            self.diff.clean()
        if self.test.expected_file:
            self.test.expected_file.clean()
        if self.test.source_file:
            self.test.source_file.clean()
