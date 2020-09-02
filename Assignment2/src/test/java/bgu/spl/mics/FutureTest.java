package bgu.spl.mics;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.concurrent.TimeUnit;

import static org.junit.jupiter.api.Assertions.*;

public class FutureTest {
    Future<String> future;
    @BeforeEach
    public void setUp(){
        future = new Future<>();
    }

    @Test
    public void test(){
        String string = "kim";
        future.resolve(string);
        assertEquals(string, future.get());
        assertTrue(future.isDone());
        String ans = future.get(5, TimeUnit.MINUTES);
        assertEquals("kim", ans);
    }
}
