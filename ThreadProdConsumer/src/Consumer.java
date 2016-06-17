/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package com.very.simple;

import static java.lang.Thread.interrupted;
import static java.lang.Thread.sleep;
import java.util.Random;

/**
 *
 * @author Денис
 */
public class Consumer extends Thread{

    Queue queue;
    /*
    Queue queue1;
    Queue queue2;
    Queue queue3;
     */

    public Consumer(Queue queue) {
        this.queue = queue;
    }

    public void run(){
        while(!interrupted()){
            Integer i = queue.pop();
            System.out.println("value = "+ i);
            try {
                sleep(800);
            } catch (InterruptedException ex) {
                return ;
            }
        }
    }

    public boolean tryTake( Process p ) {

        return false;
    }


}
