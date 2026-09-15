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

## Week 6: Buffer Overflow

C is considered to be the root of all modern programming languages, translating from mnemonics to english language, and the creation of functions and having them run independently. C can be seen as the bane of the security community, primarily due to programmers not implementing logic under the assumption that it will be handled by C. Examples of risks within C include writing beyond the boundaries of an array, failing to catch integer overflows and truncations, and calling functions with the incorrect number of arguments.

When data is written outside the boundaries of memory, an overflow occurs.

### Key Vulnerable Functions in C/C++

- strcat()
- strcpy()
- sprint()
- vsprintf()
- bcopy()
- gets()
- scanf()

## Week 8: Secure Development Lifecycle

For the assignment, consider using the Microsoft Secure Development Lifecycle compared to other standards such as ISO/IEC and the OWASP SMM.

The link to this can be found [here][MicrosoftSDLPractices]. STRIDE threat modelling should also be considered and can be read up on [here][STRIDEGuidance]].

Potential assignment point: criticality and evaluation could be used when determining which framework is the most appropriate for the assignment project.

![]

[MicrosoftSDLPractices]:[https://www.microsoft.com/en-us/securityengineering/sdl/practices]

[STRIDEGuidance]:[https://www.gov.uk/government/publications/secure-connected-places-playbook-documents/conducting-a-stride-based-threat-analysis]

## Week 9: Secure Software Supply Chain

Understand what GHAS does. Try to map high-level frameworks to CI/CD piplines.
