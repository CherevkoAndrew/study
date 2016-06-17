/**
 * Created by adi on 5/26/16.
 */

import javax.swing.*;
import java.awt.*;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Date;
import java.util.LinkedList;

/**
 * Simple Java program to connect to MySQL database running on localhost and
 * running SELECT and INSERT query to retrieve and add data.
 * @author Javin Paul
 */
public class JavaToMySQL {

    // JDBC URL, username and password of MySQL server
    private static final String url = "jdbc:mysql://localhost:3306/cw";
    private static final String user = "root";
    private static final String password = "GroundControll";

    // JDBC variables for opening and managing connection
    private static Connection con;
    private static Statement stmt;
    private static ResultSet rs;

    public static void main(String args[]) {
      /*  String query = "select count(*) from S";

        try {
            // opening database connection to MySQL server
            con = DriverManager.getConnection(url, user, password);

            // getting Statement object to execute query
            stmt = con.createStatement();

            // executing SELECT query
            rs = stmt.executeQuery(query);

            while (rs.next()) {
                int count = rs.getInt(1);
                System.out.println("Total number of books in the table : " + count);
            }

        } catch (SQLException sqlEx) {
            sqlEx.printStackTrace();
        } finally {
            //close connection ,stmt and resultset here
           // try { con.close(); } catch(SQLException se) { /*can't do anything */
           // try { stmt.close(); } catch(SQLException se) { /*can't do anything */ }
           // try { rs.close(); } catch(SQLException se) { /*can't do anything */ }
      //  }*/

        EventQueue.invokeLater(new Runnable()
        {
            public void run()
            {
                FlightFrame frame = new FlightFrame();
                frame.setTitle("Calculator");
                frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                frame.setVisible(true);
            }
        });
    }

    public static LinkedList<TimeTableItem> readTimeTable() {
        String query = "select time, flight from time_table";
        LinkedList<TimeTableItem> result = new LinkedList<>();
        Date date;
        String flightId;
        try {
            // opening database connection to MySQL server
            con = DriverManager.getConnection(url, user, password);

            // getting Statement object to execute query
            stmt = con.createStatement();

            // executing SELECT query
            rs = stmt.executeQuery(query);

          //  result = new ArrayList<>(rs.getFetchSize());

            while (rs.next()) {
                date = rs.getTimestamp(1);
                flightId = rs.getString(2);

                result.add(new TimeTableItem(date, flightId));
                System.out.println(result.getLast());
            }

        } catch (SQLException sqlEx) {
            sqlEx.printStackTrace();
        } finally {
            //close connection ,stmt and resultset here
            try { con.close(); } catch(SQLException se) { /*can't do anything */ }
            try { stmt.close(); } catch(SQLException se) { /*can't do anything */ }
            try { rs.close(); } catch(SQLException se) { /*can't do anything */ }
        }

        return result;
    }

    public String getFullFlightInfo(TimeTableItem tti) {
        String query = "select time, flight from time_table";
        String result;
        //Date date;
        //String flightId;
        try {
            // opening database connection to MySQL server
            con = DriverManager.getConnection(url, user, password);

            // getting Statement object to execute query
            stmt = con.createStatement();

            // executing SELECT query
            rs = stmt.executeQuery(query);

            //  result = new ArrayList<>(rs.getFetchSize());

            while (rs.next()) {
                date = rs.getTimestamp(1);
                flightId = rs.getString(2);

                result.add(new TimeTableItem(date, flightId));
                System.out.println(result.getLast());
            }

        } catch (SQLException sqlEx) {
            sqlEx.printStackTrace();
        } finally {
            //close connection ,stmt and resultset here
            try { con.close(); } catch(SQLException se) { /*can't do anything */ }
            try { stmt.close(); } catch(SQLException se) { /*can't do anything */ }
            try { rs.close(); } catch(SQLException se) { /*can't do anything */ }
        }

        return result;
    }
}