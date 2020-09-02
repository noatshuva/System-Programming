package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.impl.stomp.frame.Frame;
import bgu.spl.net.impl.stomp.frame.received.Frame_Received;

import java.lang.reflect.Constructor;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

public class StompMessageEncoderDecoderImpl implements MessageEncoderDecoder<Frame> {

    private static final String package_received = "bgu.spl.net.impl.stomp.frame.received.";

    private LinkedList<Byte> bytes = new LinkedList<>();
    private int i = 0;

    @Override
    public Frame decodeNextByte(byte nextByte) {
        if (nextByte == '\0') {
            return popFrame();
        }
        bytes.add(nextByte);
        return null;
    }

    @Override
    public byte[] encode(Frame frame) {
        String ms = frame.toString();
        return (ms + '\0').getBytes();
    }

    private Frame popFrame() {
        byte[] bytes_arr = new byte[bytes.size()];
        bytes.forEach(b -> bytes_arr[i++] = b);
        String msgAsSTR = new String(bytes_arr, StandardCharsets.UTF_8);
        Frame_Received frame = str2frm(msgAsSTR);

        i = 0;
        bytes.clear();
        return frame;
    }

    @SuppressWarnings("unchecked")
    private static Frame_Received str2frm(String msgAsSTR) {
        Frame_Received frame = null;
        List<String> lines = makeLines(msgAsSTR);
        String clazzAsSTR = package_received + lines.remove(0);
        try {
            Class<? extends Frame_Received> clazz = (Class<? extends Frame_Received>) Class.forName(clazzAsSTR);
            Constructor<? extends Frame_Received> constructor = clazz.getConstructor();
            frame = constructor.newInstance();

            for (String line : lines) {
                String[] splitted = line.split(":", 2);

                String key = splitted[0];
                String value = splitted[1];

                frame.addHeader(key, value);
            }


        } catch (Exception ignored) {
        }
        return frame;
    }

    private static List<String> makeLines(String msg) {
        msg = msg.replaceAll("\n\r", "\r");
        msg = msg.replaceAll("\r\n", "\r");
        msg = msg.replaceAll("\n", "\r");
        msg = msg.replace("\r\r", "\rbody:");

        String[] linesArr = msg.split("\r");
        List<String> badlst = Arrays.asList(linesArr);
        return new LinkedList<>(badlst);
    }
}


















