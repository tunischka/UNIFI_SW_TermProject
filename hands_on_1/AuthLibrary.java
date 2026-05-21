package hands_on1;
import java.io.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class AuthLibrary {

    private String dbPath = "users.db";

    // tracks failed logins per user — lockout kicks in at 3
    private Map<String, Integer> failedAttempts = new HashMap<>();

    public int authInit(String path) {
        if (path != null && !path.isEmpty()) {
            this.dbPath = path;
        }

        File dbFile = new File(dbPath);
        if (dbFile.exists()) return 0;

        try {
            dbFile.createNewFile();
            return 0;
        } catch (IOException e) {
            return -1;
        }
    }

    public int authAdd(String username, String password) {
        if (username == null || username.isEmpty() ||
            password == null || password.isEmpty()) {
            return -1;
        }

        // colon and newline would break our file format
        if (username.contains(":") || username.contains("\n") || password.contains("\n")) {
            return -1;
        }

        if (findUser(username) != null) return -1; // already exists

        try (PrintWriter writer = new PrintWriter(new FileWriter(dbPath, true))) {
            writer.println(username + ":" + password);
            return 0;
        } catch (IOException e) {
            return -1;
        }
    }

    // returns: 1 = ok, 0 = wrong password, -1 = bad input, -2 = locked
    public int authCheck(String username, String password) {
        if (username == null || password == null) return -1;

        int attempts = failedAttempts.getOrDefault(username, 0);
        if (attempts >= 3) {
            System.err.println("account locked: " + username);
            return -2;
        }

        String stored = findUser(username);
        if (stored != null && stored.equals(password)) {
            failedAttempts.put(username, 0); // reset on success
            return 1;
        }

        failedAttempts.put(username, attempts + 1);
        return 0;
    }

    public int authDelete(String username) {
        if (username == null || username.isEmpty()) return -1;
        return removeOrUpdateUser(username, null);
    }

    public int authChangePassword(String username, String newPassword) {
        if (username == null || newPassword == null || newPassword.isEmpty()) return -1;
        return removeOrUpdateUser(username, newPassword);
    }

    private String findUser(String username) {
        try (BufferedReader reader = new BufferedReader(new FileReader(dbPath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                int sep = line.indexOf(':');
                if (sep == -1) continue; // malformed line, skip

                if (line.substring(0, sep).equals(username)) {
                    return line.substring(sep + 1);
                }
            }
        } catch (IOException e) {
            // file might not exist yet
        }
        return null;
    }

    // null newPassword = delete, otherwise updates it
    private int removeOrUpdateUser(String target, String newPassword) {
        List<String> lines = new ArrayList<>();
        boolean found = false;

        try (BufferedReader reader = new BufferedReader(new FileReader(dbPath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                int sep = line.indexOf(':');
                if (sep == -1) {
                    lines.add(line);
                    continue;
                }

                if (line.substring(0, sep).equals(target)) {
                    found = true;
                    if (newPassword != null) {
                        lines.add(target + ":" + newPassword);
                    }
                    // skip line entirely if deleting
                } else {
                    lines.add(line);
                }
            }
        } catch (IOException e) {
            return -1;
        }

        if (!found) return -1;

        try (PrintWriter writer = new PrintWriter(new FileWriter(dbPath))) {
            for (String line : lines) {
                writer.println(line);
            }
            return 0;
        } catch (IOException e) {
            return -1;
        }
    }
}