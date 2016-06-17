import java.awt.geom.Point2D;
import java.util.*;

/**
 * Created by adi on 4/29/16.
 */
public class GeneticAlgorithm {
    public void setR(Route r) {
        this.r = r;
        maxLen = r.getLength();
    }

    public Route getR() {
        return r;
    }

    private Route r;

    public double getMaxLen() {
        return maxLen;
    }

    private double maxLen;
    private int populationSize = 100;
    private int selectionSize = 10;
    private int numOfIter = 1000;
    private Random random = new Random();
    private double mutationRate = 0.04;
    //public GeneticAlgorithm(Route r) {
    //    this.r = r;
    //}


    public int getPopulationSize() {
        return populationSize;
    }

    public void setPopulationSize(int populationSize) {
        this.populationSize = populationSize;
    }

    public int getNumOfIter() {
        return numOfIter;
    }

    public void setNumOfIter(int numOfIter) {
        this.numOfIter = numOfIter;
    }

    public double getMutationRate() {
        return mutationRate;
    }

    public void setMutationRate(double mutationRate) {
        this.mutationRate = mutationRate;
    }

    public Route route(){
        LinkedList<Route> population = new LinkedList<>();

        //Route next = new Route( new LinkedList<>());
        LinkedList<Point2D> toSuffle = new LinkedList<>();
        LinkedList<Point2D> tmp = new LinkedList<>();
        for (Point2D p : r.getComList())
            toSuffle.add(p);
        for ( int i = 0; i < populationSize; ++i) {
           // Collections.copy(toSuffle, r.getComList());

            //Collections.shuffle(next.getComList());
            //tmp = new LinkedList<>();
            Collections.shuffle(toSuffle);
            tmp.addAll(toSuffle);
            population.add(new Route(tmp));
            tmp.clear();
        }

        for ( int i = 0; i < numOfIter; ++i)
            population = nextPopulation( population );

        return r;
    }

    private LinkedList<Route> nextPopulation(LinkedList<Route> population) {

        //SELECTION
        for ( Route r: population
             ) {
            mutation(r.getComList());
        }
        LinkedList<Route> selection = selection(population);
        //Collections.sort(population);
        //selection = (LinkedList<Route>)population.subList(0, selectionSize);
        return crossover(selection);

    }

    private LinkedList<Route> selection(LinkedList<Route> population) {
        LinkedList<Route> selection = new LinkedList<>();
        Collections.sort(population);
        //Collections.min(population);
        //System.out.println(population.getFirst().getLength());
        if(population.get(0).getLength() < r.getLength())
            r = population.get(0);
        if(population.getLast().getLength() > maxLen){
            maxLen = population.getLast().getLength();
        }
        double maxLen = population.getLast().getLength();
        /*for ( Route r : population) {
            if ( r.getLength() > maxLen ) maxLen = r.getLength();
        }*/

        LinkedList<Double> fitnes = new LinkedList<>(/*population.size()*/);

        for ( Route r : population) {
            fitnes.add(r.getLength()/maxLen);
        }

        for (int i = 0; i < fitnes.size(); ++i)
            for ( int j = i+1; j < fitnes.size(); ++j )
                fitnes.set(i,fitnes.get(i)+fitnes.get(j));
        double r;
        for ( int i = 0; i < populationSize; ++i) {
            r = random.nextDouble() * fitnes.size();
            int j = population.size() - 1;
            while ( j > 0 && fitnes.get(j) < r )
                --j;
            selection.add(population.get(j));
            //fitnes.set(j,-1.);
        }
        return selection;
    }

    private LinkedList<Route> crossover(LinkedList<Route> population){
        LinkedList<Route> result = new LinkedList<>(/*population.size()*/);

        Iterator<Route> populationIterator = population.iterator();
        Route parent1, parent2;
        while ( populationIterator.hasNext()) {
            parent1 = populationIterator.next();
            populationIterator.hasNext();
            parent2 = populationIterator.next();
            Iterator<Point2D> ip1 = parent1.getComList().iterator();
            Iterator<Point2D> ip2 = parent2.getComList().iterator();
            LinkedHashSet<Point2D> child1 = new LinkedHashSet<>();
            LinkedHashSet<Point2D> child2 = new LinkedHashSet<>();
            for ( int i = 0; (i < parent1.getComList().size() / 2) & ip1.hasNext() & ip2.hasNext() ; ++i) {
                child1.add(ip1.next());
                child2.add(ip2.next());
            }
            ip1 = parent1.getComList().iterator();
            ip2 = parent2.getComList().iterator();
            for ( int i = 0; ip1.hasNext() & ip2.hasNext() ; ++i) {
                child1.add(ip2.next());
                child2.add(ip1.next());
            }
            result.add(new Route(child1));
            result.add(new Route(child2));
        }
        return result;
    }

    public void mutation( LinkedHashSet<Point2D> r) {
        if ( random.nextDouble() < mutationRate ) {

            LinkedList<Point2D> tmpList = new LinkedList<>();
            tmpList.addAll(r);
            int i1 = random.nextInt(r.size());
            int i2 = random.nextInt(r.size());

            Point2D tmp = tmpList.get(i1);

            tmpList.set(i1,tmpList.get(i2));
            tmpList.set(i2,tmp);
            r.clear();
            r.addAll(tmpList);

        }
    }
}
