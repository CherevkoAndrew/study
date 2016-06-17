import javax.swing.*;
import java.util.Date;
import java.util.jar.JarFile;

/**
 * Created by adi on 5/26/16.
 */
public class FlightFrame extends JFrame {
    public FlightFrame () {
        add(new FlightList());
        pack();
    }

    public void showFlightInfo(Date date, String flightID ) {
        System.out.println(date.toString() + flightID);
    }
}
