/**
 * Created by adi on 5/4/16.
 */
public class Neuron {
    //Все входы нейрона
    public Link[] IncomingLinks;

    public Neuron(int inputsNum) {
        IncomingLinks = new Link[inputsNum];
    }


    // Накопленный нейроном заряд
    public double Power;
}
