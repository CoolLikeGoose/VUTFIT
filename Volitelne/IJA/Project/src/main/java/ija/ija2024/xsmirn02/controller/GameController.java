package ija.ija2024.xsmirn02.controller;

//Author : Shaposhnik Bogdan : xshapo04. Controller For Linking View and Model of Game Part
import ija.ija2024.xsmirn02.model.GameBoard;
import ija.ija2024.xsmirn02.model.GameState;
import ija.ija2024.xsmirn02.model.common.enums.Difficulty;
import ija.ija2024.xsmirn02.model.common.interfaces.Observable;
import ija.ija2024.xsmirn02.view.SceneManager;
import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class GameController implements Observable.Observer {
    @FXML
    private BorderPane boardContainer;
    private GameState gameState;

    public Button prevButton;
    public Button nextButton;

    public void initGame(Difficulty difficulty) {
        gameState = new GameState(difficulty);
        gameState.registerObserver(this);

        GameBoard gameBoard = gameState.getGameBoard();
        boardContainer.setCenter(gameBoard);
    }

    public void onBackToMenu(ActionEvent actionEvent) {
        SceneManager.switchToMenu();
    }

    public void onPreviousStep(ActionEvent event) {
        prevButton.setDisable(!gameState.stepBack());
        nextButton.setDisable(gameState.redoStack.isEmpty());
    }
    public void onNextStep(ActionEvent event) {
        nextButton.setDisable(!gameState.stepForward());
        prevButton.setDisable(gameState.undoStack.isEmpty());
    }

    public void onExitGame(ActionEvent event) {
        System.exit(0);
    }

    @FXML private VBox popupWindow;

    @FXML
    private void showPopup(ActionEvent event) {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/popup_content.fxml"));
            Parent popupContent = loader.load();

            Stage popupStage = new Stage();
            popupStage.initModality(Modality.APPLICATION_MODAL); // Blocks parent window
            popupStage.setScene(new Scene(popupContent));
            popupStage.setTitle("Exit");
            popupStage.getIcons().add(new Image(SceneManager.class.getResource("/lightning.png").toExternalForm()));
            popupStage.setResizable(false);
            popupStage.sizeToScene();

            popupStage.setOnCloseRequest(Event::consume); //Close denial
            popupStage.show();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void update(Observable var1) {
        if (gameState.isGameComplete()) {
            showPopup(null);
        } else {
            prevButton.setDisable(gameState.undoStack.isEmpty());
            nextButton.setDisable(gameState.redoStack.isEmpty());
        }
    }
}