<?php

namespace IPP\Student\Instructions;

use DOMElement;
use IPP\Core\ReturnCode;
use IPP\Student\Memory\DataController;

abstract class Instruction
{
    protected static DataController $__dataController;

    public static function initInstructions(DataController $dataController): void
    {
        static::$__dataController = $dataController;
    }

    //Internal functions for faster DOMElement parsing
    protected static function getArgValue(DOMElement $element, int $argNum): string
    {
        /** @var DOMElement|null $arg */
        $arg = $element->getElementsByTagName("arg".$argNum)->item(0);
        if ($arg !== null && $arg->nodeValue !== null) {
            return trim($arg->nodeValue);
        }
        return "";
    }

    protected static function getArgType(DOMElement $element, int $argNum): int
    {
        /** @var DOMElement|null $arg */
        $arg = $element->getElementsByTagName("arg".$argNum)->item(0);
        if ($arg !== null) {
            return Types::getSymbType($arg->getAttribute("type"));
        }
        return Types::ANY_type;
    }

    //Convert string-value from getArgValue to corresponding type in php
    protected static function castStringToVar(string $var, int $type): int|string|bool|null
    {
        if ($type == Types::INT_type) {
            if (ctype_digit($var)) return (int)$var;
        } else if ($type == Types::STRING_type) {
            return static::$__dataController->parseString($var); 
        } else if ($type == Types::BOOL_type) {
            if ($var == "true") return true;
            if ($var == "false") return false;
        } else {
            if ($var == "nil") return null;
        }

        exit(ReturnCode::OPERAND_TYPE_ERROR);
    }

    abstract public function execute(): int;

    public static function parse(DOMElement $instruction, int $instructionNumber): Instruction 
    {
        switch (strtoupper($instruction->getAttribute("opcode"))) 
        {
            case "MOVE":
                return new MOVEInstruction($instruction);
            case "CREATEFRAME":
                return new CREATEFRAMEInstruction;
            case "PUSHFRAME":
                return new PUSHFRAMEInstruction;
            case "POPFRAME":
                return new POPFRAMEInstruction;
            case "DEFVAR":
                return new DEFVARInstruction($instruction);
            case "CALL":
                return new CALLInstruction($instruction, $instructionNumber);
            case "RETURN":
                return new RETURNInstruction;
            case "ADD":
                return new ADDInstruction($instruction);
            case "SUB":
                return new SUBInstruction($instruction);
            case "MUL":
                return new MULInstruction($instruction);
            case "IDIV":
                return new IDIVInstruction($instruction);
            case "LT":
                return new LTInstruction($instruction);
            case "GT":
                return new GTInstruction($instruction);
            case "EQ":
                return new EQInstruction($instruction);
            case "AND":
                return new ANDInstruction($instruction);
            case "OR":
                return new ORInstruction($instruction);
            case "NOT":
                return new NOTInstruction($instruction);
            case "INT2CHAR":
                return new INT2CHARInstruction($instruction);
            case "STRI2INT":
                return new STRI2INTInstruction($instruction);
            case "READ":
                return new READInstruction($instruction);
            case "WRITE":
                return new WRITEInstruction($instruction);
            case "CONCAT":
                return new CONCATInstruction($instruction);
            case "STRLEN":
                return new STRLENInstruction($instruction);
            case "GETCHAR":
                return new GETCHARInstruction($instruction);
            case "SETCHAR":
                return new SETCHARInstruction($instruction);
            case "TYPE":
                return new TYPEInstruction($instruction);
            case "LABEL":
                return new LABELInstruction($instruction, $instructionNumber);
            case "JUMP":
                return new JUMPInstruction($instruction);
            case "JUMPIFEQ":
                return new JUMPIFEQInstruction($instruction);
            case "JUMPIFNEQ":
                return new JUMPIFNEQInstruction($instruction);
            case "EXIT":
                return new EXITInstruction($instruction);
            case "DPRINT":
                return new DPRINTInstruction();
            case "BREAK":
                return new BREAKInstruction;
            case "PUSHS":
                return new PUSHSInstruction($instruction);
            case "POPS":
                return new POPSInstruction($instruction);
            default:
                return new UnknownInstruction;
        }
    }

}

//I tried to move the instructions to a separate file,
//but composer could not find these classes, but in the same time PHPStan recognized them.
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//                      INSTRUCTIONS                        //
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////


