package bgu.spl.net.impl.stomp.frame.received;

import bgu.spl.net.impl.stomp.Manager;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.impl.stomp.frame.Frame;
import bgu.spl.net.impl.stomp.frame.sent.CONNECTED;
import bgu.spl.net.impl.stomp.frame.sent.ERROR;
import bgu.spl.net.srv.User;

import java.util.Map;

public class CONNECT extends Frame_Received {
    @Override
    public void execute(StompMessagingProtocolImpl protocol) {
        Map<String, String> map = this.getHeaders();
        String userName = map.get("login");
        String password = map.get("passcode");
        Manager manager = protocol.getManager();
        User user;
        Frame frame;

        if(manager.getUser(userName) != null) {
            user = manager.getUser(userName);

            if (user.getPassword().equals(password) && user.logIn()) {
                frame = new CONNECTED();
                frame.addHeader("version", map.get("accept-version"));
                manager.addIfAbsent(protocol.getConnectionId(), new User(userName, password));
            } else {
                frame = new ERROR();
                frame.addHeader("receipt-id", map.get("receipt"));
                boolean wrongPass = !user.getPassword().equals(map.get("passcode"));
                String errStr = wrongPass ? "Wrong password" : "User already logged in";
                frame.addHeader("message", errStr);
            }
        }
        else {
            frame = new CONNECTED();
            frame.addHeader("version", map.get("accept-version"));
            manager.addIfAbsent(protocol.getConnectionId(), new User(userName, password));
        }

        protocol.getConnections().send(protocol.getConnectionId(), frame);

    }
}
