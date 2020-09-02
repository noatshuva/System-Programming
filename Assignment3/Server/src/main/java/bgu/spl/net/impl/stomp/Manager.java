package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.User;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class Manager {

    private ConcurrentHashMap<Integer, User> nameIdMap;
    private ReadWriteLock RWL = new ReentrantReadWriteLock();

    public Manager() {
        this.nameIdMap = new ConcurrentHashMap<>();
    }

    public User getUserByConnectionId(int id) {
        return nameIdMap.get(id);
    }

    public boolean userExists(String username) {
        return getUser(username) != null;
    }

    public User getUser(String username) { //return user by username
        User result = null;

        lock_Read_lock();//beg: a(1,2)
        Map<Integer,User> tmpmap = new HashMap<>(nameIdMap);
        lock_Read_release();//end: a(1,2)

        for (User user : tmpmap.values()) {
            if (user.getUserName().equals(username))
                result = user;
        }

        return result;
    }

    public void removeUser(int id) {
        lock_Write_lock();//beg: a(1)
        nameIdMap.remove(id);
        lock_Write_release();//end: a(1)
    }

    public void addIfAbsent(int id, User user) {
        lock_Write_lock();//beg: a(2)
        nameIdMap.putIfAbsent(id, user);
        lock_Write_release();//end: a(2)
    }


    private void lock_Read_lock() {
        RWL.readLock().lock();
    }

    private void lock_Write_lock() {
        RWL.writeLock().lock();
    }

    private void lock_Read_release() {
        RWL.readLock().unlock();
    }

    private void lock_Write_release() {
        RWL.writeLock().unlock();
    }
}
