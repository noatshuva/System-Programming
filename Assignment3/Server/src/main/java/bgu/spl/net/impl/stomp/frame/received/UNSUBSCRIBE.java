package bgu.spl.net.impl.stomp.frame.received;

import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.impl.stomp.frame.Frame;
import bgu.spl.net.impl.stomp.frame.sent.RECEIPT;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.User;

import java.util.Map;

public class UNSUBSCRIBE extends Frame_Received {
    @Override
    public void execute(StompMessagingProtocolImpl protocol) {
        Map<String, String> map = this.getHeaders();
        ConnectionsImpl con = (ConnectionsImpl)protocol.getConnections();
        User user = protocol.getManager().getUserByConnectionId(protocol.getConnectionId());
        int id =  Integer.parseInt(map.get("id"));
        String topic = user.getTopicById(id);
        user.removeTopicId(id);
        con.removeUserToTopic(protocol.getConnectionId(), topic);
        Frame receipt = new RECEIPT();
        receipt.addHeader("receipt-id", map.get("receipt"));
        protocol.getConnections().send(protocol.getConnectionId(), receipt);
    }
}
