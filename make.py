#!/usr/bin/python3

from cgi import test
from code import compile_command
import os
import sys

if len(sys.argv) == 1:
    sys.argv.append("all")

sourceDirectory = "source/"
buildDirectory = "build/"

interpreter = "interpreter/"
tests = "tests/"

def compileInterpreter():
    compileCommand = "clang++ -Wall -Werror -std=c++20 ./" + sourceDirectory + interpreter + "*.cpp -o " + buildDirectory + interpreter + "esolang"
    print("Compiling interpreter... ", end="")
    os.system(compileCommand)
    print("Done.", end="\n")


def compileTests():
    compileCommand = "customasm -q -fbinary " + sourceDirectory + tests + "ruleset.asm " + sourceDirectory + tests
    print(compileCommand)
    print("Compiling tests... ", end="")
    for test in next(os.walk(sourceDirectory+tests))[1]:
        os.system(compileCommand + test + "/*.asm -o" + buildDirectory + tests + test + ".bin")
    print("Done.", end="\n")

if sys.argv[1] == "all":
    compileInterpreter()
    compileTests()
elif sys.argv[1] == "interpreter":
    compileInterpreter()
elif sys.argv[1] == "tests":
    compileTests()
else:
    print("Invalid target \"{0}\"".format(sys.argv[1]))