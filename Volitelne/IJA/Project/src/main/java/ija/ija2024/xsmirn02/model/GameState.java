package ija.ija2024.xsmirn02.model;
//Author : Smirnov Nikita : xsmirn02. Manages main logic of game`s Status
import ija.ija2024.xsmirn02.model.common.GameNode;
import ija.ija2024.xsmirn02.model.common.RotateCommand;
import ija.ija2024.xsmirn02.model.common.enums.Difficulty;
import java.util.ArrayList;
import java.util.List;
import java.util.Stack;
import java.time.LocalDateTime;

import ija.ija2024.xsmirn02.model.common.interfaces.Command;
import ija.ija2024.xsmirn02.model.common.interfaces.Observable;
import ija.ija2024.xsmirn02.util.Logger;

public class GameState implements Observable.Observer, Observable {
    private final GameBoard gameBoard;
    private Observer observer;

    public final Stack<Command> undoStack = new Stack<>();
    public final Stack<Command> redoStack = new Stack<>();

    private final int size;
    private final Difficulty difficulty;

    private final List<GameNode> bulbs = new ArrayList<>();

    public GameState(Difficulty difficulty) {
        Logger.getInstance().log("--------------------Game initialized at [" + LocalDateTime.now() + "]--------------------");

        this.difficulty = difficulty;
        Logger.getInstance().log("Difficulty: " + difficulty.toString());

        this.size = 5 + 2 * difficulty.ordinal();
        Logger.getInstance().log("Board size: " + size);

        int bulbCount = this.size - 4;

        this.gameBoard = new GameBoard(size);
        this.gameBoard.generateBoard(bulbCount);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                GameNode node = gameBoard.getNode(i, j);
                node.registerObserver(this);
                if (node.isBulb()) {
                    bulbs.add(node);
                }
            }
        }

        handleConnectionUpdate();
    }

    public GameBoard getGameBoard() {
        return gameBoard;
    }

    @Override
    public void update(Observable observable) {
        if (observable instanceof GameNode node) {
            if (!node.autoMove) {
                redoStack.clear();
                undoStack.push(new RotateCommand(node));
                notifyObserver();
            }
            handleConnectionUpdate();
        }
    }

    private void handleConnectionUpdate() {
        gameBoard.updateConnections(gameBoard.getPowerNodePosition());
        if (isGameComplete()) {
            notifyObserver();
        }
    }

    public boolean isGameComplete() {
        int litBulbsCount = 0;
        for (GameNode bulb : bulbs) {
            litBulbsCount += bulb.isPowered() ? 1 : 0;
        }

        return litBulbsCount == bulbs.size();
    }

    @Override
    public void registerObserver(Observer var1) {
        observer = var1;
    }

    @Override
    public void removeObserver(Observer var1) {
        observer = null;
    }

    @Override
    public void notifyObserver() {
        observer.update(this);
    }

    //Command
    public boolean stepForward() {
        Command command = redoStack.pop();
        command.execute();
        undoStack.push(command);

        Logger.getInstance().log("REDO: " + command.toString());

        return !redoStack.isEmpty();
    }

    public boolean stepBack() {
        Command command = undoStack.pop();
        command.undo();
        redoStack.push(command);

        Logger.getInstance().log("UNDO: " + command.toString());

        return !undoStack.isEmpty();
    }
}