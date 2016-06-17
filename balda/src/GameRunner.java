import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;
import java.util.Scanner;

import static java.lang.Thread.sleep;

/**
 * Created by adi on 5/23/16.
 */
public class GameRunner {
    private static char[][] letters;

    private static LinkedList<String> userWords = new LinkedList<>();
    private static LinkedList<String> opponentWords = new LinkedList<>();
    private static String startWord = "title";

    private static int localPoints;
    private static int remotePoints;
    private static boolean pass;
    private static boolean opponentPass;

    private static Game game;
    private static String host;
    private static int port;

    private static InputStream inStream;
    private static OutputStream outStream;

    private static PrintWriter out;
    private static Scanner in;

    private static GameFrame gameFrame;


    //private static Print

    public static void main(String[] args){
        EventQueue.invokeLater(new Runnable()
        {
            public void run()
            {
                StartFrame frame = new StartFrame();
                frame.setTitle("game");
                frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                frame.setVisible(true);
            }
        });

    }

    public static void createGame(int port) throws IOException {
        GameRunner.port = port;
        letters = new char[5][5];
        for (int i = 0; i < 5; ++i){
            letters[2][i] = startWord.charAt(i);
        }

      //  try (ServerSocket s = new ServerSocket(port)){
         //   try (Socket incoming = s.accept()){
          //      inStream = incoming.getInputStream();
            //    outStream = incoming.getOutputStream();

             //   in = new Scanner(inStream);
             //   out = new PrintWriter(outStream,true);
                //out.println("Hello");
                //receive(in.next());
                //step();
            //    //System.out.println("in::");
             //   Scanner cin = new Scanner(System.in);
                //out.println(cin.next());

                //create area
               // Runnable r =
        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                try {
                    ServerSocket s = new ServerSocket(port);
                    Socket incoming = s.accept();
                    inStream = incoming.getInputStream();
                    outStream = incoming.getOutputStream();

                    in = new Scanner(inStream);
                    out = new PrintWriter(outStream, true);

                    out.println(startWord);

                    Scanner cin = new Scanner(System.in);

                    gameFrame = new GameFrame(startWord);
                    gameFrame.setTitle("game");
                    gameFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                    gameFrame.setVisible(true);

                    in.next();

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }9                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              


    public static void connect(String host, int port) throws IOException {
        GameRunner.port = port;
        GameRunner.host = host;

        letters = new char[5][5];

        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                try {
                    Socket s = new Socket(host, port);
                    //Socket incoming = s.accept();
                    inStream = s.getInputStream();
                    outStream = s.getOutputStream();

                    in = new Scanner(inStream);
                    out = new PrintWriter(outStream, true);

                   // Scanner cin = new Scanner(System.in);
                    startWord = in.next();
                    System.out.println(startWord);


                    gameFrame = new GameFrame(startWord);
                    gameFrame.setTitle("game");
                    gameFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                    gameFrame.setVisible(true);

                    //waitOpponent();

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }



    public static String addLetter() {
        return gameFrame.addLetter();
    }

    public static boolean send(LinkedList<int[]> nextWord, int[] position, char letter) {
        StringBuilder wordBuilder = new StringBuilder();
        letters[position[0]][position[1]] = letter;
        for (int[] pos : nextWord ) {
            wordBuilder.append(letters[pos[0]][pos[1]]);
        }
        String word = wordBuilder.toString();
        if (word.equals(startWord))
            return false;
        for (String s : userWords)
            if (s.equals(word))
                return false;
        for (String s : opponentWords)
            if (s.equals(word))
                return false;
        //SEND INFORMATION
        out.println(word);
        for (int[] pos : nextWord) {
            out.println(pos[0] + " " + pos[1]);
        }
        //out.println("/");
        out.println(position[0] + " " + position[1]);
        out.println(letter);
        localPoints += nextWord.size();
        System.out.println(remotePoints + " " + localPoints);
        pass = false;
        return true;
    }

    public static LinkedList<int[]> waitOpponent() {
        LinkedList<int[]> wordPos = new LinkedList<>();
        String inWord = in.next();
        if(!inWord.equals("pass")) {
            System.out.println("ready");
            opponentPass = false;
            remotePoints += inWord.length();
            System.out.println(remotePoints + " " + localPoints);
            for (int i = 0; i < inWord.length(); ++i) {
                wordPos.add(new int[]{in.nextInt(), in.nextInt()});
            }
            int[] position = new int[]{in.nextInt(), in.nextInt()};
            char ch = in.next().charAt(0);

            opponentWords.add(inWord);
            letters[position[0]][position[1]] = ch;

            gameFrame.newLetter(position[0], position[1], ch);
        } else {
            opponentPass = true;
            if(pass) {
                gameFrame.over(localPoints, remotePoints);
            }
        }

        return wordPos;

    }

    public static void pass() {
        out.println("pass");
        if (opponentPass) {
            gameFrame.over(localPoints, remotePoints);
        } else {
            pass = true;
        }
    }
}

