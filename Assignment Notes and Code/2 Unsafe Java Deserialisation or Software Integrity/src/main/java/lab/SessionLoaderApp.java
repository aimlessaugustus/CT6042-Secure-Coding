package lab;

import java.io.File;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * Simulates a servlet container reloading a persisted session from disk, and
 * serves as the application under attack in this lab. Invoked with
 * {@code save}, it writes a normal session so that legitimate behaviour can be
 * observed. When invoked with {@code load <sessionId>}, it reads a session back
 * through the vulnerable store. With a benign file the second command is
 * harmless, but pointed at a crafted {@code .session} file it results in remote
 * code execution. The README sets out how that malicious file is produced with
 * ysoserial.
 */
public class SessionLoaderApp {

    /**
     * Directory that stands in for Tomcat's FileStore session directory.
     */
    private static final Path SESSION_DIR = Paths.get("sessions");

    public static void main(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("usage: (save | load <sessionId>)");
            System.exit(2);
        }

        SESSION_DIR.toFile().mkdirs();

        switch (args[0]) {
            case "save":
                saveDemoSession();
                break;
            case "load":
                if (args.length < 2) {
                    System.err.println("usage: load <sessionId>");
                    System.exit(2);
                }
                loadSession(args[1]);
                break;
            default:
                System.err.println("unknown command: " + args[0]);
                System.exit(2);
        }
    }

    /**
     * Writes a normal, benign session file so that legitimate use can be
     * contrasted against the exploit.
     */
    private static void saveDemoSession() throws Exception {
        Session s = new Session("node01", "alice", "USER");
        File out = new File(SESSION_DIR.toFile(), s.getSessionId() + ".session");
        try (FileOutputStream fos = new FileOutputStream(out); ObjectOutputStream oos = new ObjectOutputStream(fos)) {
            oos.writeObject(s);
        }
        System.out.println("[app] saved benign session -> " + out.getPath());
    }

    /**
     * Loads a session through the vulnerable store and prints whatever object
     * graph comes back.
     */
    private static void loadSession(String sessionId) throws Exception {
        VulnerableSessionStore store = new VulnerableSessionStore(SESSION_DIR);
        Object loaded = store.load(sessionId);
        System.out.println("[app] loaded object: " + loaded);
    }
}
