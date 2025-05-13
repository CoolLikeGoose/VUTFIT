package ija.ija2024.homework2.game;

import ija.ija2024.homework2.common.GameNode;
import ija.ija2024.homework2.common.NodeType;
import ija.ija2024.homework2.common.Position;
import ija.ija2024.homework2.common.Side;
import ija.ija2024.tool.common.ToolEnvironment;
import ija.ija2024.tool.common.ToolField;

public class Game implements ToolEnvironment {
    private final GameNode[][] _grid;
    private boolean _isPowerPlaced = false;
//    private

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

    public void init() {
        for (int row = 0; row < _rows; row++) {
            for (int col = 0; col < _cols; col++) {
                GameNode current = _grid[row][col];
                if (current == null) continue;

                if (row > 0 && _grid[row-1][col] != null) {
                    current.setNeighbor(Side.NORTH, _grid[row-1][col]);
                    _grid[row-1][col].setNeighbor(Side.SOUTH, current);
                }

                if (col > 0 && _grid[row][col-1] != null) {
                    current.setNeighbor(Side.WEST, _grid[row][col-1]);
                    _grid[row][col-1].setNeighbor(Side.EAST, current);
                }
            }
        }

        for (int row = 0; row < _rows; row++) {
            for (int col = 0; col < _cols; col++) {
                if (_grid[row][col] != null && _grid[row][col].isPower()) {
                    _grid[row][col].updatePowerState();
                }
            }
        }
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
        return node;
    }

    public GameNode node(Position position) {
        if (invalidPosition(position)) return null;

        if (_grid[position.getRow()-1][position.getCol()-1] == null) {
            return new GameNode();
        }

        return _grid[position.getRow()-1][position.getCol()-1];
    }

    @Override
    public ToolField fieldAt(int i, int i1) {
        return node(new Position(i, i1));
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