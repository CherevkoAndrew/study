/**
 *
 * @author Денис
 */
public class Runner {
    public static void main(String[] args) throws InterruptedException {
        Queue queue = new Queue();
        new Producer(queue).start();
        Thread.sleep(20);
        new Consumer(queue).start();
        System.out.println("main is finished");

    }

}
