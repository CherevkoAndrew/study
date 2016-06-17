/**
 * Created by adi on 5/23/16.
 */
public class Game {
    private static char[][] letters = new char[5][5];
    //private int localPoints;
   // private int remotePoints;
    //boolean passs;
    //boolean over;

    public Game(String startWord){
        for ( int i = 0; i < 5; ++i) {
            letters[2][i] = startWord.charAt(i);
        }
    }

    public void move(int i, int j, char ch) {
        if (ch != '\0'){
            letters[i][j] = ch;
        }
    }
}
