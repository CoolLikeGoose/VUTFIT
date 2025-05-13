package ija.ija2024.homework2.common;

public class Position {
    private final int _row;
    private final int _col;

    public Position(int row, int col) {
        //add check to be in range [1,]?
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
}
