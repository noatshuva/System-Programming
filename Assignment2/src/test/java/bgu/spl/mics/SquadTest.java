package bgu.spl.mics;

import bgu.spl.mics.application.passiveObjects.Agent;
import bgu.spl.mics.application.passiveObjects.Squad;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class SquadTest {
    Squad squad1;
    @BeforeEach
    public void setUp(){
        squad1 = Squad.getInstance();
    }

    @Test
    public void test(){
        Squad squad2 = Squad.getInstance();
        assertEquals(squad1,squad2);
        Agent agent1 = new Agent("kim","0012" );
        Agent agent2 = new Agent("Noa","007");
        Agent agent3 = new Agent("James", "006");
        Agent agent4 = new Agent("Bond", "008");
        Agent [] agents = {agent1,agent2,agent3,agent4};
        squad1.load(agents);
        List<String> listAgent = Arrays.asList("006","007","008","0012");
        assertTrue(squad1.getAgents(listAgent));
        List<String> listAgent2 = Arrays.asList("006","009","008","0012");
        assertFalse(squad1.getAgents(listAgent2));
        List l = squad1.getAgentsNames(listAgent);
        List names = Arrays.asList("Noa", "James" , "Bond", "Kim");
        assertEquals(names, l);
    }
}
