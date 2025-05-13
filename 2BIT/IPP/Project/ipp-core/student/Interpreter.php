<?php

namespace IPP\Student;

use IPP\Core\AbstractInterpreter;
use IPP\Core\ReturnCode;
use IPP\Student\FlowController;
use IPP\Student\Instructions\Instruction;
use IPP\Student\Memory\DataController;

class Interpreter extends AbstractInterpreter
{
    public function execute(): int
    {
        //Set up Initial settings
        $dataController = new DataController($this->input, $this->stdout, $this->stderr);
        Instruction::initInstructions($dataController);

        //Get instructions
        $flowController = new FlowController;
        $instructions = $this->source->getDOMDocument()->getElementsByTagName("instruction");

        //Sort by order
        $instructionsWithOrder = [];
        foreach ($instructions as $instruction) {
            $order = $instruction->getAttribute("order");
            $instructionsWithOrder[] = ['order' => $order, 'instruction' => $instruction];
        }

        usort($instructionsWithOrder, function($a, $b) {
            return $a['order'] <=> $b['order'];
        });

        //Save instructions in flowController
        $prevOrder = -1;
        foreach ($instructionsWithOrder as $item) {
            $order = $item['order'];
            $instruction = $item['instruction'];

            if ($order < 0 || $prevOrder == $order) exit(ReturnCode::INVALID_SOURCE_STRUCTURE);

            $flowController->saveInstruction($instruction);
            $prevOrder = $order;
        }

        //execute program;
        return $flowController->execute();
    }
}
