## UNIFI_SW_TermProject
Software Engineering term project — Authentication Library

Author : Tunahan Gokgoz (matricola 7204825)
Course : Ingegneria del Software, A.A. 2025-2026

Repository organization
-----------------------

hands_on_1/  Agile re-engineering of the C running project into Java.
             - AuthLibrary.java, Main.java : Java source code
             - HandsOn1-TunahanGokgoz.pdf  : Scrum process report

hands_on_2/  Requirements Engineering: filled IEEE SRS template
             with functional + non-functional requirements and a
             use case diagram.
             - HandsOn2.pdf

hands_on_3/  UML diagrams modelling the authentication library:
             class, state, sequence, activity (PNG).

hands_on_4/  Quality analysis of the original C running project.
             - before/ : MISRA inspection, metrics, coverage on the
                         original code
             - after/  : same measurements after refactoring
             - after/modified_source/ : the cleaned C sources

hands_on_5/  Spring Boot microservice exposing the authentication
             library as a REST API, packaged with Docker.

How to run
----------

hands_on_1 : compile with javac, run Main.

hands_on_4 : in microservices/quality (original repo),
             gcc-15 *.c -fprofile-arcs -ftest-coverage -o app
             lcov for the coverage report.
             
hands_on_5 : cd hands_on_5/auth-service
             mvn package
             docker compose up -d
             curl http://localhost:8003/health
