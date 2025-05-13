package ija.ija2024.xsmirn02.controller;

//Author : Shaposhnik Bogdan : xshapo04. Controller For Linking View and Model of Menu
import ija.ija2024.xsmirn02.model.common.enums.Difficulty;
import ija.ija2024.xsmirn02.view.SceneManager;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.layout.VBox;

public class MenuController {
    @FXML
    void onEasyButtonClick(ActionEvent event) {
        SceneManager.switchToGame(Difficulty.EASY);
    }

    @FXML
    void onMediumButtonClick(ActionEvent event) {
        SceneManager.switchToGame(Difficulty.MEDIUM);
    }

    @FXML
    void onHardButtonClick(ActionEvent event) {
        SceneManager.switchToGame(Difficulty.HARD);
    }

    @FXML
    private VBox difficultyBox;
    @FXML private Button playButton;



    public void onPlayClicked(ActionEvent event) {
        difficultyBox.setVisible(true);
        difficultyBox.setManaged(true);

        playButton.setVisible(false);
        playButton.setManaged(false);
    }

    public void onLoadClicked(ActionEvent event) {
        System.out.println("Loading.");
    }

    public void onExitClicked(ActionEvent event) {
        System.exit(0);
    }
}
