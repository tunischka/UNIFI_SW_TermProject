package com.example.auth;



import org.springframework.http.HttpStatus;

import org.springframework.http.ResponseEntity;

import org.springframework.web.bind.annotation.*;

import org.springframework.web.server.ResponseStatusException;



import java.util.*;

import java.util.concurrent.ConcurrentHashMap;



@RestController

public class AuthController {



    private final Map<String, String> users = new ConcurrentHashMap<>();



    @GetMapping("/health")

    public String health() {

        return "OK";

    }



    public static class CreateUserRequest {

        public String username;

        public String password;

    }



    public static class UpdateUserRequest {

        public String newUsername;

        public String password;

    }



    public static class User {

        public String username;

        public String password;



        public User() {}

        public User(String username, String password) {

            this.username = username;

            this.password = password;

        }

    }



    private List<User> toList() {

        List<User> list = new ArrayList<>();

        for (Map.Entry<String,String> e : users.entrySet()) {

            list.add(new User(e.getKey(), e.getValue()));

        }

        list.sort(Comparator.comparing(u -> u.username));

        return list;

    }



    private User toUser(String username) {

        String pwd = users.get(username);

        if (pwd == null) throw new ResponseStatusException(HttpStatus.NOT_FOUND, "User not found");

        return new User(username, pwd);

    }



    @GetMapping("/users")

    public List<User> list() {

        return toList();

    }



    @GetMapping("/users/{username}")

    public ResponseEntity<User> get(@PathVariable String username) {

        return ResponseEntity.ok(toUser(username));

    }



    @PostMapping("/users")

    public ResponseEntity<User> create(@RequestBody CreateUserRequest req) {

        if (req == null || req.username == null || req.username.isBlank() || req.password == null || req.password.isBlank()) {

            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "username and password are required");

        }

        if (users.containsKey(req.username)) {

            throw new ResponseStatusException(HttpStatus.CONFLICT, "username already exists");

        }

        users.put(req.username, req.password);

        return ResponseEntity.status(HttpStatus.CREATED).body(new User(req.username, req.password));

    }



    @PutMapping("/users/{username}")

    public User update(@PathVariable String username, @RequestBody UpdateUserRequest req) {

        if (!users.containsKey(username)) {

            throw new ResponseStatusException(HttpStatus.NOT_FOUND, "user not found");

        }

        if (req != null && req.newUsername != null && !req.newUsername.isBlank() && !req.newUsername.equals(username)) {

            if (users.containsKey(req.newUsername)) {

                throw new ResponseStatusException(HttpStatus.CONFLICT, "new username already exists");

            }

            String oldPwd = users.remove(username);

            users.put(req.newUsername, oldPwd);

            username = req.newUsername;

        }

        if (req != null && req.password != null && !req.password.isBlank()) {

            users.put(username, req.password);

        }

        return toUser(username);

    }

    @PostMapping("/login")
    public ResponseEntity<String> login(@RequestBody CreateUserRequest req) {
        String storedPwd = users.get(req.username);
        if (storedPwd.equals(req.password)) {
            return ResponseEntity.ok("OK");
        }
        return ResponseEntity.ok("NON OK");
    }

    @DeleteMapping("/users/{username}")

    public ResponseEntity<Void> delete(@PathVariable String username) {

        if (users.remove(username) == null) {

            throw new ResponseStatusException(HttpStatus.NOT_FOUND, "user not found");

        }

        return ResponseEntity.noContent().build();

    }

}