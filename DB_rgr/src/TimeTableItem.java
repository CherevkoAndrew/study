import java.util.Date;

/**
 * Created by adi on 5/26/16.
 */
public class TimeTableItem {
    public TimeTableItem(Date time, String flightID) {
        this.time = time;
        this.flightID = flightID;
    }

    public Date getTime() {
        return time;
    }

    Date time;

    public String getFlightID() {
        return flightID;
    }

    @Override
    public String toString() {
        return time +
                " " + flightID;
    }

    String flightID;
}
