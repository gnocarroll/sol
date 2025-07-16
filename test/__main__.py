#!/usr/bin/env python3

import os
import subprocess

from pathlib import Path

from .dotenv import read_dotenv


SCRIPT_PATH = Path(os.path.split(__file__)[0])
PROJECT_DIR = SCRIPT_PATH.parent

DOTENV_VARS = read_dotenv(os.path.join(PROJECT_DIR, ".env"))

MAIN_EXE_PATH = os.path.join(
    DOTENV_VARS["BIN_DIR"],
    DOTENV_VARS["MAIN_EXE"],
)


os.chdir(PROJECT_DIR)


subprocess.run(["make"], check=True)


tests = [
    ("1", "1"),
    ("1 + 1", "2"),
    ("1 - 1", "0"),
    ("5 * 5", "25"),
    ("10 / 3", "3"),
    ("20 % 3", "2"),
    ("2 ** 3", "8"),
]


test_count = len(tests)
passed = 0


for test_idx, (input, expected_output) in enumerate(tests, start=1):
    exe_err = False
    actual_output = ""
    ret_code = 0
    
    try:
        proc = subprocess.run(
            [
                MAIN_EXE_PATH,
            ],
            capture_output=True,
            input=input,
            text=True,
            check=True,
        )

        actual_output = proc.stdout
    except subprocess.CalledProcessError as e:
        exe_err = True
        ret_code = e.returncode
        actual_output=e.stdout

    output_correct = (actual_output == expected_output)

    test_passed = (output_correct and not exe_err)

    if test_passed:
        passed += 1
    else:
        print(f"Test {test_idx} failed.")

        if exe_err:
            print(f"Executable exited with code {ret_code}")
        if not output_correct:
            print("Expected output ---")
            print(expected_output)
            print("Actual output ---")
            print(actual_output)


print()
print(f"TESTS PASSED: {passed}/{test_count}")