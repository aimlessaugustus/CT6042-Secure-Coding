package lab;

import java.io.Serializable;

/**
 * A benign session-attribute object, representing the kind of value a servlet
 * container legitimately serialises to disk when it swaps an idle HTTP session
 * out of memory. It is harmless in itself and exists only to show what normal
 * use of the store looks like, so that the exploit can be read against a
 * baseline rather than in isolation.
 *
 * <p>It is worth stressing that the vulnerability has nothing to do with this
 * class. The attacker never needs a malicious {@code Session}; the payload is an
 * entirely separate object graph, assembled from classes already present on the
 * classpath, as the project README describes.
 */
public class Session implements Serializable {

    /** Fixed serialVersionUID so that saved sessions remain loadable across builds. */
    private static final long serialVersionUID = 1L;

    private final String sessionId;
    private final String username;
    private final String role;

    public Session(String sessionId, String username, String role) {
        this.sessionId = sessionId;
        this.username = username;
        this.role = role;
    }

    public String getSessionId() { return sessionId; }
    public String getUsername()  { return username; }
    public String getRole()      { return role; }

    @Override
    public String toString() {
        return "Session{id=" + sessionId + ", user=" + username + ", role=" + role + "}";
    }
}
