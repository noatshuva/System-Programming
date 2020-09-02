package bgu.spl.net.impl.stomp.frame.received;

import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.impl.stomp.frame.Frame;
import bgu.spl.net.impl.stomp.frame.sent.MESSAGE;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.User;

import java.util.*;

public class SEND extends Frame_Received {
    @Override
    public void execute(StompMessagingProtocolImpl protocol) {
        Map<String, String> map = this.getHeaders();
        Frame msg = new MESSAGE();

        User user = protocol.getManager().getUserByConnectionId(protocol.getConnectionId());
        int id = user.getIdByTopic(map.get("destination"));
        msg.addHeader("subscription", String.valueOf(id));
        msg.addHeader("destination", map.get("destination"));
        msg.addHeader("body", this.getBody());

        protocol.getConnections().send(map.get("destination"), msg);
    }
}