//Working with memory frames, calling functions
class MOVEInstruction extends Instruction
{
    private string $varName;
    private string $symb;
    private int $symbType;


    final public function __construct(DOMElement $instruction) 
    {
        $this->varName = static::getArgValue($instruction, 1);
        $this->symb = static::getArgValue($instruction, 2);
        $this->symbType = static::getArgType($instruction, 2);
    }

    public function execute(): int 
    {
        $var = $this->symb;

        if ($this->symbType == Types::VAR_type) {
            $var = static::$__dataController->getVar($this->symb);
        } else {
            $var = static::castStringToVar($var, $this->symbType);
        }

        static::$__dataController->setVar($this->varName, $var);

        return JumpCodes::NEXT;
    }
}

class CREATEFRAMEInstruction extends Instruction
{
    final public function __construct() {}

    public function execute(): int 
    {
        static::$__dataController->createFrame();
        return JumpCodes::NEXT;
    }
}

class PUSHFRAMEInstruction extends Instruction
{
    final public function __construct() {}

    public function execute(): int 
    {
        static::$__dataController->pushFrame();
        return JumpCodes::NEXT;
    }
}

class POPFRAMEInstruction extends Instruction
{
    final public function __construct() {}

    public function execute(): int 
    {
        static::$__dataController->popFrame();
        return JumpCodes::NEXT;
    }
}

class DEFVARInstruction extends Instruction
{
    private string $varName;

    final public function __construct(DOMElement $instruction) 
    {
        $this->varName = static::getArgValue($instruction, 1);
    }

    public function execute(): int 
    {
        static::$__dataController->newVar($this->varName);
        return JumpCodes::NEXT;
    }
}

class CALLInstruction extends Instruction
{
    private string $labelName;
    private int $line;

    final public function __construct(DOMElement $instruction, int $line) 
    {
        $this->line = $line;
        $this->labelName = static::getArgValue($instruction, 1);
    }

    public function execute(): int 
    {
        static::$__dataController->saveCall($this->line+1);
        return static::$__dataController->getLabel($this->labelName);
    }
}

class RETURNInstruction extends Instruction
{
    final public function __construct() {}

    public function execute(): int 
    {
        return static::$__dataController->getCall();
    }
}

//Arithmetic, relational, boolean and conversion instructions
class ADDInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, Types::INT_type);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, Types::INT_type);
        }

        if (!is_int($firstOperand) || !is_int($secondOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, $firstOperand + $secondOperand);
        return JumpCodes::NEXT;
    }
}

class SUBInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, Types::INT_type);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, Types::INT_type);
        }

        if (!is_int($firstOperand) || !is_int($secondOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, $firstOperand - $secondOperand);
        return JumpCodes::NEXT;
    }
}

class MULInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, Types::INT_type);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, Types::INT_type);
        }

        if (!is_int($firstOperand) || !is_int($secondOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, $firstOperand * $secondOperand);
        return JumpCodes::NEXT;
    }
}

class IDIVInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, Types::INT_type);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, Types::INT_type);
        }

        if ($secondOperand == 0) exit(ReturnCode::OPERAND_VALUE_ERROR);
        if (!is_int($firstOperand) || !is_int($secondOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, $firstOperand / $secondOperand);
        return JumpCodes::NEXT;
    }
}

class LTInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, $this->firstType);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, $this->secondType);
        }

        if (Types::getVarType($firstOperand) !== Types::getVarType($secondOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);
        
        static::$__dataController->setVar($this->var, $firstOperand < $secondOperand);
        return JumpCodes::NEXT;
    }
}

class GTInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, $this->firstType);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, $this->secondType);
        }

        if (Types::getVarType($firstOperand) !== Types::getVarType($secondOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, $firstOperand > $secondOperand);
        return JumpCodes::NEXT;
    }
}

class EQInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;
        
        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, $this->firstType);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, $this->secondType);
        }

        if (Types::getVarType($firstOperand) !== Types::getVarType($secondOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, $firstOperand == $secondOperand);
        return JumpCodes::NEXT;
    }
}

class ANDInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, Types::BOOL_type);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, Types::BOOL_type);
        }

        if ((Types::getVarType($firstOperand) !== Types::BOOL_type) ||
            (Types::getVarType($secondOperand) !== Types::BOOL_type)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, $firstOperand && $secondOperand);
        return JumpCodes::NEXT;
    }
}

class ORInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, Types::BOOL_type);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, Types::BOOL_type);
        }

        if ((Types::getVarType($firstOperand) !== Types::BOOL_type) ||
            (Types::getVarType($secondOperand) !== Types::BOOL_type)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, $firstOperand || $secondOperand);
        return JumpCodes::NEXT;
    }
}

class NOTInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, Types::BOOL_type);
        }

        if ((Types::getVarType($firstOperand) !== Types::BOOL_type)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, !$firstOperand);
        return JumpCodes::NEXT;
    }
}

class INT2CHARInstruction extends Instruction
{
    private string $var;
    private string $chr;
    private int $chrType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);
        $this->chr = static::getArgValue($instruction, 2);
        $this->chrType = static::getArgType($instruction, 2);
    }

    public function execute(): int 
    {
        $chrOperand = $this->chr;

        if ($this->chrType == Types::VAR_type) {
            $chrOperand = static::$__dataController->getVar($this->chr);
        } else {
            $chrOperand = static::castStringToVar($chrOperand, Types::INT_type);
        }

        if (!is_int($chrOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        $actualChr = mb_chr($chrOperand);
        if (!$actualChr) exit(ReturnCode::STRING_OPERATION_ERROR);
        
        static::$__dataController->setVar($this->var, $actualChr);
        return JumpCodes::NEXT;
    }
}

class STRI2INTInstruction extends Instruction
{
    private string $var;
    private string $line;
    private int $lineType;
    private string $position;
    private int $positionType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);
        $this->line = static::getArgValue($instruction, 2);
        $this->lineType = static::getArgType($instruction, 2);
        $this->position = static::getArgValue($instruction, 3);
        $this->positionType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $lineOperand = $this->line;

        if ($this->lineType == Types::VAR_type) {
            $lineOperand = static::$__dataController->getVar($this->line);
        } else {
            $lineOperand = static::castStringToVar($lineOperand, Types::STRING_type);
        }

        $positionOperand = $this->position;

        if ($this->positionType == Types::VAR_type) {
            $positionOperand = static::$__dataController->getVar($this->position);
        } else {
            $positionOperand = static::castStringToVar($positionOperand, Types::INT_type);
        }

        if (!is_string($lineOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);
        if (!is_int($positionOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);
        if (strlen($lineOperand)-1 <= $positionOperand) exit(ReturnCode::STRING_OPERATION_ERROR);

        $actualChr = mb_ord($lineOperand[$positionOperand]);
        if (!$actualChr) exit(ReturnCode::STRING_OPERATION_ERROR);
        
        static::$__dataController->setVar($this->var, $actualChr);
        return JumpCodes::NEXT;
    }
}

//Input-output instructions
class READInstruction extends Instruction
{
    private string $var;
    private string $type;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);
        $this->type = static::getArgValue($instruction, 2);
    }

    public function execute(): int 
    {
        $varReceived = null;
        if ($this->type == "bool") $varReceived = static::$__dataController->__input->readBool(); 
        if ($this->type == "int") $varReceived = static::$__dataController->__input->readInt(); 
        if ($this->type == "string") $varReceived = static::$__dataController->__input->readString(); 

        static::$__dataController->setVar($this->var, $varReceived);
        return JumpCodes::NEXT;
    }
}

class WRITEInstruction extends Instruction
{
    private string $arg1;
    private int $type;

    final public function __construct(DOMElement $instruction) 
    {
        $this->arg1 = static::getArgValue($instruction, 1);
        $this->type = static::getArgType($instruction, 1);
    }

    public function execute(): int 
    {
        $var = 0;
        if ($this->type == Types::VAR_type) {
            $var = static::$__dataController->getVar($this->arg1);
        } else {
            $var = static::castStringToVar($this->arg1, $this->type);
        }

        if (is_int($var)) static::$__dataController->__stdout->writeInt($var);
        if (is_string($var)) static::$__dataController->__stdout->writeString($var);
        if (is_bool($var)) static::$__dataController->__stdout->writeBool($var);
        return JumpCodes::NEXT;
    }
}

