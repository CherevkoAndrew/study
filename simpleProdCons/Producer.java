/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package com.very.simple;

import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Денис
 */
public class Producer extends Thread{
    //Queue queue;
    int id;
    Random random = new Random();
    int maxTime;
    int minTime;

    Consumer c;

    public Producer(int id, Consumer c, int minTime, int maxTime) {
        this.id = id;
        this.c = c;
        this.maxTime = maxTime;
        this.minTime = minTime;
    }

    //public Producer(Queue queue) {
    //    this.queue = queue;
    //}
    
    public void run(){
       // System.out.println(id);
        int procLeft = 10;
        while(!interrupted() && procLeft > 0){
            c.addProc( new Proc(id));

            procLeft--;
            try {
                sleep(random.nextInt(maxTime - minTime) + minTime );
            } catch (InterruptedException ex) {
                return ;
            }
        }
    }


    
    
}
