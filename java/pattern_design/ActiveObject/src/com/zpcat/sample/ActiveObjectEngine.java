package com.zpcat.sample;

import java.util.LinkedList;

public class ActiveObjectEngine {
    private LinkedList<Command> itsCommands = new LinkedList<Command>();
    
    public void addCommand(Command c) {
        itsCommands.add(c);
    }
    
    public void run() throws Exception {
        while (!itsCommands.isEmpty()) {
            Command c = (Command) itsCommands.getFirst();
            itsCommands.removeFirst();
            c.execute();
        }
    }
}
