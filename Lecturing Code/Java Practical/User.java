// User.java

import java.io.Serializable;

public class User implements Serializable {

    private static final long serialVersionUID = 1L; // For version control
    private String username;
    private String role;
    // Constructor

    public User(String username, String role) {
        this.username = username;
        this.role = role;
    }

    // Getters
    public String getUsername() {
        return username;
    }

    public String getRole() {
        return role;
    }
    // toString method for easy printing

    @Override
    public String toString() {
        return "User{username='" + username + "', role='" + role + "'}";
    }
}
