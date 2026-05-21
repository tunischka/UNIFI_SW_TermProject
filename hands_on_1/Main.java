package hands_on1;
public class Main {
    public static void main(String[] args) {
        AuthLibrary auth = new AuthLibrary();
        auth.authInit("users.db");

        auth.authAdd("tuna", "secret123");

        // change password and make sure old one stops working
        auth.authChangePassword("tuna", "newPass456");

        // spam wrong password until lockout kicks in
        for (int i = 1; i <= 4; i++) {
            int result = auth.authCheck("tuna", "wrongpass");
            System.out.println("attempt " + i + ": " + (result == -2 ? "locked" : "failed"));
        }

        auth.authDelete("tuna");
    }
}