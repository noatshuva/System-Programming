package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.AgentsAvailableEvent;
import bgu.spl.mics.application.messages.TerminateBroadcast;
import bgu.spl.mics.application.passiveObjects.Pair;
import bgu.spl.mics.application.passiveObjects.Squad;

import java.util.List;

/**
 * Only this type of Subscriber can access the squad.
 * Three are several Moneypenny-instances - each of them holds a unique serial number that will later be printed on the report.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Moneypenny extends Subscriber {
    Squad squad;
    int id;
    int tick;

    public Moneypenny(int id) {
        super("Moneypenny" + id);
        squad = Squad.getInstance();
        this.id = id;
        tick = 0;
    }

    @Override
    protected void initialize() {

        subscribeBroadcast(TerminateBroadcast.class, (t) ->{
            terminate();
        });

        subscribeEvent(AgentsAvailableEvent.class, (av) -> {
            List<String> a = av.getAgents();
            boolean isAvail = squad.getAgents(a);
            int resultedID = isAvail ? id : -1;
            if(isAvail)
                complete(av, new Pair<>(resultedID,squad.getAgentsNames(a)));
            else
                complete(av, new Pair<>(resultedID,null));

            if(av.canSendThem().get()>0) {//if we still in this mission tick
                squad.sendAgents(a, av.canSendThem().get()); //you can send the agents
            }
            else if(isAvail) { //if the mission tick has passed but the agents were allocate for this mission
                squad.releaseAgents(a);
            }
        });


    }

}
