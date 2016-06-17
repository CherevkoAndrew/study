/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package com.very.simple;

/**
 *
 * @author Денис
 */
public class Runner {
    public static void main(String[] args) throws InterruptedException {
        Queue queue1 = new Queue();
        Queue queue2 = new Queue();
        Queue queue3 = new Queue();

        int maxTime = 500;
        int minTime = 100;

        Consumer c = new Consumer(queue1,queue2,queue3);
        c.start();
        //Thread.sleep(20);
        new Producer(1,c, minTime, maxTime).start();
        new Producer(2,c, minTime, maxTime).start();
        System.out.println("main is finished");

    }
    
}
