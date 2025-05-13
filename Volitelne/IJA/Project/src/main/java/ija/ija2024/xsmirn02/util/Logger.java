package ija.ija2024.xsmirn02.util;

//Author : Smirnov Nikita : xsmirn02. Logging information

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class Logger {
    private static final Logger instance = new Logger();
    private BufferedWriter writer;

    private Logger() {
        try {
            writer = new BufferedWriter(new FileWriter("game.log", true));
        } catch (IOException e) { e.printStackTrace(); }
    }

    public static Logger getInstance() {
        return instance;
    }

    public void log(String message) {
        try {
            writer.write(message + "\n");
            writer.flush();
        } catch (IOException e) { e.printStackTrace(); }
    }
}
