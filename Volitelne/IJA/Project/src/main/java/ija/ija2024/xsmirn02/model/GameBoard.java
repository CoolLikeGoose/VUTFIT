package ija.ija2024.xsmirn02.model;

//Author : Smirnov Nikita : xsmirn02. Manages current status of the gaming board
import ija.ija2024.xsmirn02.model.common.GameNode;
import ija.ija2024.xsmirn02.model.common.Position;
import ija.ija2024.xsmirn02.model.common.enums.Side;
import ija.ija2024.xsmirn02.model.common.enums.NodeType;
import javafx.scene.layout.Pane;

import java.util.*;

public class GameBoard extends Pane {
    private final double tileSize;
    private final double boardSize;
    private final int size;

    private final GameNode[][] nodes;
    private GameNode powerNode;

    public GameBoard(int size) {
        this.size = size;
        this.nodes = new GameNode[size][size];

        // count cell size depending on the board size
        this.boardSize = 600.0;
        this.tileSize = boardSize / size;

        setPrefSize(boardSize, boardSize);
        setMinSize(boardSize, boardSize);
        setMaxSize(boardSize, boardSize);

        initializeBoard();
    }

    private void initializeBoard() {
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                GameNode node = new GameNode(tileSize, new Position(row, col));
                nodes[row][col] = node;

                node.setLayoutX(col * tileSize);
                node.setLayoutY(row * tileSize);

                getChildren().add(node);
            }
        }
    }

    public void generateBoard(int bulbCount) {

        // Placing power source
        Position powerPos = getRandomPosition();
        powerNode = nodes[powerPos.getRow()][powerPos.getCol()];
        powerNode.setType(NodeType.Power);


        // Place bulbs and connect them to the power source
        List<Position> bulbPositions = new ArrayList<>();
        for (int i = 0; i < bulbCount; i++) {
            Position bulbPos = getRandomEmptyPosition();
            bulbPositions.add(bulbPos);
            GameNode bulbNode = nodes[bulbPos.getRow()][bulbPos.getCol()];
            bulbNode.setType(NodeType.Bulb);

            createPath(powerPos, bulbPos);
        }

        // Connect power source to near connectors
        for (Side side : Side.values()) {
            Position nextPos = getNextPosition(powerPos, side);
            if (isValidPosition(nextPos)) {
                GameNode nextNode = nodes[nextPos.getRow()][nextPos.getCol()];
                if (nextNode.containsConnector(getOppositeSide(side))) {
                    connectNodes(powerNode, side);
                }
            }
        }


        // Rotate every node silently
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                GameNode node = nodes[row][col];
                if (node.getType() != null) {
                    int rotations = new Random().nextInt(4);
                    for (int i = 0; i < rotations; i++) {
                        node.rotate(true);
                    }
                }
            }
        }

        fillEmptyNodes();
    }

    public Position getPowerNodePosition() {
        return this.powerNode != null ? this.powerNode.getPosition() : null;
    }

    public GameNode getPowerNode(){
        return powerNode;
    }

    public void updateConnections(Position powerPos) {
        // Clear all powered nodes
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                nodes[row][col].setPowered(false);
            }
        }

        updateNodeConnections(powerPos);
    }

    private void updateNodeConnections(Position pos) {
        GameNode node = nodes[pos.getRow()][pos.getCol()];
        if (node.isPowered()) {
            return;
        }

        node.setPowered(true);

        for (Side side : Side.values()) {
            if (node.containsConnector(side)) {
                Position nextPos = getNextPosition(pos, side);
                if (isValidPosition(nextPos)) {
                    GameNode nextNode = nodes[nextPos.getRow()][nextPos.getCol()];
                    if (nextNode.containsConnector(getOppositeSide(side))) {
                        updateNodeConnections(nextPos);
                    }
                }
            }
        }
    }

    private Position getNextPosition(Position pos, Side side) {
        int row = pos.getRow();
        int col = pos.getCol();
        return switch (side) {
            case NORTH -> new Position(row - 1, col);
            case EAST -> new Position(row, col + 1);
            case SOUTH -> new Position(row + 1, col);
            case WEST -> new Position(row, col - 1);
            default -> pos;
        };
    }

    private boolean isValidPosition(Position pos) {
        return pos.getRow() >= 0 && pos.getRow() < size &&
                pos.getCol() >= 0 && pos.getCol() < size;
    }

    private Position getRandomPosition() {
        Random rand = new Random();
        return new Position(rand.nextInt(size), rand.nextInt(size));
    }

    private Position getRandomEmptyPosition() {
        Position pos;
        do {
            pos = getRandomPosition();
        } while (nodes[pos.getRow()][pos.getCol()].getType() != null ||
                hasAdjacentBulb(pos));

        return pos;
    }

    private boolean hasAdjacentBulb(Position pos) {
        int row = pos.getRow();
        int col = pos.getCol();

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (row + i >= 0 && row + i < size &&
                        col + j >= 0 && col + j < size) {
                    if (nodes[row + i][col + j].isBulb()) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    private void createPath(Position from, Position to) {
        int currentRow = from.getRow();
        int currentCol = from.getCol();

        while (currentRow != to.getRow() || currentCol != to.getCol()) {
            GameNode currentNode = nodes[currentRow][currentCol];
            GameNode nextNode;
            Side currentSide;
            Side oppositeSide;

            // Find path without bulbs
            Position nextPos = findBestNextStep(new Position(currentRow, currentCol), to);
            int nextRow = nextPos.getRow();
            int nextCol = nextPos.getCol();

            nextNode = nodes[nextRow][nextCol];
            currentSide = getConnectionSide(new Position(currentRow, currentCol), nextPos);
            oppositeSide = getOppositeSide(currentSide);

            if (nextNode.getType() == null) {
                nextNode.setType(NodeType.Link);
            }

            if (!currentNode.isPower()) {
                connectNodes(currentNode, currentSide);
            }

            if (!nextNode.isBulb() || (nextRow == to.getRow() && nextCol == to.getCol())) {
                connectNodes(nextNode, oppositeSide);
            }

            currentRow = nextRow;
            currentCol = nextCol;
        }
    }

    private Position findBestNextStep(Position current, Position target) {
        List<Position> possibleSteps = new ArrayList<>();
        int currentRow = current.getRow();
        int currentCol = current.getCol();

        int[][] directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (int[] dir : directions) {
            int newRow = currentRow + dir[0];
            int newCol = currentCol + dir[1];

            if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size) {
                Position newPos = new Position(newRow, newCol);
                GameNode node = nodes[newRow][newCol];

                if (node.isBulb() && (newRow != target.getRow() || newCol != target.getCol())) {
                    continue;
                }

                possibleSteps.add(newPos);
            }
        }

        return possibleSteps.stream()
                .min(Comparator.comparingInt(pos ->
                        Math.abs(pos.getRow() - target.getRow()) +
                                Math.abs(pos.getCol() - target.getCol())))
                .orElse(new Position(
                        currentRow + Integer.compare(target.getRow(), currentRow),
                        currentCol + Integer.compare(target.getCol(), currentCol)
                ));
    }

    private Side getConnectionSide(Position from, Position to) {
        if (from.getRow() < to.getRow()) return Side.SOUTH;
        if (from.getRow() > to.getRow()) return Side.NORTH;
        if (from.getCol() < to.getCol()) return Side.EAST;
        return Side.WEST;
    }

    private Side getOppositeSide(Side side) {
        return Side.values()[(side.ordinal() + 2) % 4];
    }

    private void connectNodes(GameNode node, Side... sides) {
        node.connectSides(sides);

        if (node.getType() == NodeType.Bulb) {return;}
        // Add random connections
        if (Math.random() < 0.15) { // 15% Chance to add connections
            Side[] allSides = Side.values();
            Side randomSide = allSides[new Random().nextInt(allSides.length)];
            node.connectSides(randomSide);
        }
    }

    private void fillEmptyNodes() {
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                GameNode node = nodes[row][col];
                if (node.getType() == null) {
                    if (Math.random() < 0.5) { // Chance of creating a link
                        node.setType(NodeType.Link);

                        // Random connections
                        int connections = 2 + new Random().nextInt(3);
                        List<Side> sides = new ArrayList<>(Arrays.asList(Side.values()));
                        Collections.shuffle(sides);
                        Side[] selectedSides = sides.subList(0, connections).toArray(new Side[0]);
                        node.connectSides(selectedSides);
                    }
                }
            }
        }
    }

    public GameNode getNode(int row, int col) {
        return nodes[row][col];
    }
}