# Implementační dokumentace k 1. úloze do IPP 2023/2024 
### Jméno a příjmení: Nikita Smirnov
### Login: xsmirn02

## Overview
The code is organized into several modules, enhancing readability and facilitating maintenance.\
 Attempts have been made to utilize constructs such as Enum, private fields, static classes and methods. However, the implementation of these features in Python looks strange at times based on the programming experience in C#.

### Components

##### debug.py
- The main function is to support debug messages when -v (verbose) is enabled via `Debug` class
- Contains the `ExitCodes` enum to store error codes.

##### instructions.py
- Implements a static class `Instruction` to handle instruction mapping.
- Provides a method `get_types_by_opcode(OpCode)` to retrieve argument types based on OpCode.

- Contains the `Types` enum to store instruction types.

##### xml_builder.py
- Manages the creation of XML output.
- In the implementation used `xml.etree.ElementTree`.
- Defines `add_instruction(self, OpCode, args)` function to add instructions to output XML tree.
- Implements `print_xml_output` function to print the final XML to `sys.stdout`.

##### parse.py
- Entry point of the application.
- Instantiates the `Parser` class and initiates the parsing process by calling `parser.process_code()`.

#### parser.py
- `parser.py` contains the main parsing logic in `Parser` class 

Some explanations about the execution flow of the program:
- The regexPatterns array is related to the Types enumeration. This array contains precompiled regular expressions for certain types(enum values are the indexes in the array), which increases the compactness of the code.
- A private method, `__argument_parse()`, utilizing `argparse` to automatically generate `--help` and prevent incorrect argument usage by users.
- `The __find_header()` function reads the input stream until it finds a `header`, `opcode` or end of line, while ignoring comments and empty lines 
- The `__line_parse()` function recognizes whether there is an opcode in the string using a regular expression and passes it to `__check_opcode_types()` where the opcode itself, the number of arguments and their types are checked.