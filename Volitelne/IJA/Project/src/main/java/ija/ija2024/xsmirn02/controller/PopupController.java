package ija.ija2024.xsmirn02.controller;

//Author : Shaposhnik Bogdan : xshapo04. Controller For Linking View and Model of Popup Window
import ija.ija2024.xsmirn02.view.SceneManager;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class PopupController {
    private javafx.stage.Popup popup;

    public void setPopup(javafx.stage.Popup popup) {
        this.popup = popup;
    }

    public void onBackToMenuPopup(ActionEvent actionEvent) {
        Stage stage = (Stage) ((Node) actionEvent.getSource()).getScene().getWindow();
        stage.close();
        SceneManager.switchToMenu();
    }

    @FXML
    private VBox root;

    @FXML
    public void initialize() {
        root.getStylesheets().add(getClass().getResource("/ija/ija2024/xsmirn02/css/style.css").toExternalForm());
    }

}
