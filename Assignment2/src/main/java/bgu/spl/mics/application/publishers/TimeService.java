package bgu.spl.mics.application.publishers;

import bgu.spl.mics.Publisher;
import bgu.spl.mics.application.messages.TerminateBroadcast;
import bgu.spl.mics.application.messages.TickBroadcast;

/**
 * TimeService is the global system timer There is only one instance of this Publisher.
 * It keeps track of the amount of ticks passed since initialization and notifies
 * all other subscribers about the current time tick using {@link TickBroadcast}.
 * This class may not hold references for objects which it is not responsible for.
 * 
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class TimeService extends Publisher {

	private int tick;
	private int duration;

	public TimeService(int duration) {
		super("TimeService");
		tick = 0;
		this.duration = duration;
	}

	@Override
	protected void initialize() {}

	@Override
	public void run() {
		while (tick < duration) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			tick++;
			getSimplePublisher().sendBroadcast(new TickBroadcast(tick));
		}
		getSimplePublisher().sendBroadcast(new TerminateBroadcast());
	}

}
