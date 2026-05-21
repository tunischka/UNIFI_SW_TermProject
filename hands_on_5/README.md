# Hands-on #5 — Authentication microservice

Starting from the running project's `microservices/auth-service`, two changes were applied:

1. Added a `POST /login` endpoint in `AuthController.java` that receives a JSON body with `username` and `password`, looks the user up in the in-memory map, and replies with `OK` if the stored password matches, `NON OK` otherwise (implemented following the example in slide 37).
2. Updated `Dockerfile` to use `eclipse-temurin:17-jre` instead of `eclipse-temurin:17-jre-alpine`, because the Alpine variant is not published for the `aarch64` architecture used on Apple Silicon Macs.
