module ija.ija2024.xsmirn02 {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;

    opens ija.ija2024.xsmirn02 to javafx.fxml;
    exports ija.ija2024.xsmirn02 to javafx.fxml, javafx.graphics;

    opens ija.ija2024.xsmirn02.controller to javafx.fxml;
    exports ija.ija2024.xsmirn02.controller to javafx.fxml;
}