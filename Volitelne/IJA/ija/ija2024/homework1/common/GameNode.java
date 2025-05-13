package ija.ija2024.homework1.common;

public class GameNode {
    private Position _position;
    private boolean[] connections;

    private NodeType _nodeType;

    public GameNode() {
        connections = new boolean[] {false, false, false, false};
    }

    public void setType(NodeType type) {
        _nodeType = type;
    }

    public void connectSides(Side... sides) {
        for (Side side : sides) {
            connections[side.ordinal()] = true;
        }
    }

    public Position getPosition() {
        return _position;
    }

    public void setPosition(Position position) {
        _position = position;
    }

    public void turn() {
        boolean temp = connections[3];
        for (int i = 3; i > 0; i--) {
            connections[i] = connections[i-1];
        }
        connections[0] = temp;
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
}
