package bgu.spl.net.srv;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicBoolean;

public class User {
    private String userName;
    private String password;
    private final AtomicBoolean isLogged = new AtomicBoolean(false);
    ConcurrentHashMap<Integer, String > topicMap;


    public User(String userName, String password){
        this.userName = userName;
        this.password = password;
        isLogged.set(true);
        topicMap = new ConcurrentHashMap<>();
    }

    public String getUserName(){
        return userName;
    }

    public String getPassword(){
        return password;
    }

    public void setUserName(String userName){
        this.userName = userName;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public boolean isLogged(){
        return isLogged.get();
    }

    public boolean logIn(){
        return isLogged.compareAndSet(false,true);
    }
    public boolean logOut(){
        return isLogged.compareAndSet(true,false);
    }

    public void addTopicId (String topic, int id){
        topicMap.putIfAbsent(id,topic);
    }

    public void removeTopicId (int id){
        topicMap.remove(id);
    }
    public String getTopicById (int id){
        return topicMap.get(id);
    }

    public int getIdByTopic (String topic){
        for (Map.Entry<Integer, String> entry : topicMap.entrySet()) {
            Integer id = entry.getKey();
            String top = entry.getValue();
            if(topic.equals(top))
                return id;
        }
        return -1;
    }

    public ConcurrentHashMap<Integer, String > getTopicMap(){
        return topicMap;
    }
    public void clearTopicMap(){
        topicMap.clear();
    }
}
