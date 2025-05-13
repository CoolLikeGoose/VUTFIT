package ija.ija2024.xsmirn02.model.common;
//Author : Smirnov Nikita : xsmirn02. Representation of Gaming Interactive Elements

import ija.ija2024.xsmirn02.model.common.enums.NodeType;
import ija.ija2024.xsmirn02.model.common.enums.Side;
import ija.ija2024.xsmirn02.model.common.interfaces.Observable;
import ija.ija2024.xsmirn02.util.Logger;
import javafx.animation.RotateTransition;
import javafx.scene.Group;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Region;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import javafx.util.Duration;
import javafx.scene.text.Font;
import javafx.scene.text.Text;

public class GameNode extends Region implements Observable {
    private final Rectangle tile;
    private final Group wireGroup;
    private final Line[] allLines;
    private final RotateTransition rotateTransition;

    private boolean powered = false;
    public boolean autoMove = false;
    private Text debugText;

    private final Position _position;
    private boolean[] connections;

    private NodeType _nodeType;

    private Observer observer;

    public GameNode(double size, Position position) {
        connections = new boolean[] {false, false, false, false};
        _position = position;

        setPrefSize(size, size);
        setMinSize(size, size);
        setMaxSize(size, size);

        tile = new Rectangle(size * 0.8, size * 0.8);
        tile.setStroke(Color.BLACK);
        tile.setFill(Color.TRANSPARENT);

        tile.setLayoutX((size - tile.getWidth()) / 2);
        tile.setLayoutY((size - tile.getHeight()) / 2);

        //wires
        wireGroup = new Group();
        allLines = new Line[4];

        double halfSize = size / 2;
        
        // NORTH
        allLines[Side.NORTH.ordinal()] = createBaseLine(0, -halfSize);
        // EAST
        allLines[Side.EAST.ordinal()] = createBaseLine(halfSize, 0);
        // SOUTH
        allLines[Side.SOUTH.ordinal()] = createBaseLine(0, halfSize);
        // WEST
        allLines[Side.WEST.ordinal()] = createBaseLine(-halfSize, 0);

        wireGroup.getChildren().addAll(allLines);
        wireGroup.setTranslateX(halfSize);
        wireGroup.setTranslateY(halfSize);
        getChildren().addAll(tile, wireGroup);

        rotateTransition = new RotateTransition(Duration.millis(100), wireGroup);
        setOnMouseClicked(this::handleClick);

        // DEBUG
//        debugText = new Text();
//        debugText.setFill(Color.WHITE);
//        debugText.setFont(Font.font(10));
//        debugText.setLayoutX(5);
//        debugText.setLayoutY(10);
//        getChildren().add(debugText);

//        updateDebugText();
    }

    public String toString(boolean display) {
        StringBuilder sb = new StringBuilder("[");
        
        // Добавляем состояние для каждой стороны
        sb.append("N:").append(connections[Side.NORTH.ordinal()] ? "1" : "0").append(";");
        if (display) {sb.append("\n");}
        sb.append("E:").append(connections[Side.EAST.ordinal()] ? "1" : "0").append(";");
        if (display) {sb.append("\n");}
        sb.append("S:").append(connections[Side.SOUTH.ordinal()] ? "1" : "0").append(";");
        if (display) {sb.append("\n");}
        sb.append("W:").append(connections[Side.WEST.ordinal()] ? "1" : "0").append(";");
        if (display) {sb.append("\n");}

        // Добавляем тип ноды
        if (isPower()) {
            sb.append("P");
        } else if (isBulb()) {
            sb.append("B");
        } else if (isLink()) {
            sb.append("L");
        } else {
            sb.append("-");
        }

        if (display) {sb.append("\n").append("H:").append(powered ? "1" : "0");}
        else {
            sb.append(_position.toString());
        }
        sb.append("]");
        return sb.toString();
    }

    @Override
    public String toString() {
        return toString(false);
    }

    private Line createBaseLine(double endX, double endY) {
        Line line = new Line(0, 0, endX, endY);
        line.setStrokeWidth(3);
        line.setStroke(Color.TRANSPARENT);
        return line;
    }

    private void handleClick(MouseEvent event) {
        Logger.getInstance().log("ROTATED: " + "[" + this.toString() + "]");
        autoMove = false;
        rotate();
    }

    public void connectSides(Side... sides) {
        for (Side side : sides) {
            connections[side.ordinal()] = true;
        }
        updateWiresVisibility();
//        updateDebugText();
    }

    private void updateWiresVisibility() {
        for (int i = 0; i < connections.length; i++) {
            allLines[i].setStroke(connections[i] ? Color.BLACK : Color.TRANSPARENT);
        }
    }

    @Override
    public void registerObserver(Observer observer) {
        this.observer = observer;
    }

    @Override
    public void removeObserver(Observer observer) {
        this.observer = null;
    }

    @Override
    public void notifyObserver() {
        this.observer.update(this);
    }

    public void rotate(boolean silent) {
        boolean temp = connections[3];
        for (int i = 3; i > 0; i--) {
            connections[i] = connections[i-1];
        }
        connections[0] = temp;

        if (silent) {
            wireGroup.setRotate(wireGroup.getRotate() + 90);
        } else {
            rotateTransition.setByAngle(90);
            rotateTransition.play();

//            updateDebugText();
            notifyObserver();
        }
    }

    public void rotate() {
        rotate(false);
    }

    public void rotateBack() {
        boolean temp = connections[0];
        for (int i = 0; i < 3; i++) {
            connections[i] = connections[i+1];
        }
        connections[3] = temp;

        rotateTransition.setByAngle(-90);
        rotateTransition.play();

//        updateDebugText();
        notifyObserver();
    }

    public void setType(NodeType type) {
        _nodeType = type;
        switch (type) {
            case Bulb -> {
                tile.setFill(Color.DARKBLUE);
                connections = new boolean[]{false, false, false, false};
            }
            case Power -> tile.setFill(Color.RED);
        }
//        updateDebugText();
    }

    public Position getPosition() {
        return _position;
    }

    public NodeType getType() {
        return _nodeType;
    }

    public boolean isLink() {
        return _nodeType == NodeType.Link;
    }

    public boolean isBulb() {
        return _nodeType == NodeType.Bulb;
    }

    public boolean isPower() {
        return _nodeType == NodeType.Power;
    }

    public boolean containsConnector(Side s) {
        return connections[s.ordinal()];
    }

    public boolean isPowered() {
        return powered;
    }

    public void setPowered(boolean powered) {
        this.powered = powered;

        if (powered) {
            for (Line line : allLines) {
                if (line.getStroke() != Color.TRANSPARENT) {
                    line.setStroke(Color.YELLOW);
                }
            }
            if (isBulb()) {
                tile.setFill(Color.YELLOW);
            }
        } else {
            for (Line line : allLines) {
                if (line.getStroke() != Color.TRANSPARENT) {
                    line.setStroke(Color.BLACK);
                }
            }
            if (isBulb()) {
                tile.setFill(Color.DARKBLUE);
            }
        }

//        updateDebugText();
    }

//    private void updateDebugText() {
//        debugText.setText(toString(true));
//    }
}