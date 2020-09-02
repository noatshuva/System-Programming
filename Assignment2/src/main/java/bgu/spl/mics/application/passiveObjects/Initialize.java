package bgu.spl.mics.application.passiveObjects;

import java.util.concurrent.atomic.AtomicInteger;

public class Initialize {

    private AtomicInteger counter;

    private Initialize() {
        counter = new AtomicInteger(0);
    }

    public static Initialize getInstance() {
        return Initialize.SingletonHolder.singleton;
    }

    private static class SingletonHolder {
        private static Initialize singleton = new Initialize();
    }

    public AtomicInteger getCounter(){
        return counter;
    }

    public void addToCounter(){
        counter.getAndAdd(1);
    }

}
