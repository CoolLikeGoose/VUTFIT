Authors: Smirnov Nikita (xsmirn02), Shaposhnik Bogdan(xshpo04)
Teamleader: Smirnov Nikita
Tools: Java, JavaFX, Maven

How to use:
    For program run use:
        >>> mvn javafx:run

    For project compiling use:
        >>> mvn compile
    For docs generating use:
        >>> mvn javadoc:javadoc
    For JAR compile use:
        >>> mvn package

Before this step either add javafx lib files to lib or replace --modudle-path
We cant include those modules in ZIP archive because VUT IS have limit on archive size up to 5 mb
(With those modules our archive size is 7 mb)
    To run JAR use:
    java --module-path lib --add-modules javafx.controls,javafx.fxml -jar target/xsmirn02-1.0-SNAPSHOT.jar
