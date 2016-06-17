/**
 * Created by adi on 4/30/16.
 */
public class Proc {
    public int getId() {
        return id;
    }

    public Proc( int type ) {
        this.id = lastId++;
        this.type = type;
        //System.out.println(type);
    }

    public int getType() {
        return type;
    }



    private int id;
    private int type;
    static int lastId;
}
