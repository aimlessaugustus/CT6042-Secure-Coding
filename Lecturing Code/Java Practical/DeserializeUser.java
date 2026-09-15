// DeserializeUser.java

import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;

public class DeserializeUser {

    public static void main(String[] args) {
        User user = null;
        try (FileInputStream fileIn = new FileInputStream("user.ser"); ObjectInputStream in = new ObjectInputStream(fileIn)) {
            user = (User) in.readObject();
            System.out.println("User object deserialized successfully.");
            System.out.println("User details: " + user);
        } catch (IOException | ClassNotFoundException i) {
            i.printStackTrace();
        }
    }
}
