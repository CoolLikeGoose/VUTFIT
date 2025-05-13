<?php

namespace IPP\Student\Memory;

use IPP\Core\ReturnCode;

class Frame 
{
    private const DEFAULT_VALUE = "/@__{ERR_UNDEFINED}__@/:6e64da5117193485afc319e789791b6dsd645jdsy21b543d90b3";

    /** @var array<string, string|int|bool|null> */
    private array $variables = array();

    public function addToFrame(string $varName): void
    {
        if (isset($this->variables[$varName])) 
        {
            exit(ReturnCode::SEMANTIC_ERROR);
        }
        $this->variables[$varName] = self::DEFAULT_VALUE;
    }

    public function setInFrame(string $varName, int|string|bool|null $value): void
    {
        if (!array_key_exists($varName, $this->variables)) 
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        $this->variables[$varName] = $value;
    }

    public function getFromFrame(string $varName): int|string|bool|null
    {
        if (!array_key_exists($varName, $this->variables)) 
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        
        if (is_string($this->variables[$varName]) && ($this->variables[$varName] == self::DEFAULT_VALUE)) {
            exit(ReturnCode::VALUE_ERROR);
        }
        return $this->variables[$varName];
    }
}