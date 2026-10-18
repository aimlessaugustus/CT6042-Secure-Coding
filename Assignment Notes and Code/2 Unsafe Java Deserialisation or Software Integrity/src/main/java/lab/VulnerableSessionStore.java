package lab;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.nio.file.Path;

/**
 * Demonstrates the insecure deserialisation sink associated with CVE-2020-9484.
 * Session data is loaded from a file and deserialised using
 * ObjectInputStream.readObject() without class filtering, reproducing the
 * behaviour present in vulnerable Tomcat configurations.
 *
 * <p>
 * The implementation also retains the path traversal condition that allowed
 * attacker-controlled session IDs to reference files outside the intended
 * storage directory.
 *
 * <p>
 * For educational use only. Do not use in production systems.
 */
public class VulnerableSessionStore {

    private final Path directory;

    public VulnerableSessionStore(Path directory) {
        this.directory = directory;
    }

    /**
     * Loads and deserialises the session file for the supplied identifier. This
     * mirrors the vulnerable behaviour associated with CVE-2020-9484 by
     * resolving a session file and passing it directly to
     * {@code ObjectInputStream.readObject()} without any class filtering.
     *
     * @param sessionId attacker-influenced session identifier; path traversal
     * sequences such as {@code ../} can escape the configured storage directory
     * @return the deserialised object
     */
    public Object load(String sessionId) throws IOException, ClassNotFoundException {

        // Deliberately vulnerable path resolution used to demonstrate the
        // traversal condition required by CVE-2020-9484.
        File file = new File(directory.toFile(), sessionId + ".session");

        System.out.println("[store] loading session file: " + file.getPath());

        try (
                FileInputStream fis = new FileInputStream(file); // Insecure deserialisation sink. No allow-list or filtering is
                // applied before readObject() reconstructs the object graph.
                 ObjectInputStream ois = new ObjectInputStream(fis)) {
            return ois.readObject();
        }
    }
}
