package com.zpcat.sample;

public class DelayedTyper implements Command {

    private long mDelay = 0;
    private char mChar;
    private static boolean stop = false;
    private static ActiveObjectEngine engine = new ActiveObjectEngine();
    
    public DelayedTyper(long delay, char c) {
        mDelay = delay;
        mChar = c;
    }

    @Override
    public void execute() throws Exception {
        System.out.print(mChar);
        if (!stop) {
            delayAndRepeat();
        }
    }

    private void delayAndRepeat() {
        engine.addCommand(new SleepCommand(mDelay, engine, this));
    }
    
    public static void main(String args[]) throws Exception {
        engine.addCommand(new DelayedTyper(200, 'a'));
        engine.addCommand(new DelayedTyper(300, 'b'));
        engine.addCommand(new DelayedTyper(400, 'c'));
        engine.addCommand(new DelayedTyper(500, 'd'));
        
        Command stopCommand = new Command() {
            
            @Override
            public void execute() throws Exception {
                stop = true;
                
            }
        };
        
        engine.addCommand(new SleepCommand(20000, engine, stopCommand));
        engine.run();
    }
}
