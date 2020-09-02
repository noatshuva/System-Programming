package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.MissionReceivedEvent;
import bgu.spl.mics.application.messages.TerminateBroadcast;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.passiveObjects.MissionInfo;

import java.util.Comparator;
import java.util.LinkedList;

/**
 * A Publisher\Subscriber.
 * Holds a list of Info objects and sends them
 *
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Intelligence extends Subscriber {

	private LinkedList<MissionInfo> mis = new LinkedList<>();
	private int tick;

	public Intelligence(int id,LinkedList<MissionInfo> lst) {
		super("Intelligence" + id);
		mis = lst;
		mis.sort(Comparator.comparingInt(MissionInfo::getTimeIssued)); //sort the mission by there timeIssued
		tick = 0;
	}

	@Override
	protected void initialize() {

		subscribeBroadcast(TerminateBroadcast.class, (t) ->{
			terminate();
		});

		subscribeBroadcast(TickBroadcast.class,(b)->{
			tick = b.getTick();
			for (int i = 0; i < mis.size(); i++) {
				MissionInfo mi = mis.get(i);
					if (mi.getTimeIssued() == tick) {
						getSimplePublisher().sendEvent(new MissionReceivedEvent(mi));
					}
			}
		});

	}
}
