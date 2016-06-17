import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

/**
 * Created by adi on 5/23/16.
 */
public class StartPanel extends JPanel{
    private JButton create;
    private JButton connect;

    JTextField hostInput = new JTextField("host", 30);
    JTextField portInput = new JTextField("port", 4);

    private String host;
    private int port;

    public StartPanel() {
        create = new JButton("creeate game");
        connect = new JButton("connect");

        create.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    GameRunner.createGame(Integer.parseInt(portInput.getText()));
                } catch (IOException e1)
                {
                    e1.printStackTrace();
                }
            }
        });

        connect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    GameRunner.connect(hostInput.getText(), Integer.parseInt(portInput.getText()));
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            }
        });

        add(hostInput);
        add(portInput);
        add(create);
        add(connect);
    }


}
