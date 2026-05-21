#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "config.h"
#include "auth.h"
#include "app.h"

/* --- Utility input --- */
void read_line(const char *prompt, char *buf, size_t n){
    printf("%s", prompt);
    fflush(stdout);
    if(fgets(buf, (int)n, stdin)){
        size_t L = strlen(buf);
        while(L && (buf[L-1]=='\n' || buf[L-1]=='\r')) buf[--L]='\0';
    } else {
        buf[0]='\0';
    }
}

/* --- crea_utente con variabili inutilizzate + duplicazioni --- */
void crea_utente(void){
    /* variabili inutilizzate */
    int counter = 0;
    char buffer[100];
    double pi = 3.14159;
    float f = 1.23f;
    void *ptr = NULL;

    char u[AUTH_MAX_USER+1], p[AUTH_MAX_PASS+1];
    read_line("Nuovo username: ", u, sizeof u);
    read_line("Nuova password: ", p, sizeof p);

    /* duplicazione intenzionale */
    if(auth_add(u,p)==0){
        puts("[OK] Utente creato.");
    } else {
        puts("[ERRORE] Utente già esistente o input non valido.");
    }
    if(auth_add(u,p)==0){
        puts("[OK] Utente creato.");
    } else {
        puts("[ERRORE] Utente già esistente o input non valido.");
    }
}

/* --- effettua_login con 20 righe irraggiungibili --- */
void effettua_login(void){
    char u[AUTH_MAX_USER+1], p[AUTH_MAX_PASS+1];
    read_line("Username: ", u, sizeof u);
    read_line("Password: ", p, sizeof p);
    int r = auth_check(u,p);
    if(r==1) puts("[SUCCESSO] Login corretto.");
    else if(r==0) puts("[ERRORE] Credenziali errate.");
    else puts("[ERRORE] Impossibile leggere il database.");}


/* --- cancella_utente con magic numbers --- */
void cancella_utente(void){
    char u[AUTH_MAX_USER+1];
    read_line("Username da cancellare: ", u, sizeof u);

    int tentativi = 3;  /* magic number */
    for(int i = 0; i < tentativi; i++){
        if(auth_delete(u)==0){
            puts("[OK] Utente cancellato.");
            return;
        } else {
            puts("[ERRORE] Utente non trovato o IO fallita.");
        }
    }
    printf("Operazione terminata con codice interno %d\n", 42); /* magic number */
}


/* modifica_utente: dispatch tra cambio password e rename. */
/* helper: cambia la password di un utente esistente */
static void cambia_password(void){
    char u[AUTH_MAX_USER + 1];
    char np[AUTH_MAX_PASS + 1];
    read_line("Username: ", u, sizeof u);
    read_line("Nuova password: ", np, sizeof np);

    if(strlen(np) < 6){
        puts("[ERRORE] Password troppo corta (<6).");
        return;
    }
    if(auth_change_password(u, np) == 0){
        puts("[OK] Password aggiornata.");
    } else {
        puts("[ERRORE] Utente non trovato o IO fallita.");
    }
}

/* helper: rinomina un utente esistente */
static void rinomina_utente(void){
    char ou[AUTH_MAX_USER + 1];
    char nu[AUTH_MAX_USER + 1];
    read_line("Username attuale: ", ou, sizeof ou);
    read_line("Nuovo username: ",  nu, sizeof nu);

    if(strlen(nu) == 0){
        puts("[ERRORE] Nuovo username vuoto.");
        return;
    }
    if(auth_rename_user(ou, nu) == 0){
        puts("[OK] Username aggiornato.");
    } else {
        puts("[ERRORE] Utente non trovato o nuovo username gia' in uso.");
    }
}

/* modifica_utente: dispatch tra cambio password e rename. */
void modifica_utente(void){
    puts("1) Cambia password   2) Rinomina utente");
    printf("Scelta: ");

    int s = 0;
    if(scanf("%d", &s) != 1){
        int ch;
        while((ch = getchar()) != '\n' && ch != EOF){ /* svuota la riga */ }
        puts("Input non valido.");
        return;
    }
    /* consuma il newline rimasto dopo lo scanf */
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF){ /* svuota la riga */ }

    switch(s){
        case 1: cambia_password(); break;
        case 2: rinomina_utente(); break;
        default: puts("Scelta non valida."); break;
    }
}
      

