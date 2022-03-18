
import java.io.BufferedReader;
import java.io.InputStreamReader;

import Tokens.TokenStream;
import Propositions.Proposition;


public class Main {
    public static void main(String[] args) {
        //https://www.geeksforgeeks.org/ways-to-read-input-from-console-in-java/
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
 
        try {
            for (String input = reader.readLine(); !input.equals("quit"); input = reader.readLine()) {
                try {
                    System.out.println("Tokens: " + (new TokenStream(input)));
                    System.out.println("Parsed: " + (new Proposition(input)));
                }
                catch (Exception e) {
                    System.out.println(e.getMessage());
                }
            }   
        }
        catch(Exception e) {
            System.out.println(e);
        }
    }
}
