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

    private Queue q1;
    private Queue q2;
    private Queue q3;
    private Proc runningProc;
    private boolean isBusy = true;
    private boolean free = true;
    private Object busyLock = new Object();
    private Random random = new Random();
    int interruptedProcesses;
    int length1;
    int length2;
    int length3;


    public void setMaxTime(int maxTime) {
        this.maxTime = maxTime;
    }

    public void setMinTime(int minTime) {
        this.minTime = minTime;
    }

    private int maxTime = 500;
    private int minTime = 100;


    public Consumer(Queue q1, Queue q2, Queue q3) {
        this.q1 = q1;
        this.q2 = q2;
        this.q3 = q3;
    }

    public void run(){

        while(!interrupted()){
            if (free) {
                synchronized (busyLock) {

                    if (!q1.isEmpty()) {
                        runningProc = q1.pop();
                        System.out.println("run proc from queue 1 : " + runningProc.getId());
                    }
                    else if (!q3.isEmpty()) {
                        runningProc = q3.pop();
                        System.out.println("run proc from queue 3 : " + runningProc.getId());
                    }
                    else if (!q2.isEmpty()) {
                        runningProc = q2.pop();
                        System.out.println("run proc from queue 2 : " + runningProc.getId());
                    }
                    else {
                        isBusy = false;
                        try {
                            System.out.println("interrupted : " + interruptedProcesses + "; max length 1" + length1 +  "; max length 2" + length2 + "; max length 3" + length3);
                            busyLock.wait();
                            isBusy = true;
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            } else {
                free = true;
            }
               // System.out.println("value = " + runningProc.getId()+" "+runningProc.getType());
                try {
                    sleep(random.nextInt(maxTime - minTime) + minTime );
                } catch (InterruptedException ex) {
                    synchronized (busyLock) {
                        free = false;
                      //  System.out.println("interrupted " + Thread.currentThread().getId());
                       // System.out.println(Thread.currentThread());
                    }

                    // System.out.println("interrupted");
                    //return;
                }
            //wait();
        }
        //synchronized (busyLock) {}
    }

    public synchronized void addProc(Proc p) {
        synchronized ( busyLock ) {
            if (!isBusy) {
                isBusy = true;
                runningProc = p;
                System.out.println("running process : " + p.getId()+"   type : " + p.getType());
                busyLock.notifyAll();
                return;
            }
        }
        if ( p.getType() == 1) {
            //System.out.println("push1");
            if ( runningProc.getType() == 1) {
                q1.push(p);
                length1++;
                System.out.println("running process : not changed" + " ;  queue 1 push id : " + p.getId()+"  type : " + p.getType());
            } else {
                synchronized (busyLock) {
                    q3.push(runningProc);
                    length3++;
                    //System.out.println(Thread.currentThread().getId());
                    System.out.println("running process : " + p.getId()+"  type : " + p.getType() +";   queue 3 push id : " + runningProc.getId()+"  type : " + runningProc.getType());
                    interruptedProcesses++;
                    this.interrupt();
                    runningProc = p;
                }
            }

        } else {
            q2.push(p);
            length2++;
            //System.out.println("push2");
            System.out.println("running process : not changed" + ";   queue 2 push id : " + p.getId()+"  type : " + p.getType());
        }
    }
    
}
