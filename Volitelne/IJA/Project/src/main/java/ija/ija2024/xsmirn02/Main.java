package ija.ija2024.xsmirn02;
//Author : Smirnov Nikita : xsmirn02. Main Program Initialization
import javafx.application.Application;
import javafx.stage.Stage;
import ija.ija2024.xsmirn02.view.SceneManager;

import java.io.IOException;

public class Main extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        SceneManager.init(stage);
        SceneManager.switchToMenu();
    }

    public static void main(String[] args) {
        launch();
    }
}