package ija.ija2024.xsmirn02.model.common;
//Author : Smirnov Nikita : xsmirn02. Stores positions 

public class Position {
    private final int _row;
    private final int _col;

    public Position(int row, int col) {
        _row = row;
        _col = col;
    }

    public int getRow() {
        return _row;
    }

    public int getCol() {
        return _col;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;

        return _row == ((Position) obj).getRow() && _col == ((Position) obj).getCol();
    }

    @Override
    public String toString() {
        return "{" + _col + ":" + _row + "}";
    }
}