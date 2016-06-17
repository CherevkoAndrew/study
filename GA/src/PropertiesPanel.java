import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Created by adi on 5/9/16.
 */
public class PropertiesPanel extends JPanel {
    private JButton runButton;
    private JButton clearButton;
    private JTextField numIter;
    private JTextField populationSize;
    private JTextField mutationRate;
    private GaFrame frame;
    private JPanel paramPanel;
    private JPanel randPanel;
    private JTextField pointsAmount;
    private JButton randButton;
    private JPanel results;
    private JLabel min;
    private JLabel max;

    public PropertiesPanel(GaFrame gaFrame) {
        setLayout(new BorderLayout());
        this.frame = gaFrame;
        results = new JPanel();
        paramPanel = new JPanel();
        randPanel = new JPanel();
        paramPanel.setLayout(new FlowLayout());

        min = new JLabel("");
        max = new JLabel("");

        results.add(max);
        results.add(min);

        runButton = new JButton("run");
        runButton.addActionListener(new RunListener());
        //setBackground(Color.BLACK);
        clearButton = new JButton("clear");
        clearButton.addActionListener(new ClearListener());

        numIter = new JTextField(10);
        populationSize = new JTextField(10);
        mutationRate = new JTextField(10);

        paramPanel.add(new JLabel("num of iterations"));
        paramPanel.add(numIter);
        paramPanel.add(new JLabel("population size"));
        paramPanel.add(populationSize);
        paramPanel.add(new JLabel("mutation rate"));
        paramPanel.add(mutationRate);

        //randPanel.add
        paramPanel.add(runButton);
        paramPanel.add(clearButton);


        pointsAmount = new JTextField(10);
        randButton = new JButton("random");
        randButton.addActionListener(new RandListener());
        randPanel.add(pointsAmount);
        randPanel.add(randButton);

        add(results, BorderLayout.NORTH);
        add(paramPanel,BorderLayout.CENTER);
        add(randPanel,BorderLayout.SOUTH);

    }


    private class RunListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            frame.getGa().setMutationRate(Double.parseDouble(mutationRate.getText()));
            frame.getGa().setNumOfIter(Integer.parseInt(numIter.getText()));
            frame.getGa().setPopulationSize(Integer.parseInt(populationSize.getText()));
            frame.count();
            max.setText("max length = " + frame.getGa().getMaxLen());
            min.setText("min length = " + frame.getGa().getR().getLength());
        }
    }

    private class ClearListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            frame.clear();
        }
    }

    private class RandListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            frame.rand(Integer.parseInt(pointsAmount.getText()));
        }
    }

    //public Dimension getPreferredSize() {
      //  return new Dimension(500,200);
    //}
}