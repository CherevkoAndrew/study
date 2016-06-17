import javax.swing.*;
import java.awt.event.WindowEvent;

/**
 * Created by adi on 5/24/16.
 */
public class GameFrame extends JFrame {
    GamePanel p;
    public GameFrame(String startWord) {
        add(p = new GamePanel(startWord));
        pack();
    }

    public String addLetter() {
        return JOptionPane.showInputDialog("");
    }


    public void newLetter(int i, int j, char ch) {
        p.newLetter(i,  j, ch);
    }

    public void over(int localPoints, int remotePoints) {
        //System.out.println("over");
        String out;
        if(localPoints < remotePoints)
            out = "you lose";
        else if(localPoints > remotePoints)
            out = "you win";
        else
            out = "draw";
        JOptionPane.showMessageDialog(this,
                out + ": "+ localPoints + "-" + remotePoints);
        this.dispatchEvent(new WindowEvent(this, WindowEvent.WINDOW_CLOSING));
    }
}
