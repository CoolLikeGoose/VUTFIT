<?php

namespace IPP\Student\Memory;

use IPP\Core\ReturnCode;

class FramedHeap 
{
    private Frame $globalFrame;
    private ?Frame $temporaryFrame;

    /** @var Frame[] */
    private array $frameStack;

    final public function __construct() 
    {
        $this->globalFrame = new Frame;
        $this->frameStack = [];
    }

    //Frames
    private function getFrame(int $frameIndex): Frame 
    {
        if ($frameIndex == FrameTypes::LF) {
            if (!end($this->frameStack)) {
                exit(ReturnCode::FRAME_ACCESS_ERROR);
            }
            return end($this->frameStack);
        }
        else if ($frameIndex == FrameTypes::TF) {
            if (!isset($this->temporaryFrame)) {
                exit(ReturnCode::FRAME_ACCESS_ERROR);
            }
            return $this->temporaryFrame;
        }
        return $this->globalFrame;
    }

    public function createFrame(): void {
        $this->temporaryFrame = new Frame;
    }

    public function pushFrame(): void {
        if (!isset($this->temporaryFrame)) 
        {
            exit(ReturnCode::FRAME_ACCESS_ERROR);
        }

        array_push($this->frameStack, $this->temporaryFrame);
        $this->temporaryFrame = null;
    }

    public function popFrame(): void {
        if (null == end($this->frameStack)) 
        {
            exit(ReturnCode::FRAME_ACCESS_ERROR);
        }
        $this->temporaryFrame = array_pop($this->frameStack);
    }

    //Vars
    public function newVar(string $varName, int $frameType): void 
    {
        $this->getFrame($frameType)->addToFrame($varName);
    }

    public function setVar(string $varName, int|string|bool|null $value, int $frameType): void
    {
        $this->getFrame($frameType)->setInFrame($varName, $value);
    }

    public function getVar(string $varName, int $frameType): int|string|bool|null
    {
        return $this->getFrame($frameType)->getFromFrame($varName);
    }
}