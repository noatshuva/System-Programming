package bgu.spl.mics.application;

import bgu.spl.mics.application.passiveObjects.*;
import bgu.spl.mics.application.publishers.TimeService;
import bgu.spl.mics.application.subscribers.Intelligence;
import bgu.spl.mics.application.subscribers.M;
import bgu.spl.mics.application.subscribers.Moneypenny;
import bgu.spl.mics.application.subscribers.Q;
import com.google.gson.Gson;
import com.google.gson.stream.JsonReader;

import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.LinkedList;

/** This is the Main class of the application. You should parse the input file,
 * create the different instances of the objects, and run the system.
 * In the end, you should output serialized objects.
 */
public class MI6Runner {
    public static void main(String[] args) throws IOException, InterruptedException {

        //read from json file
        Gson gson = new Gson();
        JsonReader jsonReader = new JsonReader(new FileReader((args[0])));
        GsonObject object = gson.fromJson(jsonReader, GsonObject.class);

        LinkedList<Thread> threads = new LinkedList<>();

        //create inventory
        Inventory inventory = Inventory.getInstance();
        inventory.load(object.inventory);

        //add agents to squad
        Squad squad = Squad.getInstance();
        Agent[] agents = new Agent[object.squad.length];
        for (int i = 0; i < object.squad.length; i++) {
            agents[i] = new Agent(object.squad[i].getSerialNumber(), object.squad[i].getName());
        }
        squad.load(agents);

        //create intelligence
        for (int i = 0; i < object.services.intelligence.length ; i++) {
            LinkedList<MissionInfo> mis = new LinkedList<>();
            for (int j = 0; j < object.services.intelligence[i].missions.length; j++) {
                MissionInfo missionInfo = new MissionInfo();
                missionInfo.setSerialAgentsNumbers(Arrays.asList(object.services.intelligence[i].missions[j].serialAgentsNumbers));
                missionInfo.setDuration(object.services.intelligence[i].missions[j].duration);
                missionInfo.setGadget(object.services.intelligence[i].missions[j].gadget);
                missionInfo.setMissionName(object.services.intelligence[i].missions[j].name);
                missionInfo.setTimeExpired(object.services.intelligence[i].missions[j].timeExpired);
                missionInfo.setTimeIssued(object.services.intelligence[i].missions[j].timeIssued);
                mis.add(missionInfo);
            }
            Intelligence intelligence = new Intelligence(i,mis);
            Thread thread = new Thread(intelligence);
            threads.add(thread);
            thread.setName("intelligence" + i);
        }

        //create M
        for(int i = 1; i <= object.services.M; i++){
            M m = new M(i);
            Thread thread = new Thread(m);
            threads.add(thread);
            thread.setName("M" + i);
        }

        //create MoneyPenny
        for (int i = 1; i <= object.services.Moneypenny ; i++) {
            Moneypenny moneypenny = new Moneypenny(i);
            Thread thread = new Thread(moneypenny);
            threads.add(thread);
            thread.setName("MP" + i);
        }

        //create Q
        Q q = new Q(1);
        Thread t = new Thread(q);
        threads.add(t);
        t.setName("Q");

        //create TimeService
        TimeService timeService = new TimeService(object.services.time);
        Thread time = new Thread(timeService);
        time.setName("time");

        Initialize initialize = Initialize.getInstance();

        for (Thread thread : threads)
            thread.start();

        //make sure we dont start the timeService before all the threads finished initialization
        while (initialize.getCounter().get() != threads.size()){}

        time.start();

        threads.add(time);

        for(Thread thread : threads)
            thread.join();

        //create Diary
        Diary diary = Diary.getInstance();

        diary.printToFile(args[2]);
        inventory.printToFile(args[1]);
    }
}
