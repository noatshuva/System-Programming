package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {

        if (args.length == 0) {
            args = new String[]{"7777", "tpc"};
        }

        if (args.length < 2) {
            System.out.println("you must supply two arguments: port, server");
            System.exit(1);
        }
        Manager manager=new Manager();
        Server server = null;
        if(args[1].equals("tpc")) {
            server = Server.threadPerClient(Integer.parseInt(args[0]),
                    () -> new StompMessagingProtocolImpl(manager),
                    StompMessageEncoderDecoderImpl::new);
        }
        else if(args[1].equals("reactor")){
            server = Server.reactor(10, Integer.parseInt(args[0]),
                    () -> new StompMessagingProtocolImpl(manager),
                    StompMessageEncoderDecoderImpl::new);
        }
        server.serve();

    }


}
