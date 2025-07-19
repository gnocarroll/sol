#!/usr/bin/env python3

import os
import subprocess
import sys

from pathlib import Path
from typing import Generator, Optional

from .dotenv import read_dotenv


SCRIPT_PATH = Path(os.path.split(__file__)[0])
PROJECT_DIR = SCRIPT_PATH.parent

DOTENV_VARS = read_dotenv(os.path.join(PROJECT_DIR, ".env"))

MAIN_EXE_PATH = os.path.join(
    DOTENV_VARS["BIN_DIR"],
    DOTENV_VARS["MAIN_EXE"],
)

TEST_RESOURCES = "./test_resources"


os.chdir(PROJECT_DIR)

subprocess.run(["make"], check=True)

if not os.path.isdir(TEST_RESOURCES):
    sys.exit(f"directory containing tests {TEST_RESOURCES} not found")


def _file_to_str(path: str) -> Optional[str]:
    try:
        with open(path, "r") as f:
            return f.read()
    except OSError:
        return None

def _listdir_paths(path: str) -> Generator[str]:
    for filename in os.listdir(path):
        yield os.path.join(path, filename)


test_dirs = _listdir_paths(TEST_RESOURCES)
test_count = 0
passed = 0

CODE_FILENAME = "code.txt"
EXPECTED_FILENAME = "output.txt"

for test_dir in test_dirs:
    test_count += 1

    code_path = os.path.join(test_dir, CODE_FILENAME)
    expected_path = os.path.join(test_dir, EXPECTED_FILENAME)

    code_txt = _file_to_str(code_path)
    expected_output = _file_to_str(expected_path)

    if code_txt is None:
        print(f"For test {test_count} failed to read {code_path} as str")
        continue
    if expected_output is None:
        print(f"For test {test_count} failed to read {expected_path} as str")
        continue

    exe_err = False
    actual_output = ""
    ret_code = 0
    
    try:
        proc = subprocess.run(
            [
                MAIN_EXE_PATH,
            ],
            capture_output=True,
            input=code_txt,
            text=True,
            check=True,
        )

        actual_output = proc.stdout
    except subprocess.CalledProcessError as e:
        exe_err = True
        ret_code = e.returncode
        actual_output=e.stdout

    actual_output = actual_output.strip()
    expected_output = expected_output.strip()

    output_correct = (actual_output == expected_output)

    test_passed = (output_correct and not exe_err)

    if test_passed:
        passed += 1
    else:
        print(f"Test {test_count} failed.")

        if exe_err:
            print(f"Executable exited with code {ret_code}")
        if not output_correct:
            print("Expected output ---")
            print(expected_output)
            print("Actual output ---")
            print(actual_output)

print()
print(f"TESTS PASSED: {passed}/{test_count}")