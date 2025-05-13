// Controls scene change
package ija.ija2024.xsmirn02.view;

//Author : Smirnov Nikita : xsmirn02. Manages Scene switching and load recourses needed for that
import ija.ija2024.xsmirn02.controller.GameController;
import ija.ija2024.xsmirn02.model.common.enums.Difficulty;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.URL;

public class SceneManager {
    private static Stage stage;

    public static void init(Stage primaryStage) {
        stage = primaryStage;
        stage.setTitle("LightBulb Game");
        stage.getIcons().add(new Image(SceneManager.class.getResource("/lightning.png").toExternalForm()));
        stage.setResizable(false);
        stage.setWidth(1280);
        stage.setHeight(720);
    }

    public static void switchToMenu() {
        try {
            Scene scene = new Scene(FXMLLoader.load(SceneManager.class.getResource("menu-view.fxml")));
            scene.getStylesheets().add(SceneManager.class.getResource("/ija/ija2024/xsmirn02/css/style.css").toExternalForm());
            stage.setScene(scene);
            stage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void switchToGame(Difficulty difficulty) {
        try {
            FXMLLoader loader = new FXMLLoader(SceneManager.class.getResource("/ija/ija2024/xsmirn02/view/game-view.fxml"));
            Scene scene = new Scene(loader.load());
            scene.getStylesheets().add(SceneManager.class.getResource("/ija/ija2024/xsmirn02/css/style.css").toExternalForm());

            GameController controller = loader.getController();
            controller.initGame(difficulty); // Новый метод для инициализации с параметрами

            stage.setScene(scene);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}