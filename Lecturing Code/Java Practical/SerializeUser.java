// SerializeUser.java

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

public class SerializeUser {

    public static void main(String[] args) {
        User user = new User("alice", "admin");
        try (FileOutputStream fileOut = new FileOutputStream("user.ser"); ObjectOutputStream out = new ObjectOutputStream(fileOut)) {
            out.writeObject(user);
            System.out.println("User object serialized successfully.");
        } catch (IOException i) {
            i.printStackTrace();
        }
    }
}
