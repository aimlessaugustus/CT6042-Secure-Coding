package lab;

import java.io.Serializable;

/**
 * A benign session attribute used to represent the type of object that a
 * servlet container may legitimately serialise and store within an HTTP
 * session.
 *
 * This class is not involved in the vulnerability itself. It exists solely as a
 * normal baseline example, highlighting that insecure deserialisation arises
 * from processing attacker-controlled object graphs rather than from the
 * application's session objects.
 */
public class Session implements Serializable {

    /**
     * Fixed serialVersionUID so that saved sessions remain loadable across
     * builds.
     */
    private static final long serialVersionUID = 1L;

    private final String sessionId;
    private final String username;
    private final String role;

    public Session(String sessionId, String username, String role) {
        this.sessionId = sessionId;
        this.username = username;
        this.role = role;
    }

    public String getSessionId() {
        return sessionId;
    }

    public String getUsername() {
        return username;
    }

    public String getRole() {
        return role;
    }

    @Override
    public String toString() {
        return "Session{id=" + sessionId + ", user=" + username + ", role=" + role + "}";
    }
}
