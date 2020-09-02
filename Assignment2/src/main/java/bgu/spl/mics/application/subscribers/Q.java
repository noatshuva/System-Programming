package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.GadgetAvailableEvent;
import bgu.spl.mics.application.messages.TerminateBroadcast;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.passiveObjects.Inventory;

/**
 * Q is the only Subscriber\Publisher that has access to the {@link bgu.spl.mics.application.passiveObjects.Inventory}.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Q extends Subscriber {

    private Inventory inventory;
    private int tick;

    public Q(int id) {
        super("Q" + id);
        inventory = Inventory.getInstance();
        tick = 0;
    }

    @Override
    protected void initialize() {
        subscribeBroadcast(TerminateBroadcast.class, (t) ->{
            terminate();
        });

        subscribeBroadcast(TickBroadcast.class, (t) -> {
            tick = t.getTick();
        });

        subscribeEvent(GadgetAvailableEvent.class, (g) -> {
            String gadget = g.getGadget();
            boolean isAvail = inventory.getItem(gadget);
            int result = isAvail ? tick : -1;
            complete(g, result);
        });

    }

}
