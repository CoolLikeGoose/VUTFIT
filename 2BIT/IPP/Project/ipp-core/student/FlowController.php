<?php

namespace IPP\Student;

use DOMElement;
use IPP\Student\Instructions\Instruction;
use IPP\Student\Instructions\JumpCodes;
use IPP\Student\Instructions;


class FlowController 
{
    /** @var Instruction[] */
    private array $instructions = [];
    private int $curInstruction = 0;

    final public function __construct() {}

    //pre-compile instructions
    public function saveInstruction(DOMElement $instruction): void
    {
        $this->instructions[$this->curInstruction] = Instruction::parse($instruction, $this->curInstruction);
        $this->curInstruction++;
    }

    //Go through the program 
    public function execute(): int 
    {
        if (count($this->instructions) == 0) return 0;

        $index = 0;
        while (true) 
        {
            $jump = $this->instructions[$index]->execute(); 

            if ($jump == JumpCodes::NEXT) $index++;
            else if ($jump < 0) return (-$jump)-1;
            else $index = $jump;
            if ($index == count($this->instructions)) break;
        }

        return 0;
    }
}