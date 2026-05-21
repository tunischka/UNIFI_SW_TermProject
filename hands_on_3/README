# Hands-on #3 — UML Diagrams

UML diagrams modeling the Authentication Library, designed in accordance with the requirements defined in Hands-on #2 (SRS).

## Diagrams

### `class_diagram.png`
Static structure of the system. The `AuthLibrary` class exposes the five public operations described in the SRS (`authInit`, `authAdd`, `authCheck`, `authDelete`, `authChangePassword`) and uses two private helpers (`findUser`, `removeOrUpdateUser`). The `Main` class depends on `AuthLibrary` (uses relationship).

### `state_diagram.png`
Lifecycle of a single user account. An account moves from `NotRegistered` to `Active` upon `authAdd`, and to `Locked` after three consecutive failed authentication attempts (REQ-SW-10). Successful authentication resets the failed-attempts counter (REQ-SW-11). `authDelete` terminates the account from either state.

### `sequence_diagram.png`
Interaction during two consecutive `authCheck` calls — a failed attempt followed by a successful one. Shows the messages exchanged between the User, the `AuthLibrary` instance, and the credential file. The `opt` fragments model the conditional update of the failed-attempts counter.

### `activity_diagram.png`
Workflow of the `authAdd` operation: input validation, character validation, duplicate-username check, and persistence to the credential file. Each validation failure produces a `-1` return; the happy path produces `0`.

## Tools used

- **draw.io** — class diagram, state diagram
- **sequencediagram.org** — sequence diagram
- **PlantUML** — activity diagram