//Working with strings
class CONCATInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, Types::STRING_type);
        }

        $secondOperand = $this->secondSymb;
        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, Types::STRING_type);
        }

        if ((!is_string($firstOperand)) || (!is_string($secondOperand))) exit(ReturnCode::OPERAND_TYPE_ERROR);

        static::$__dataController->setVar($this->var, $firstOperand.$secondOperand);
        return JumpCodes::NEXT;
    }
}

class STRLENInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);  
    }

    public function execute(): int 
    {
        $firstOperand = "";
        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, Types::STRING_type);
        }

        if (!is_string($firstOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);
        static::$__dataController->setVar($this->var, strlen($firstOperand));
        return JumpCodes::NEXT;
    }
}

class GETCHARInstruction extends Instruction
{
    private string $var;
    private string $line;
    private int $lineType;
    private string $position;
    private int $positionType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);
        $this->line = static::getArgValue($instruction, 2);
        $this->lineType = static::getArgType($instruction, 2);
        $this->position = static::getArgValue($instruction, 3);
        $this->positionType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $lineOperand = $this->line;
        if ($this->lineType == Types::VAR_type) {
            $lineOperand = static::$__dataController->getVar($this->line);
        } else {
            $lineOperand = static::castStringToVar($lineOperand, Types::STRING_type);
        }

        $positionOperand = $this->position;

        if ($this->positionType == Types::VAR_type) {
            $positionOperand = static::$__dataController->getVar($this->position);
        } else {
            $positionOperand = static::castStringToVar($positionOperand, Types::INT_type);
        }

        if (!is_string($lineOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);
        if (!is_int($positionOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);
        if (strlen($lineOperand) <= $positionOperand) exit(ReturnCode::STRING_OPERATION_ERROR);
        
        static::$__dataController->setVar($this->var, $lineOperand[$positionOperand]);
        return JumpCodes::NEXT;
    }
}

class SETCHARInstruction extends Instruction
{
    private string $var;
    private string $chr;
    private int $chrType;
    private string $position;
    private int $positionType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);
        $this->chr = static::getArgValue($instruction, 3);
        $this->chrType = static::getArgType($instruction, 3);
        $this->position = static::getArgValue($instruction, 2);
        $this->positionType = static::getArgType($instruction, 2);
    }

    public function execute(): int 
    {
        $chrOperand = $this->chr;

        if ($this->chrType == Types::VAR_type) {
            $chrOperand = static::$__dataController->getVar($this->chr);
        } else {
            $chrOperand = static::castStringToVar($chrOperand, Types::STRING_type);
        }

        $positionOperand = $this->position;

        if ($this->positionType == Types::VAR_type) {
            $positionOperand = static::$__dataController->getVar($this->position);
        } else {
            $positionOperand = static::castStringToVar($positionOperand, Types::INT_type);
        }

        $str = static::$__dataController->getVar($this->var);

        if (!is_string($chrOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);
        if (!is_string($str)) exit(ReturnCode::OPERAND_TYPE_ERROR);
        if (!is_int($positionOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);
        if (strlen($chrOperand) == 0) exit(ReturnCode::STRING_OPERATION_ERROR);
        if (strlen($str)-1 <= $positionOperand) exit(ReturnCode::STRING_OPERATION_ERROR);
        
        $str[$positionOperand] = $chrOperand;

        static::$__dataController->setVar($this->var, $str);
        return JumpCodes::NEXT;
    }
}

//Working with types
class TYPEInstruction extends Instruction
{
    private string $var;
    private string $firstSymb;
    private int $firstType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->var = static::getArgValue($instruction, 1);        
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);  
    }

    //TODO: add null handling
    public function execute(): int 
    {
        $firstOperand = "";
        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            static::$__dataController->setVar($this->var, Types::getTypeName($this->firstType));
            return JumpCodes::NEXT;
        }

        static::$__dataController->setVar($this->var, Types::getTypeName(Types::getVarType($firstOperand)));
        return JumpCodes::NEXT;
    }
}

//Instructions for program flow control
class LABELInstruction extends Instruction
{
    private string $label;
    private int $line;

    final public function __construct(DOMElement $instruction, int $line) 
    {
        $this->label = static::getArgValue($instruction, 1);     
        $this->line = $line; 
        static::$__dataController->saveLabel($this->label, $this->line);
    }

    public function execute(): int 
    {
        return JumpCodes::NEXT;
    }
}

class JUMPInstruction extends Instruction
{
    private string $labelName;

    final public function __construct(DOMElement $instruction) 
    {
        $this->labelName = static::getArgValue($instruction, 1);
    }

