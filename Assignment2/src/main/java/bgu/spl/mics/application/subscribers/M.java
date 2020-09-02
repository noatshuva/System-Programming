package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.*;
import bgu.spl.mics.application.messages.*;
import bgu.spl.mics.application.passiveObjects.Diary;
import bgu.spl.mics.application.passiveObjects.MissionInfo;
import bgu.spl.mics.application.passiveObjects.Pair;
import bgu.spl.mics.application.passiveObjects.Report;

import java.util.List;


/**
 * M handles ReadyEvent - fills a report and sends agents to mission.
 *
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class M extends Subscriber {

	private Diary diary;
	private int tick;
	MissionInfo mi;
	int id;


	public M(int id) {
		super("M"+id);
		diary = Diary.getInstance();
		tick = 0;
		mi = new MissionInfo();
		this.id = id;
	}

	@Override
	protected void initialize() {
		subscribeBroadcast(TerminateBroadcast.class, (t) ->{
			terminate();
		});

		subscribeBroadcast(TickBroadcast.class, (t) ->{
			tick = t.getTick();
		});

		subscribeEvent(MissionReceivedEvent.class, (m) -> { //M receives the event m
			diary.increment();
			int timeCreate = tick;
			mi = m.getMi(); //mission info received for m
			AgentsAvailableEvent aae = new AgentsAvailableEvent(mi.getSerialAgentsNumbers());
			Future<Pair<Integer,List<String>>> agentAvl = getSimplePublisher().sendEvent(aae); //check if agents are available

			if(agentAvl != null && agentAvl.get() != null && agentAvl.get().getFirst() != -1 ) {
				Future<Integer> gadAvl = getSimplePublisher().sendEvent(new GadgetAvailableEvent(mi.getGadget())); //check if gadgets are available
				if(gadAvl != null) {
					int Qtime = gadAvl.get();
					if (mi.getTimeExpired() > Qtime && Qtime != -1) {
						aae.canSendThem().resolve(mi.getDuration()); //we need that the MP will stay in hold for this mission in order to prevent deadlocks

						Report report = new Report();
						report.setM(id);
						report.setAgentsSerialNumbers(mi.getSerialAgentsNumbers());
						report.setTimeIssued(mi.getTimeIssued());
						report.setGadgetName(mi.getGadget());
						report.setTimeCreated(timeCreate);
						report.setMissionName(mi.getMissionName());
						report.setMoneypenny(agentAvl.get().getFirst());
						report.setAgentsNames(agentAvl.get().getSecond());
						report.setQTime(Qtime);
						diary.addReport(report);

						complete(m, null);
						return; //to prevent releasing the agents before mission completed
					}
				}
			}
			aae.canSendThem().resolve(-1);

		});

	}

}