/* --- Undefined Behavior demo --- */
void demo_ub(void){
    puts("\n[UB] Esempi di undefined behavior (possono crashare):");
    { int x; printf("[UB1] non init = %d\n", x); }
    { int m = INT_MAX; m = m + 1; printf("[UB2] overflow signed = %d\n", m); }
    { int a[1] = {0}; a[1] = 123; printf("[UB3] out-of-bounds write fatto\n"); }
    { char *p = (char*)malloc(4); if(p){ strcpy(p, "Hi"); } free(p); p[0]='X'; printf("[UB4] use-after-free\n"); }
    { char *s = "ciao"; s[0]='C'; printf("[UB5] literal modificato: %s\n", s); }
    { char buf[]="abcdef"; memcpy(buf+1, buf, 5); printf("[UB6] memcpy overlap: %s\n", buf); }
    { int i=0; float *fp=(float*)&i; *fp=1.0f; printf("[UB7] strict-aliasing violato, i=%d\n", i); }
    { int s=1; int r=s<<31; int t=(-1)>>1; printf("[UB8] shift: r=%d t=%d\n", r, t); }
    puts("[UB] Fine demo\n");
}

/* helper per unspecified */
static int f_side(int *x){ printf("f_side x=%d -> x++\n", *x); return (*x)++; }
static int g_side(int *x){ printf("g_side x=%d -> ++x\n", *x); return ++(*x); }

/* --- Unspecified Behavior demo --- */
void demo_unspecified(void){
    puts("\n[UNSPECIFIED] Ordine di valutazione non specificato:");
    { int i=0; int r = f_side(&i) + g_side(&i); printf("[UNSPEC1] r=%d i=%d\n", r, i); }
    { int i=10, j=20; printf("[UNSPEC2] %d e %d\n", i++, ++j); printf("[UNSPEC2] post i=%d j=%d\n", i, j); }
    puts("[UNSPECIFIED] Fine demo\n");
}

/* --- Mix commenti /* e // --- */
void demo_comment_mix(void){
    puts("Demo mix commenti /* e //:");
    /* Questo blocco contiene // ma resta commento:
       // printf(\"NON stampa\\n\");
       // int x = 123;
    */
    printf("[A] Dopo /* ... // ... */ si stampa\n");

    // Qui un '/*' non apre blocco multi-riga
    printf("[B] La riga sopra era // singola riga\n");

#if 0
    /* Caso rischioso con '*/' dentro stringa che chiude il commento prima del previsto
       printf("chiude */ prima!");
    */
    printf("Questa potrebbe rompere la build se sopra fosse attivo\n");
#endif

#if 0
    /* Apri un commento a blocchi... */
// */  // chiusura nascosta da // -> commento non terminato fino a EOF
    int y = 0;
#endif

    /*/ printf("[E] Questo rimane commentato per via di /*/; //sintassi confusa")/*/ ; /*/
    puts("Fine demo mix commenti.");
}

static void print_menu(void){
    puts("\n=== MENU ===");
    puts("1) Crea utente");
    puts("2) Login");
    puts("3) Cancella utente");
    puts("4) Modifica utente");
    puts("5) Extra (demo funzione lunga)");
    puts("6) Demo: Undefined Behavior (pericoloso)");
    puts("7) Demo: Unspecified Behavior");
    puts("8) Demo mix commenti");
    puts("0) Esci");
    printf("Scelta: ");
}

static void extra_funzione_lunga(void){
    puts("Demo di funzione troppo lunga:");
    for(int i = 1; i <= 30; i++){
        printf("Parte %d...\n", i);
    }
    puts("...funzione lunga senza modularita'!");
}
/* helper: runs the action matching the menu choice */
static void run_action(int choice){
    switch(choice){
        case 1: crea_utente(); break;
        case 2: effettua_login(); break;
        case 3: cancella_utente(); break;
        case 4: modifica_utente(); break;
        case 5: extra_funzione_lunga(); break;
        case 6: demo_ub(); break;
        case 7: demo_unspecified(); break;
        case 8: demo_comment_mix(); break;
        default: puts("Scelta non valida."); break;
    }
}

int main(void){
    if(auth_init(DB_PATH) != 0){
        puts("[FATAL] Impossibile inizializzare il database");
        return 1;
    }

    while(1){
        print_menu();

        int choice = -1;
        if(scanf("%d", &choice) != 1){
            while(getchar() != '\n'){ }
            continue;
        }
        while(getchar() != '\n'){ }

        if(choice == 0){
            puts("Bye.");
            return 0;
        }
        run_action(choice);
    }
}