package bgu.spl.mics.application;
import bgu.spl.mics.application.passiveObjects.Agent;

public class GsonObject {
    public String[] inventory;
    public Services services;
    public Agent[] squad;

    public class Services{
        public int M;
        public int Moneypenny;
        public Intelligences [] intelligence;
        public int time;

        public class Intelligences{
            public Missions [] missions;
            public class Missions{
                public String[] serialAgentsNumbers;
                public int duration;
                public String gadget;
                public String name;
                public int timeExpired;
                public int timeIssued;
            }
        }
    }
}
