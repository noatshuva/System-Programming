package bgu.spl.mics.application.messages;

import bgu.spl.mics.Event;
import bgu.spl.mics.application.passiveObjects.MissionInfo;

public class MissionReceivedEvent implements Event<Void> {
    private MissionInfo mi;

    public MissionReceivedEvent(MissionInfo info) {
        this.mi = info;
    }

    public MissionInfo getMi(){ return mi; }


}
