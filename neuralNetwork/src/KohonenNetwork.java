import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

/**
 * Created by adi on 5/4/16.
 */
public class KohonenNetwork {
    public Input[] getInputs() {
        return inputs;
    }

    private Input[] inputs;
    private Neuron[] neurons;
    public static String key = "ABCDEFGHIJKL";
    //private int neuronsN

    public KohonenNetwork(int neuronsNum, int inputsNum) {
        this.inputs = new Input[inputsNum];
        this.neurons = new Neuron[neuronsNum];

        for ( int i = 0; i < neuronsNum; ++i) {
            neurons[i] = new Neuron(inputsNum);
        }

        for ( int i = 0; i < inputsNum; ++i) {
            inputs[i] = new Input(neuronsNum);
            for ( int j = 0; j < neuronsNum; ++j) {
                inputs[i].OutgoingLinks[j].neuron = neurons[j];
                neurons[j].IncomingLinks[i] = inputs[i].OutgoingLinks[j];
            }
        }
    }

    public int Handle(int[] input) {
        for (int i = 0; i < inputs.length; i++)
        {
            Input inputNeuron = inputs[i];
            for (Link outgoingLink : inputNeuron.OutgoingLinks)
            {
                outgoingLink.neuron.Power += outgoingLink.Weight * input[i];
            }
        }
        int maxIndex = 0;
        for (int i = 0; i < neurons.length; i++)
        {
            System.out.println(key.charAt(i) + " : " + neurons[i].Power);
            if (neurons[i].Power > neurons[maxIndex].Power)
                maxIndex = i;
        }
        //снять импульс со всех нейронов:
        for (Neuron outputNeuron : neurons)
        {
            outputNeuron.Power = 0;
        }
        return maxIndex;
    }

    public void Study(int[] input, int correctAnswer)
    {
        Neuron neuron = neurons[correctAnswer];
        for (int i = 0; i < neuron.IncomingLinks.length; i++)
        {
            Link incomingLink = neuron.IncomingLinks[i];
            incomingLink.Weight = incomingLink.Weight + 0.5 * (input[i] - incomingLink.Weight);
        }
    }

    public static void main( String[] args ) {
        KohonenNetwork nn = new KohonenNetwork(6,2500);
        BufferedImage img = null;
        int[] data = new int[2500];
        File directory;// = new File("")
        File[] fList;
        try {
            for (int symbol = 0; symbol < 6; ++symbol) {

                fList = (new File("/home/adi/NN/study/"+symbol)).listFiles();
                for ( int example = 0; example < fList.length; ++example) {
                    //try {
                        img = ImageIO.read(fList[example]);

                        for (int i = 0; i < 50; ++i) {
                            for (int j = 0; j < 50; ++j) {
                             //   System.out.print(Integer.toHexString((img.getRGB(j, i) & 0x00)) + " ");
                                data[i * 50 + j] = img.getRGB(j, i) ;
                            }
                            //System.out.println("");
                            nn.Study(data,symbol);
                        }

                }
            }
        } catch (IOException e) {
            System.out.println("IO ERROR");
        }



        try {
            img = ImageIO.read(new File("/home/adi/NN/test.png"));

            for (int i = 0; i < 50; ++i) {
                for (int j = 0; j < 50; ++j) {
                   // System.out.print(Integer.toHexString(img.getRGB(j, i)) + " ");
                    data[i * 50 + j] = img.getRGB(j, i);
                }
               // System.out.println("");
            }
        } catch (IOException e) {
        }
        System.out.println("answer : " + key.charAt(nn.Handle(data)));
    }
}
