import subprocess as sp
from typing import List, Tuple


def execute(args: List[str], stdin: str = None) -> Tuple[int, str]:
    cp = sp.run(
        args, stdout=sp.PIPE, stderr=sp.STDOUT, input=stdin, text=True, errors="ignore"
    )
    return (cp.returncode, cp.stdout)
