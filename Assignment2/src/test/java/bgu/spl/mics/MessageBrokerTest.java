package bgu.spl.mics;

import bgu.spl.mics.application.messages.MissionReceivedEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.subscribers.M;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class MessageBrokerTest {
    MessageBroker msg;

    @BeforeEach
    public void setUp(){
        msg = MessageBrokerImpl.getInstance();
    }

    @Test
    public void test(){
        Subscriber sub = new M(1);
        msg.register(sub);
        msg.subscribeEvent(MissionReceivedEvent.class, sub);
        msg.subscribeBroadcast(TickBroadcast.class, sub);
        MessageBroker msg2 = MessageBrokerImpl.getInstance();
        assertEquals(msg2,msg);

    }
}
