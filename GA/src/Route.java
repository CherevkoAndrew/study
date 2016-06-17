import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.Map;
import java.util.regex.Matcher;

/**
 * Created by adi on 4/29/16.
 */
public class Route implements Comparable<Route>{
    private LinkedHashSet<Point2D> comList;// = new LinkedHashSet<>();
    private double length;

    public Route(LinkedHashSet comList) {
        this.comList = comList;
        calcLength();
    }

    public Route(LinkedList<Point2D> tmp) {
        comList = new LinkedHashSet<>();
        comList.addAll(tmp);
        calcLength();
    }

    public LinkedHashSet<Point2D> getComList() {
        return comList;
    }

    public void setComList(LinkedHashSet<Point2D> comList) {
        this.comList = comList;
        calcLength();
    }

    public double getLength() {
        return length;
    }

    double calcLength() {
        double result = 0;
        Iterator<Point2D> iterator = comList.iterator();
        if (!iterator.hasNext()) return 0;
        Point2D prev = iterator.next();
        Point2D next;
        while ( iterator.hasNext()) {
            next = iterator.next();
            result += Math.sqrt( Math.pow(next.getX() - prev.getX(),2) + Math.pow(next.getY() - prev.getY(),2));
            prev = next;
        }
        length = result;
        return result;
    }

    @Override
    public int compareTo(Route o) {
        return length > o.getLength() ? 1 : length < o.getLength() ? -1 : 0;
    }
}
