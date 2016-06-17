import javax.swing.*;
import java.awt.*;
import java.awt.geom.Point2D;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.Random;

/**
 * Created by adi on 4/29/16.
 */
public class GaWindow {

    public static void main(String[] args) {

        LinkedHashSet<Point2D> r1 = new LinkedHashSet<>();
        int k = 50;
        Random random = new Random();
        for ( int i = 1; i < 50; ++i) {
            //for ( int j = 0; j < 100; ++j) {
                r1.add(new Point2D.Double(random.nextInt(500), random.nextInt(500)));
            //}
        }
        /*Route r = new Route(r1);
        //GeneticAlgorithm ga = new GeneticAlgorithm(r);
        System.out.println(r.getLength());
        Route r2 = ga.route();
        System.out.println(ga.route().getLength());*/

        EventQueue.invokeLater(new Runnable() {
            //private LinkedList<Point2D> comList = new LinkedList<>();
            //private Route r = new Route(new LinkedHashSet());
            public void run() {
                ////////////////////////

                ////////////////////////
                JFrame frame = new GaFrame();
                frame.setTitle("Genetic Algorithm");
                frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
                frame.setVisible(true);
            }
        });
    }

}
