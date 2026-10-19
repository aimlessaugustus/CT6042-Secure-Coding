package lab;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputFilter;
import java.io.ObjectInputStream;
import java.nio.file.Path;

/**
 * Hardened counterpart to {@link VulnerableSessionStore}. It loads a session file
 * through the same {@code ObjectInputStream.readObject()} path, but applies a
 * JEP 290 class filter so that only expected classes are instantiated,
 * reproducing the approach taken by the Tomcat fix for CVE-2020-9484.
 *
 * <p>
 * The gadget chain executes while the object graph is being reconstructed, so
 * validating the returned object would act too late. The filter is applied
 * before readObject() runs and rejects disallowed classes during reconstruction.
 *
 * <p>
 * A strict allow-list is used in preference to a block-list: known gadget chains
 * grow over time, so permitting only expected classes fails safe where denying
 * known-bad names does not.
 *
 * <p>
 * For educational use only.
 */
public class MitigatedSessionStore {

    private final Path directory;

    public MitigatedSessionStore(Path directory) {
        this.directory = directory;
    }

    /**
     * Builds the allow-list filter. Only {@link Session} and the core
     * {@code java.lang}/{@code java.util} types a legitimate session uses are
     * admitted; everything else is rejected by the trailing {@code !*}. Revise
     * the allow-list, not the reject, when a legitimate session's contents change.
     */
    private static ObjectInputFilter allowListFilter() {
        return ObjectInputFilter.Config.createFilter(
                "lab.Session;"
                + "java.lang.String;java.lang.Number;java.lang.Integer;java.lang.Long;"
                + "java.lang.Boolean;java.util.*;"
                + "!*");
    }

    /**
     * Loads and deserialises the session file for the supplied identifier, with
     * the allow-list filter applied to the stream before any object is read. A
     * payload built from disallowed classes is rejected with an
     * {@code InvalidClassException} and the gadget chain never runs.
     *
     * @param sessionId session identifier
     * @return the deserialised object, consisting only of allow-listed classes
     */
    public Object load(String sessionId) throws IOException, ClassNotFoundException {

        File file = new File(directory.toFile(), sessionId + ".session");

        System.out.println("[store] loading session file: " + file.getPath());

        try (
                FileInputStream fis = new FileInputStream(file);
                ObjectInputStream ois = new ObjectInputStream(fis)) {
            // Security control. The allow-list filter is set before readObject()
            // so a disallowed class is rejected during reconstruction.
            ois.setObjectInputFilter(allowListFilter());
            return ois.readObject();
        }
    }
}
