package bgu.spl.net.impl.stomp.frame.sent;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Supplier;

public class MESSAGE extends Frame_Sent {
    private static final AtomicInteger runningID = new AtomicInteger(20);
    private static final Supplier<String> id_supplier = () -> fill(runningID.incrementAndGet());
    private String id;

    public MESSAGE() {
        id = id_supplier.get();
        addHeader("subscription", "");
        addHeader("Message-id", id);
    }

    private static String fill(int num) {
        StringBuilder str = new StringBuilder(num + "");
        while (str.length() < 5)
            str.insert(0, "0");
        return str.toString();
    }
}
