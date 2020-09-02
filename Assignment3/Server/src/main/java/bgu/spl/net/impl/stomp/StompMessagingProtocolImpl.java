package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.frame.Frame;
import bgu.spl.net.impl.stomp.frame.received.Frame_Received;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<Frame> {

    private int connectionId;
    private ConnectionsImpl<Frame> connections;
    private boolean shouldTerminate;
    private Manager manager;

    public StompMessagingProtocolImpl(Manager manager) {
        this.manager = manager;
    }

    @Override
    public void start(int connectionId, Connections<Frame> connections) {
        this.connectionId = connectionId;
        this.connections = (ConnectionsImpl<Frame>) connections;
        shouldTerminate = false;
    }

    @Override
    public void process(Frame message) {
        Frame_Received received = (Frame_Received) message;
        received.execute(this);
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public ConnectionsImpl<Frame> getConnections() {
        return connections;
    }

    public boolean isShouldTerminate() {
        return shouldTerminate;
    }

    public Manager getManager() {
        return manager;
    }
}
