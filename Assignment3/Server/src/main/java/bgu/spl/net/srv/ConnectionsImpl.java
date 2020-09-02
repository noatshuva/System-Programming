package bgu.spl.net.srv;

import java.io.IOException;
import java.util.LinkedList;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> handlerMap;
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<Integer>> topicMap;
    private AtomicInteger id;

    public ConnectionsImpl() {
        handlerMap = new ConcurrentHashMap<>();
        topicMap = new ConcurrentHashMap<>();
        this.id = new AtomicInteger(0);
    }

    @Override
    public boolean send(int connectionId, T msg) {
        ConnectionHandler<T> handler = handlerMap.get(connectionId);
        if (handler == null)
            return false;
        handler.send(msg);
        return true;
    }

    @Override
    public void send(String topic, T msg) {
        ConcurrentLinkedQueue<Integer> ids = topicMap.get(topic);
        if (ids == null)
            return;
        ids.forEach(idN -> send(idN, msg));
    }

    @Override
    public void disconnect(int connectionId) {
        try {
            ConnectionHandler<T> handler = handlerMap.get(connectionId);
            if (handler == null)
                return;
            handler.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        handlerMap.remove(connectionId);
    }

    @Override
    public int add(ConnectionHandler<T> connectionHandler) {
        int newId = id.getAndIncrement();
        handlerMap.put(newId, connectionHandler);
        return newId;
    }

    public ConcurrentHashMap<String, ConcurrentLinkedQueue<Integer>> getTopicMap() {
        return topicMap;
    }

    public void addUserToTopic(int id, String topic) {
        topicMap.putIfAbsent(topic, new ConcurrentLinkedQueue<>());
        topicMap.get(topic).add(id);
    }

    public void removeUserToTopic(int id, String topic) {
        topicMap.get(topic).remove(id);
    }

    public int size() {
        return handlerMap.size();
    }
}