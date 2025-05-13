<?php

namespace IPP\Student\Memory;

use IPP\Core\Interface\InputReader;
use IPP\Core\Interface\OutputWriter;
use IPP\Core\ReturnCode;
use IPP\Student\Instructions\Instruction;
use IPP\Student\Instructions\Types;

class DataController 
{
    public InputReader $__input;
    public OutputWriter $__stdout;
    public OutputWriter $__stderr;

    private FramedHeap $framedHeap;
    private Stack $stack;

    /** @var array<string, int> */
    private array $labels;
    /** @var int[] */
    private array $callStack;

    final public function __construct(InputReader $input, OutputWriter $stdout, OutputWriter $stderr) 
    {
        $this->__input = $input;
        $this->__stdout = $stdout;
        $this->__stderr = $stderr;
        $this->framedHeap = new FramedHeap;
        $this->stack = new Stack;
        $this->labels = [];
        $this->callStack = [];
    }

    public static function parseString(string $line): string
    {
        return (string) preg_replace_callback('/\\\\(\d{3})/', function ($matches) {
            return chr((int) $matches[1]); 
        }, $line);
    }

    //Labels
    public function saveLabel(string $labelName, int $line): void 
    {
        if (isset($this->labels[$labelName])) {
            exit(ReturnCode::SEMANTIC_ERROR);
        }

        $this->labels[$labelName] = $line+1;
    }

    public function getLabel(string $labelName): int 
    {
        if (!isset($this->labels[$labelName])) {
            exit(ReturnCode::SEMANTIC_ERROR);
        }

        return $this->labels[$labelName];
    }

    public function saveCall(int $line): void 
    {
        array_push($this->callStack, $line+1);
    }

    public function getCall(): int 
    {
        $lastCall = array_pop($this->callStack);
        if ($lastCall == null) {
            exit(ReturnCode::VALUE_ERROR);
        }
        
        return $lastCall;
    }

    //Memory control [Frames]
    public function createFrame(): void {
        $this->framedHeap->createFrame();
    }

    public function pushFrame(): void {
        $this->framedHeap->pushFrame();
    }

    public function popFrame(): void {
        $this->framedHeap->popFrame();
    }

    //Memory control [FramedHeap]
    /**
    * @return array{0: int, 1: string}
    */
    public static function parseVar(string $varName): array
    {
        if (preg_match('/(TF|LF|GF)@(.+)/', $varName, $matches)) {
            $frameType = FrameTypes::getFrameType($matches[1]);
            $varName = $matches[2];
            
            return [$frameType, $varName];
        } else {
            //Not valid variable
            return [0, ""];
        }
    }

    public function newVar(string $varName): void 
    {
        [$frameType, $varName] = static::parseVar($varName);
        $this->framedHeap->newVar($varName, $frameType);
    }

    public function setVar(string $varName, int|string|bool|null $value): void
    {
        [$frameType, $varName] = static::parseVar($varName);
        $this->framedHeap->setVar($varName, $value, $frameType);
    }

    public function getVar(string $varName): int|string|bool|null
    {
        [$frameType, $varName] = static::parseVar($varName);
        return $this->framedHeap->getVar($varName, $frameType);
    }

    //Memory control [Stack]

    public function pushStack(int|string|bool|null $value): void
    {
        $this->stack->push($value);
    }

    public function popStack(): int|string|bool|null
    {
        return $this->stack->pop();
    }
}