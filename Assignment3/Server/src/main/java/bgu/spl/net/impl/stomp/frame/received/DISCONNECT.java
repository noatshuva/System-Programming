package bgu.spl.net.impl.stomp.frame.received;

import bgu.spl.net.impl.stomp.Manager;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.impl.stomp.frame.Frame;
import bgu.spl.net.impl.stomp.frame.sent.RECEIPT;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.User;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class DISCONNECT extends Frame_Received {
    @Override
    public void execute(StompMessagingProtocolImpl protocol) {
        Map<String, String> map = this.getHeaders();
        Manager manager = protocol.getManager();
        User user = manager.getUserByConnectionId(protocol.getConnectionId());
        ConnectionsImpl<Frame> con = protocol.getConnections();
        ConcurrentHashMap<Integer, String> topicMap = user.getTopicMap();
        if (user.logOut()) {
            if(topicMap != null) {
                for (int topicId : topicMap.keySet()) {
                    String topic = topicMap.get(topicId);
                    con.removeUserToTopic(protocol.getConnectionId(), topic);
                }
            user.clearTopicMap();
//                con.clearUsersTopicMap(protocol.getConnectionId());
            }

            Frame receipt = new RECEIPT();
            receipt.addHeader("receipt-id", map.get("receipt"));
            protocol.getConnections().send(protocol.getConnectionId(), receipt);
            con.disconnect(protocol.getConnectionId());
        }
    }
}