    public function execute(): int 
    {
        return static::$__dataController->getLabel($this->labelName);
    }
}

class JUMPIFEQInstruction extends Instruction
{
    private string $labelName;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->labelName = static::getArgValue($instruction, 1);
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, $this->firstType);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, $this->secondType);
        }

        //TODO: add nil handler
        if (Types::getVarType($firstOperand) !== Types::getVarType($secondOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        if ($firstOperand == $secondOperand) {
            return static::$__dataController->getLabel($this->labelName);
        }

        return JumpCodes::NEXT;
    }
}

class JUMPIFNEQInstruction extends Instruction
{
    private string $labelName;
    private string $firstSymb;
    private int $firstType;
    private string $secondSymb;
    private int $secondType;

    final public function __construct(DOMElement $instruction) 
    {
        $this->labelName = static::getArgValue($instruction, 1);
        $this->firstSymb = static::getArgValue($instruction, 2);       
        $this->firstType = static::getArgType($instruction, 2);       
        $this->secondSymb = static::getArgValue($instruction, 3);
        $this->secondType = static::getArgType($instruction, 3);
    }

    public function execute(): int 
    {
        $firstOperand = $this->firstSymb;
        $secondOperand = $this->secondSymb;

        if ($this->firstType == Types::VAR_type) {
            $firstOperand = static::$__dataController->getVar($this->firstSymb);
        } else {
            $firstOperand = static::castStringToVar($firstOperand, $this->firstType);
        }

        if ($this->secondType == Types::VAR_type) {
            $secondOperand = static::$__dataController->getVar($this->secondSymb);
        } else {
            $secondOperand = static::castStringToVar($secondOperand, $this->secondType);
        }

        //TODO: add nil handler
        if (Types::getVarType($firstOperand) !== Types::getVarType($secondOperand)) exit(ReturnCode::OPERAND_TYPE_ERROR);

        if ($firstOperand != $secondOperand) {
            return static::$__dataController->getLabel($this->labelName);
        }

        return JumpCodes::NEXT;
    }
}

class EXITInstruction extends Instruction
{
    private int $exitCode;

    final public function __construct(DOMElement $instruction) 
    {
        /** @var DOMElement|null $element */
        $element = $instruction->getElementsByTagName("arg1")->item(0);
        if ($element !== null) {
            $code = $element->nodeValue ?? '';

            if (ctype_digit($code) && $code >= '0' && $code <= '9') {
                $this->exitCode = (int)$code;
                return;
            }
            exit(ReturnCode::OPERAND_VALUE_ERROR);
        }
    }

    public function execute(): int 
    {
        return -($this->exitCode+1);
    }
}

//Debug instructions
class DPRINTInstruction extends Instruction
{
    // private string $arg1;

    final public function __construct() 
    {
        // $this->arg1 = $instruction->getAttribute("arg1");
    }

    public function execute(): int 
    {
        // static::$__dataController->__stdout->writeString($this->arg1);
        return JumpCodes::NEXT;
    }
}

class BREAKInstruction extends Instruction
{
    final public function __construct() 
    {
        // throw new NotImplementedException();
    }

    public function execute(): int 
    {
        return JumpCodes::NEXT;
    }
}


//Working with the data stack
class PUSHSInstruction extends Instruction
{
    private string $symb;
    private int $symbType;


    final public function __construct(DOMElement $instruction) 
    {
        $this->symb = static::getArgValue($instruction, 1);
        $this->symbType = static::getArgType($instruction, 1);
    }

    public function execute(): int 
    {
        $var = $this->symb;

        if ($this->symbType == Types::VAR_type) {
            $var = static::$__dataController->getVar($this->symb);
        } else {
            $var = static::castStringToVar($var, $this->symbType);
        }

        static::$__dataController->pushStack($var);
        return JumpCodes::NEXT;
    }
}

class POPSInstruction extends Instruction
{
    private string $varName;


    final public function __construct(DOMElement $instruction) 
    {
        $this->varName = static::getArgValue($instruction, 1);
    }

    public function execute(): int 
    {

        static::$__dataController->setVar($this->varName, static::$__dataController->popStack());
        return JumpCodes::NEXT;
    }
}

//Unknown instruction
class UnknownInstruction extends Instruction
{
    public function execute(): int 
    {
        return 0;
    }
}