#ifndef AUTH_H
#define AUTH_H

#include <stddef.h>

#define AUTH_MAX_USER 64
#define AUTH_MAX_PASS 128

int  auth_init(const char *db_path);
int  auth_add(const char *username, const char *password);
int  auth_check(const char *username, const char *password); /* 1 ok, 0 ko, -1 errore IO */
int  auth_delete(const char *username);
int  auth_change_password(const char *username, const char *new_password);
int  auth_rename_user(const char *old_username, const char *new_username);

#endif /* AUTH_H */
