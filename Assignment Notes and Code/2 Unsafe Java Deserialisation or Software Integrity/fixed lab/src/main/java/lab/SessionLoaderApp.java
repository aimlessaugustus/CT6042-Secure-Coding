package lab;

import java.io.File;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * Entry point for the remediated lab. Invoked with {@code save}, it writes a
 * normal session; invoked with {@code load-safe <sessionId>}, it reads a session
 * back through the hardened {@link MitigatedSessionStore}, whose JEP 290 filter
 * rejects any payload built from disallowed classes. The vulnerable store has
 * been removed from this build so the fixed project can be scanned in isolation.
 */
public class SessionLoaderApp {

    /** Directory that stands in for Tomcat's FileStore session directory. */
    private static final Path SESSION_DIR = Paths.get("sessions");

    public static void main(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("usage: (save | load-safe <sessionId>)");
            System.exit(2);
        }

        SESSION_DIR.toFile().mkdirs();

        switch (args[0]) {
            case "save":
                saveDemoSession();
                break;
            case "load-safe":
                if (args.length < 2) {
                    System.err.println("usage: load-safe <sessionId>");
                    System.exit(2);
                }
                loadSessionSafely(args[1]);
                break;
            default:
                System.err.println("unknown command: " + args[0]);
                System.exit(2);
        }
    }

    /** Writes a normal, benign session file so that legitimate behaviour can be observed. */
    private static void saveDemoSession() throws Exception {
        Session s = new Session("node01", "alice", "USER");
        File out = new File(SESSION_DIR.toFile(), s.getSessionId() + ".session");
        try (FileOutputStream fos = new FileOutputStream(out);
             ObjectOutputStream oos = new ObjectOutputStream(fos)) {
            oos.writeObject(s);
        }
        System.out.println("[app] saved benign session -> " + out.getPath());
    }

    /** Loads a session through the hardened store; a disallowed payload is rejected before construction. */
    private static void loadSessionSafely(String sessionId) throws Exception {
        MitigatedSessionStore store = new MitigatedSessionStore(SESSION_DIR);
        Object loaded = store.load(sessionId);
        System.out.println("[app] loaded object: " + loaded);
    }
}
