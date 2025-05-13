<?php

namespace IPP\Student\Memory;

use IPP\Core\ReturnCode;

class Stack 
{
    /** @var array<int|string|bool|null> */
    private array $stack;

    final public function __construct() 
    {
        $this->stack = [];
    }

    public function push(int|string|bool|null $value): void 
    {
        array_push($this->stack, $value);
    }

    public function pop(): int|string|bool|null 
    {
        $result = array_pop($this->stack);
        if ($result == null) 
        {
            exit(ReturnCode::VALUE_ERROR);
        }
        return $result;
    }
}