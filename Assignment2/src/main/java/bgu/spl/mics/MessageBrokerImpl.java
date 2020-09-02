package bgu.spl.mics;

import bgu.spl.mics.application.messages.TerminateBroadcast;

import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * The {@link MessageBrokerImpl class is the implementation of the MessageBroker interface.
 * Write your implementation here!
 * Only private fields and methods can be added to this class.
 */
public class MessageBrokerImpl implements MessageBroker {
    ConcurrentHashMap<Class<? extends Message>, LinkedBlockingQueue<Subscriber>> eventMap;
    ConcurrentHashMap<Class<? extends Broadcast>, LinkedBlockingQueue<Subscriber>> broadMap;
    ConcurrentHashMap<Subscriber, LinkedBlockingQueue<Message>> subsMap;
    ConcurrentHashMap<Event, Future> futMap;

    /**
     * Retrieves the single instance of this class.
     */
    private MessageBrokerImpl() {
        eventMap = new ConcurrentHashMap<>();
        broadMap = new ConcurrentHashMap<>();
        subsMap = new ConcurrentHashMap<>();
        futMap = new ConcurrentHashMap<>();
    }

    public static MessageBroker getInstance() {
        return SingletonHolder.singleton;
    }

    private static class SingletonHolder {
        private static MessageBroker singleton = new MessageBrokerImpl();
    }


    @Override
    public <T> void subscribeEvent(Class<? extends Event<T>> type, Subscriber m) {
        if (eventMap != null) {
            eventMap.putIfAbsent(type, new LinkedBlockingQueue<Subscriber>()); //create a queue with the key type, if not there
            eventMap.get(type).add(m); //add the subscriber to the queue of the event type "type"
        }
    }

    @Override
    public void subscribeBroadcast(Class<? extends Broadcast> type, Subscriber m) {
        if(broadMap != null) {
            broadMap.putIfAbsent(type, new LinkedBlockingQueue<Subscriber>()); //creates a queue with the key type, if not there
            broadMap.get(type).add(m); //add the subscriber to the queue of the broadcast type "type"
        }
    }

    @Override
    public <T> void complete(Event<T> e, T result) {
        Future<T> f = futMap.get(e);
        f.resolve(result);
    }

    @Override
    public void sendBroadcast(Broadcast b) {
        LinkedBlockingQueue<Subscriber> subs = broadMap.get(b.getClass()); // takes the queue which holds all the subscribes which assign this broadcast
        if (subs != null) {
            for (Subscriber s : subs) {
                LinkedBlockingQueue<Message> msg = subsMap.get(s); //looking for the messages queue of the subscriber
                msg.add(b); //add the message to the queue
            }
        }
    }


    @Override
    public <T> Future<T> sendEvent(Event<T> e) {
		Future<T> f = new Future<>();
		Subscriber s;
		LinkedBlockingQueue<Subscriber> subs = eventMap.get(e.getClass()); //takes the queue which holds all the subscribes which assign this event
		if (subs == null) {
			return null;
		}
		synchronized (e.getClass()) { //there might be 2 events from the same class which try to do poll and there might be only 1 sub
			s = subs.poll(); //takes the first subscriber in the queue and removing it
			if (s == null) {


				return null;
			}
			subs.add(s); //add the subscriber back to the end of the queue
		}

		futMap.put(e, f);
		synchronized (s) { //#A there might be another thread which trying to remove s -->
			LinkedBlockingQueue<Message> msgs = subsMap.get(s); //looking for the messages queue of the subscriber
			if (msgs == null) {
				return null;
			}
			msgs.add(e); // #B --> in that case well add the event to a queue which we wont approach anymore and the event well never be resolved
		}
        return f;
    }

    @Override
    public void register(Subscriber m) { //create a queue for the subscriber m in the hash map
        subsMap.putIfAbsent(m, new LinkedBlockingQueue<>());
    }

    @Override
    public void unregister(Subscriber m) {
        for (Entry<Class<? extends Message>, LinkedBlockingQueue<Subscriber>> pair : eventMap.entrySet()) {
            synchronized (pair.getKey()) { //there might be a thread which trying to add to this sub queue
                pair.getValue().remove(m); //Removes the subscriber from this queue, if it is present.
            }
        }
        for (Entry<Class<? extends Broadcast>, LinkedBlockingQueue<Subscriber>> pair : broadMap.entrySet()) {
            synchronized (pair.getKey()) { //there might be a thread which trying to add to this sub queue
                pair.getValue().remove(m); //Removes the subscriber from this queue, if it is present.
            }
        }
		LinkedBlockingQueue<Message> msgQu;
        synchronized (m) { //there might be a thread which trying to add to this sub queue
			 msgQu = subsMap.remove(m);
		}
        for (Message msg : msgQu) {
            msgQu.remove(msg); //removes each msg from the message queue
            if (msg instanceof Event) {
                Event<?> e = (Event<?>) msg;
                Future<?> future = futMap.get(e);
                future.resolve(null); //if something using f.get() it will set him free
            }
        }
    }

    @Override
    public Message awaitMessage(Subscriber m) throws InterruptedException {
        return subsMap.get(m).take(); //retrieve and remove the head of this queue. If the queue is empty then it will wait until an element becomes available
    }


}

