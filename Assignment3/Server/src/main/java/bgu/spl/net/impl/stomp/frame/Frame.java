package bgu.spl.net.impl.stomp.frame;

import java.util.LinkedHashMap;
import java.util.Map;

public abstract class Frame {
    private LinkedHashMap<String, String> map = new LinkedHashMap<>();

    public String getBody() {
        String body = map.get("body");
        if (body == null) {
            return "";
        }
        return body;
    }

    public Map<String, String> getHeaders() {
        return map;
    }

    public void addHeader(String key, String value) {
        map.put(key, value);
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder(this.getClass().getSimpleName() + "\n");
        this.getHeaders().forEach((key, value) -> {
            result.append(key).append(":").append(value);
            if (!key.equals("body")) {
                result.append("\n");
            }
        });
        String str = result.toString();
        str = str.replace("body:", "");
        str = str + ("\n");

        return str;
    }
}
