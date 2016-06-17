import java.util.Comparator;

/**
 * Created by adi on 4/29/16.
 */
public class RouteComparator implements Comparator<Route> {
    @Override
    public int compare(Route o1, Route o2) {
        return (int)((o1.getLength() - o2.getLength()) * 100 ) ;
    }
}
