/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package com.very.simple;

import java.util.LinkedList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Денис
 */
public class Queue {
    private LinkedList<Proc> list = new LinkedList<>();
    //private int limit = 10;
    public synchronized void push(Proc p) {
        /*while( list.size() >= limit ){
            try {
                wait();
            } catch (InterruptedException ex) {
                Logger.getLogger(Queue.class.getName()).log(Level.SEVERE, null, ex);
            }
        }*/
        list.add(p);
      //  notify();
        
    }
    
    public synchronized Proc pop(){
       /* while( list.size() == 0 ){
            try {
                wait();
            } catch (InterruptedException ex) {
                Logger.getLogger(Queue.class.getName()).log(Level.SEVERE, null, ex);
            }
        }*/

        return list.pop();
        
    }

    public synchronized boolean isEmpty(){
        return list.isEmpty();
    }
}
