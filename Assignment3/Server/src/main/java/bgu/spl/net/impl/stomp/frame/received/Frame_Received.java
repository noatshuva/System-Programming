package bgu.spl.net.impl.stomp.frame.received;

import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.impl.stomp.frame.Frame;

public abstract class Frame_Received extends Frame {
    public abstract void execute(StompMessagingProtocolImpl protocol);
}
