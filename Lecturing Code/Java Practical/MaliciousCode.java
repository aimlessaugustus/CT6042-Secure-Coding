
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.Serializable;

public class MaliciousCode implements Serializable {

    private static final long serialVersionUID = 1L;

    private void readObject(ObjectInputStream ois) throws ClassNotFoundException, IOException {
        ois.defaultReadObject();
        // Code to be executed during deserialization
        System.out.println("Malicious code executed!");
        // Potentially harmful actions could be placed here
    }
}
