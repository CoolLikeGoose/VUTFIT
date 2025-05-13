package ija.ija2024.xsmirn02.model.common;
//Author : Smirnov Nikita : xsmirn02. For rotate command management 
import ija.ija2024.xsmirn02.model.common.interfaces.Command;

public class RotateCommand implements Command {
    private final GameNode node;

    public RotateCommand(GameNode node) {
        this.node = node;
    }

    @Override
    public void execute() {
        node.autoMove = true;
        node.rotate();
    }

    @Override
    public void undo() {
        node.autoMove = true;
        node.rotateBack();
    }

    @Override
    public String toString() {
        return "RotateCommand{" +
                "node=" + node.toString() +
                '}';
    }
}
