package ija.ija2024.homework2.common;

import ija.ija2024.tool.common.AbstractObservableField;
import ija.ija2024.tool.common.Observable;
import java.util.Set;
import java.util.HashSet;

public class GameNode extends AbstractObservableField implements Observable.Observer {
    private Position _position;

    private boolean[] connections;
    private GameNode[] neighbors;

    private boolean _isLit = false;

    private NodeType _nodeType;

    public GameNode() {
        connections = new boolean[] {false, false, false, false};
        neighbors = new GameNode[4];
    }

    public void setNeighbor(Side side, GameNode node) {
        neighbors[side.ordinal()] = node;
        if (node != null) {
            addObserver(node);
            node.addObserver(this);
        }
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

        updatePowerState();
        notifyObservers();
    }

    @Override
    public boolean north() {
        return connections[0];
    }

    @Override
    public boolean east() {
        return connections[1];
    }

    @Override
    public boolean south() {
        return connections[2];
    }

    @Override
    public boolean west() {
        return connections[3];
    }

    @Override
    public boolean light() {
        return _isLit;
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

    public String toString() {

        StringBuilder sb = new StringBuilder("{");
        switch (_nodeType) {
            case Link:
                sb.append("L");
                break;
            case Bulb:
                sb.append("B");
                break;
            case Power:
                sb.append("P");
                break;
            default:
                sb.append("E");
                break;
        }

        sb.append("[" + this._position.getRow() + "@" + this._position.getCol() + "]");

        if(this._nodeType == null){
            sb.append("[]}");
            return sb.toString();
        }

        sb.append("[");
        for (int i = 0; i < connections.length; i++) {
            if (!connections[i])
                continue;

            switch (i) {
                case 0:
                        sb.append("NORTH,");
                    break;
                case 1:
                        sb.append("EAST,");
                    break;
                case 2:
                        sb.append("SOUTH,");
                    break;
                case 3:
                        sb.append("WEST,");
                    break;
            }
        }
        sb.deleteCharAt(sb.length() - 1);
        sb.append("]}");

        return sb.toString();
    }

    @Override
    public void update(Observable observable) {
        updatePowerState();
    }

    public void updatePowerState() {
        boolean lastState = _isLit;
        _isLit = isPower() || isConnectedToPoweredNode();

        if (lastState != _isLit) {
            notifyObservers();
        }
    }

    private boolean isConnectedToPoweredNode() {
        Set<GameNode> visited = new HashSet<>();
        return findPathToPower(visited);
    }

    private boolean findPathToPower(Set<GameNode> visited) {
        if (visited.contains(this)) {
            return false;
        }
        visited.add(this);
        
        for (Side side : Side.values()) {
            int index = side.ordinal();
            if (!connections[index] || neighbors[index] == null) {
                continue;
            }
            
            GameNode neighbor = neighbors[index];
            boolean isConnected = false;
            
            switch(side) {
                case NORTH:
                    isConnected = neighbor.south();
                    break;
                case EAST:
                    isConnected = neighbor.west();
                    break;
                case SOUTH:
                    isConnected = neighbor.north();
                    break;
                case WEST:
                    isConnected = neighbor.east();
                    break;
            }
            
            if (isConnected) {
                if (neighbor.isPower()) {
                    return true;
                }
                if (neighbor.findPathToPower(visited)) {
                    return true;
                }
            }
        }
        
        return false;
    }

}