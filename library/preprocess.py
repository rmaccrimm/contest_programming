"""
A sort of simplified implementation of the C preprocessor whose sole goal is to
include local includes, i.e. those enclosed in "" rather than <>.
"""

from argparse import ArgumentParser
import os
import re


def preprocess(filename: str, defines: set[str] | None = None) -> str:
    if defines is None:
        defines = set()
    try:
        f = open(f"./{filename}")
    except FileNotFoundError:
        f = open(f"../library/{filename}")

    lines = f.read().strip(os.linesep).split(os.linesep)
    f.close()
    
    output = []
    if_stack = []
    skipping_current = False

    for line in lines:
        
        m_ifndef = re.match(r"^#ifndef\s+(\w+).*$", line)
        m_endif = re.match(r"^#endif.*$", line)
        m_define = re.match(r"^#define\s+(\w+).*$", line)
        m_include = re.match(r'^#include\s+"([\w\.]+)".*$', line)

        if m_endif:
            if_stack.pop()
            if not if_stack:
                # Reached the end of the topmost if directive
                skipping_current = False
                continue

        if line.startswith("#if"):
            if_stack.append(0)

        if m_ifndef and len(if_stack) == 1:
            # Only deal with topmost ifndef
            if m_ifndef.group(1) in defines:
                skipping_current = True
            # skip the ifndef line
            continue

        if skipping_current:
            continue

        if m_define and len(if_stack) == 1:
            # Only parse topmost define
            defines.add(m_define.group(1))

        elif m_include:
            include_contents = preprocess(m_include.group(1), defines)
            if not include_contents:
                continue

            output.append(include_contents)
            
        else:
            output.append(line)

    return os.linesep.join(output)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("filename")
    args = parser.parse_args()

    print(preprocess(args.filename))
