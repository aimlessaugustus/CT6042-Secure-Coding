# Tuesday

## Week 4: Deserialisaiton Exploit in Java

Ahsan will be doing the assignments and workshop and other assessments, so treat him as the point of contact from here on out.

Java will often consist of a source file (.java file), a compiled class file (.class) and an archive file (.jar) that packages multiple .class files and resources for distribution.

Deserialisation is taking some structured data (such as a database) and turning it back into an object.

### Why Java?

- Has the most links to deserialisation vulnerabilities
- Does not mean that it's the only language that has deserialisation - PHP and C# can have the same vulnerabilities.
- On the other side, it has a lot of scalability, which could make it useful for big companies anticipating large growth.

#### Risks

An attacker exploiting deserialisation can do the following:

- RCE
- Tampering
- DoS attacks
- Authentication Bypass.

Deseralising untrusted data, a lack of validation and the use of classes with dangerous methods can contribute to deserialisation vulnerabilities. During our discussion of deserialisaiton, using Java documentation on deprecated/dangerous methods could be a good source. Like wise, look at real code vulnerabilities such as Solar Winds to base my examples for my portfolio, as this has more context tied to it.

To avoid deserialisation, developers can do the following:

- Do not deserialise untrusted data
- Implement class filtering
- Remove unnecessary classes from class path
- Avoid readObject, readResolve or similar
- Validate serialised data
