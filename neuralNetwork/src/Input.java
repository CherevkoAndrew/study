/**
 * Created by adi on 5/4/16.
 */
public class Input {
    public Input(int linksNum) {
        OutgoingLinks = new Link[linksNum];
        for ( int i = 0; i < linksNum; ++i) {
            OutgoingLinks[i] = new Link();
        }
    }

    public Link[] OutgoingLinks;
}
