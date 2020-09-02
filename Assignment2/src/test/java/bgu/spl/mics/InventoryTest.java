package bgu.spl.mics;

import bgu.spl.mics.application.passiveObjects.Inventory;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.io.IOException;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.fail;

public class InventoryTest {
    Inventory inv;
    @BeforeEach
    public void setUp(){
        inv = Inventory.getInstance();
    }

    @Test
    public void test() throws IOException {
            Inventory inv2 = Inventory.getInstance();
            assertEquals(inv,inv2);
            String [] gadgets = {"g1", "g2", "g3", "g4"};
            inv.load(gadgets);
            assertFalse(inv.getItem("g5"));
            assertTrue(inv.getItem("g1"));
            inv.printToFile("inv.json");
            File file = new File("inv.json");
            assertTrue(file.exists());
    }
}
