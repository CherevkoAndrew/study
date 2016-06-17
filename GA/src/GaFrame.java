import javax.swing.*;
import java.awt.*;
import java.awt.geom.Point2D;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.Random;

/**
 * Created by adi on 4/29/16.
 */
public class GaFrame extends JFrame {
    public GaFrame() {
        ga = new GeneticAlgorithm();
        points = new LinkedHashSet<>();
        borders = new JPanel();
        draw = new GaDrawComponent( points );
        panel = new PropertiesPanel(this);

        ga.setR(new Route(points));
        borders.setLayout(new BorderLayout());
        borders.add( draw, BorderLayout.CENTER);
        borders.add( panel, BorderLayout.SOUTH);
        add(borders);
        pack();
    }

    private JPanel borders;
    private GaDrawComponent draw;
    private PropertiesPanel panel;

    public GeneticAlgorithm getGa() {
        return ga;
    }

    private GeneticAlgorithm ga;
    private LinkedHashSet<Point2D> points;

    public void count() {
        draw.showLines = true;
        ga.setR(new Route(points));
        points = ga.route().getComList();
        draw.setRoute(points);
        draw.repaint();
    }

    public void clear() {
        draw.showLines = false;
        //ga.setR(new Route(points));
        //points = ga.route().getComList();

        points.clear();
        draw.setRoute(points);
        draw.repaint();
    }

    public void rand(int i) {
        draw.showLines = false;
        Random random = new Random();
        points.clear();
        for (int j = 0; j < i; ++j){
            points.add(new Point2D.Double(random.nextInt(draw.getWidth()),random.nextInt(draw.getHeight())));
        }

        draw.setRoute(points);
        draw.repaint();
    }
}
