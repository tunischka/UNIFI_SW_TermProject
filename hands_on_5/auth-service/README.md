# Auth Service


Microservizio minimale: **un solo controller** e **store in memoria** (niente file/DB).

Tutti i file sono con terminatori CRLF (`\r\n`), adatti a Windows.


## Endpoints

- `GET /health`

- `GET /users`

- `GET /users/{username}`

- `POST /users` – body `{ "username": "...", "password": "..." }`

- `PUT /users/{username}` – body `{ "newUsername": "...", "password": "..." }` (entrambi opzionali)

- `DELETE /users/{username}`



## Avvio rapido

```bash

docker compose up --build

# servizio su http://localhost:8003

```



## Esempi

```bash

curl -X POST http://localhost:8003/users -H "Content-Type: application/json" -d "{"username":"bob","password":"secret"}"

curl http://localhost:8003/users/bob

curl -X PUT http://localhost:8003/users/bob -H "Content-Type: application/json" -d "{"newUsername":"bobby","password":"newSecret"}"

curl -X DELETE http://localhost:8003/users/bobby

```


Dati **volatili**: si perdono al riavvio (niente persistenza).


