#include <stdio.h>
 
// Function to compute `a^m mod n`
int compute(int a, int m, int n)
{
    int r;
    int y = 1;
 
    while (m > 0)
    {
        r = m % 2;
 
        // fast exponention
        if (r == 1) {
            y = (y*a) % n;
        }
        a = a*a % n;
        m = m / 2;
    }
 
    return y;
}
 
int main()
{
    int p = 23;        // modulus
    int g = 5;        // base
 
    int a, b;    
    int A, B;    
 
    // choose a secret integer for Alice's private key 
    printf("Enter the secret integer for Alice's private key : ");
    scanf("%d", &a);
 
    // Calculate Alice's public key 
    A = compute(g, a, p);
 
    // choose a secret integer for Bob's private key (only known to Bob)
    printf("Enter the secret integer for Bob's private key : ");
    scanf("%d", &b);
 
    // Calculate Bob's public key (Bob will send `B` to Alice)
    B = compute(g, b, p);
 
    // Alice and Bob Exchange their public key `A` and `B` with each other
 
    // Find secret key
    int keyA = compute(B, a, p);
    int keyB = compute(A, b, p);
 
    printf("Alice's secret key is %d\nBob's secret key is %d", keyA, keyB);
 
    return 0;
}