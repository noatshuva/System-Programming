package bgu.spl.mics.application.messages;

import bgu.spl.mics.Event;

import java.util.List;

public class GadgetAvailableEvent implements Event<Integer>{

    private String gadget;

    public GadgetAvailableEvent (String gadget){
        this.gadget = gadget;
    }

    public String getGadget (){
        return gadget;
    }
}
