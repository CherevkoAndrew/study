import com.sun.org.apache.xerces.internal.impl.xpath.regex.Match;
import com.sun.org.apache.xml.internal.utils.StringBufferPool;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.CompoundBorder;
import javax.swing.border.EmptyBorder;
import javax.swing.border.LineBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;
import java.util.LinkedList;

/**
 * Created by adi on 5/23/16.
 */
public class GamePanel extends JPanel {
    private JButton[][] gameArea;
    private JButton ok;
    private JButton pass;
    private JButton clear;

    private LinkedList<int[]> nextWord = new LinkedList<>();
    private JPanel panel;
    private int[] letterPosition;
    private boolean letterAdded;
    private boolean letterUsed;
    private char letter;

    public GamePanel(String word){
 //       nextWord.add(new int[]{1,2});
        letterPosition = new int[2];
        setLayout(new BorderLayout());

        panel = new JPanel();
        panel.setLayout(new GridLayout(5,5));

        gameArea = new JButton[5][5];

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                gameArea[i][j] = new JButton("");
                gameArea[i][j].setForeground(Color.BLACK);
                gameArea[i][j].setBackground(Color.WHITE);

                Border line = new LineBorder(Color.BLACK);
                Border margin = new EmptyBorder(5, 15, 5, 15);
                Border compound = new CompoundBorder(line, margin);
                gameArea[i][j].setBorder(compound);

                gameArea[i][j].setPreferredSize(new Dimension(50,50));

                gameArea[i][j].addActionListener(new stepListener(i,j));
                panel.add(gameArea[i][j]);
            }
        }
        for (int i = 0; i < 5; ++i) {
            gameArea[2][i].setText(""+word.charAt(i));
        }
        add(panel,BorderLayout.CENTER);

        JPanel buttonsPane = new JPanel();

        ok = new JButton("Ok");
        pass = new JButton("pass");
        clear = new JButton("clear");

        ok.addActionListener(new okListener());
        pass.addActionListener(new passListener());
        clear.addActionListener(new cancelListener());

        buttonsPane.add(ok);
        buttonsPane.add(pass);
        buttonsPane.add(clear);

        ok.setEnabled(false);

        add(buttonsPane, BorderLayout.SOUTH);

    }

    public void newLetter(int i, int j, char ch) {
        gameArea[i][j].setText(""+ch);
    }

    private class stepListener implements ActionListener {

        private int i;
        private int j;
        boolean empty;

        public stepListener(int i, int j) {
            this.i = i;
            this.j = j;
        }
        @Override
        public void actionPerformed(ActionEvent e) {
            //gameArea[i][j].setBackground(Color.CYAN);
            if (!letterAdded) {
                if(gameArea[i][j].getText().isEmpty()) {
                    String s = GameRunner.addLetter();
                    if (s != null && !s.isEmpty()) {
                        gameArea[i][j].setBackground(Color.LIGHT_GRAY);
                        letterAdded = true;
                        letterPosition[0] = i;
                        letterPosition[1] = j;
                        gameArea[i][j].setText(s);
                        letter = s.charAt(0);
                    }
                }
            } else if (!gameArea[i][j].getText().isEmpty()) {
                if (nextWord.isEmpty() || ((Math.abs(nextWord.getLast()[0] - i) == 1 && nextWord.getLast()[1] == j) ^ (Math.abs(nextWord.getLast()[1] - j) == 1 && nextWord.getLast()[0] == i))) {
                    gameArea[i][j].setBackground(Color.CYAN);
                    nextWord.add(new int[]{i, j});

                    if (Arrays.equals(new int[]{i, j}, letterPosition)) {
                        letterUsed = true;
                        ok.setEnabled(true);
                    }
                }
            }
        }
    }

    private class okListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            if(GameRunner.send(nextWord, letterPosition, letter)) {
                nextWord.clear();
                letterAdded = false;
                letterUsed = false;

                LinkedList<int[]> oppWord = GameRunner.waitOpponent();

                for (int i = 0; i < 5; ++i) {
                    for (int j = 0; j < 5; ++j) {
                        gameArea[i][j].setBackground(Color.WHITE);
                    }
                }

                for (int[] pos : oppWord ){
                    gameArea[pos[0]][pos[1]].setBackground(Color.YELLOW);
                }
            }
        }
    }

    private class passListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {

            GameRunner.pass();
            nextWord.clear();
            letterAdded = false;
            letterUsed = false;

            LinkedList<int[]> oppWord = GameRunner.waitOpponent();

            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    gameArea[i][j].setBackground(Color.WHITE);
                }
            }

            for (int[] pos : oppWord ){
                gameArea[pos[0]][pos[1]].setBackground(Color.YELLOW);
            }
        }
    }

    private class cancelListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            nextWord.clear();
            if(letterAdded) {
                letterAdded = false;
                letterUsed = false;
                gameArea[letterPosition[0]][letterPosition[1]].setText("");
            }

            for (int i = 0 ; i < 5; ++i){
                for (int j = 0; j < 5; ++j){
                    gameArea[i][j].setBackground(Color.WHITE);
                }
            }
        }
    }
    private void setLetter(int i, int j, char ch) {

    }
}

