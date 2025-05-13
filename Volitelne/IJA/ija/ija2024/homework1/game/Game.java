package ija.ija2024.homework1.game;

import ija.ija2024.homework1.common.GameNode;
import ija.ija2024.homework1.common.NodeType;
import ija.ija2024.homework1.common.Position;
import ija.ija2024.homework1.common.Side;

public class Game {
    private final GameNode[][] _grid;
    private boolean _isPowerPlaced = false;

    private final int _rows;
    public int rows() {
        return _rows;
    }

    private final int _cols;
    public int cols() {
        return _cols;
    }

    private Game(int rows, int cols) {
        _rows = rows;
        _cols = cols;

        _grid = new GameNode[_rows][_cols];
    }

    public static Game create(int rows, int cols) {
        if (rows <= 0 || cols <= 0) {
            throw new IllegalArgumentException("Bad grid sizes");
        }

        return new Game(rows, cols);
    }

    public GameNode createLinkNode(Position p, Side... sides) {
        if (invalidPosition(p) || sides.length < 2) return null;

        GameNode node = new GameNode();
        node.setType(NodeType.Link);
        node.setPosition(p);
        node.connectSides(sides);

        placeNode(p, node);
        return node;
    }

    public GameNode createPowerNode(Position p, Side... sides) {
        if (invalidPosition(p) || _isPowerPlaced || sides.length == 0) return null;

        GameNode node = new GameNode();
        node.setType(NodeType.Power);
        node.setPosition(p);
        node.connectSides(sides);

        placeNode(p, node);
        _isPowerPlaced = true;
        return node;
    }

    public GameNode createBulbNode(Position p, Side... sides) {
        if (invalidPosition(p) || sides.length != 1) return null;

        GameNode node = new GameNode();
        node.setType(NodeType.Bulb);
        node.setPosition(p);
        node.connectSides(sides);

        placeNode(p, node);
        _isPowerPlaced = true;
        return node;
    }


    public GameNode node(Position position) {
        if (invalidPosition(position)) return null;

        if (_grid[position.getRow()-1][position.getCol()-1] == null) {
            return new GameNode();
        }

        return _grid[position.getRow()-1][position.getCol()-1];
    }

    private void placeNode(Position position, GameNode node) {
        _grid[position.getRow()-1][position.getCol()-1] = node;
    }

    private boolean invalidPosition(Position position) {
        return position.getRow() <= 0 ||
                position.getCol() <= 0 ||
                position.getCol() > _cols ||
                position.getRow() > _rows;
    }
}
