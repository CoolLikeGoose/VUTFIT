import sys
from enum import Enum

class ExitCodes(Enum):
    wrongArguments = 10
    errorWhileOpenningFileInput = 11
    errorWhileOpenningFileOutput = 12
    noHeaderError = 21
    badOpCode = 22
    sourceCodeError = 23
    internalError = 99


class Debug():
    def __init__(self, verbose):
        self.verb = verbose
        if verbose:
            print("[D]: Verbose mode is on")
    
    
    def log(self, message):
        if self.verb:
            print(f"[D]: {message}", file=sys.stderr)