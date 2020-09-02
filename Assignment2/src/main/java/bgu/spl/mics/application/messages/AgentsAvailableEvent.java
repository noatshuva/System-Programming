package bgu.spl.mics.application.messages;

import bgu.spl.mics.Event;
import bgu.spl.mics.Future;
import bgu.spl.mics.application.passiveObjects.Pair;

import java.util.List;

public class AgentsAvailableEvent implements Event<Pair<Integer, List<String>>> {
    private List<String> agents;
    private Future<Integer> canSendThem;

    public AgentsAvailableEvent(List<String> agents) {
        this.agents = agents;
        canSendThem = new Future<>();
    }

    public List<String> getAgents() {
        return agents;
    }

    public Future<Integer> canSendThem() {
        return canSendThem;
    }
}
