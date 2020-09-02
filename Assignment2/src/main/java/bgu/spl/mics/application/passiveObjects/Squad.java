package bgu.spl.mics.application.passiveObjects;

import java.util.List;
import java.util.Map;
import java.util.*;

/**
 * Passive data-object representing a information about an agent in MI6.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You may add ONLY private fields and methods to this class.
 */
public class Squad {

    private Map<String, Agent> agents;

    /**
     * Retrieves the single instance of this class.
     */

    private Squad() {
        agents = new HashMap<>();
    }

    public static Squad getInstance() {
        return SingletonHolder.singleton;
    }

    private static class SingletonHolder {
       private static Squad singleton = new Squad();
    }

    /**
     * Initializes the squad. This method adds all the agents to the squad.
     * <p>
     *
     * @param agents Data structure containing all data necessary for initialization
     *               of the squad.
     */
    public void load(Agent[] agents) {
        for (Agent a : agents) {
			synchronized (this.agents) {
				this.agents.put(a.getSerialNumber(), a);
			}
        }
    }

    /**
     * Releases agents.
     */
    public void releaseAgents(List<String> serials) {
        for (int i = 0; i < serials.size(); i++) {
            Agent a = agents.get(serials.get(i));
            releaseNotify(a);
        }
    }

    /**
     * simulates executing a mission by calling sleep.
     *
     * @param time time ticks to sleep
     */
    public void sendAgents(List<String> serials, int time) {
        try {
            int sleepFor = time*100;
            Thread.sleep(sleepFor);
        }
        catch (InterruptedException ignored){}
        releaseAgents(serials);
    }

    /**
     * acquires an agent, i.e. holds the agent until the caller is done with it
     *
     * @param serials the serial numbers of the agents
     * @return ‘false’ if an agent of serialNumber ‘serial’ is missing, and ‘true’ otherwise
     */
    public boolean getAgents(List<String> serials) {
    	serials.sort(String::compareTo);
        for (String ser : serials){
            if (!(agents.containsKey(ser)))
                return false;
        }
        String last = "";
        try {
            for (String ser : serials) {
                Agent a = agents.get(ser);
                synchronized (a) {
                    while (!a.isAvailable()) {
                            a.wait();
                    }
                    a.acquire();
					last = ser;
				}

            }
        } catch (Exception ignored) {
			for (int i = 0; serials.get(i).compareTo(last)<=0; i++) {
                releaseNotify(agents.get(serials.get(i)));
			}
        }
        return true;
    }

    private void releaseNotify(Agent a) {
        synchronized (a) {
            a.release();
            a.notifyAll();
        }
    }

    /**
     * gets the agents names
     *
     * @param serials the serial numbers of the agents
     * @return a list of the names of the agents with the specified serials.
     */
    public List<String> getAgentsNames(List<String> serials) {
        List<String> agentsNames = new LinkedList<>();
        for (int i = 0; i < serials.size() ; i++) {
            Agent agent = agents.get(serials.get(i));
            agentsNames.add(agent.getName());
        }
        return agentsNames;
    }

}
