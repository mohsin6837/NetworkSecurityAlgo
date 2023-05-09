#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#define SIZE 3 // Size of the matrix key

// Function to calculate the determinant of a 2x2 matrix

int calcDeterminant(int key[SIZE][SIZE]) {
    return key[0][0] * (key[1][1] * key[2][2] - key[2][1] * key[1][2]) -
           key[0][1] * (key[1][0] * key[2][2] - key[2][0] * key[1][2]) +
           key[0][2] * (key[1][0] * key[2][1] - key[2][0] * key[1][1]);
}

// Function to calculate the modular inverse of a number
int calcModInverse(int num, int m) {
    num = num % m;
    for (int x = 1; x < m; x++)
        if ((num * x) % m == 1)
            return x;
    return -1;
}
// Function to encrypt a message using the Hill cipher
void encrypt(char *message, int key[SIZE][SIZE]) {
    int messageLen = strlen(message);
    int cipher[messageLen];

    for (int i = 0; i < messageLen; i++)
        message[i] = toupper(message[i]);

    for (int i = 0; i < messageLen; i += SIZE) {
        for (int j = 0; j < SIZE; j++)
            cipher[i + j] = 0;

        for (int k = 0; k < SIZE; k++)
            for (int j = 0; j < SIZE; j++)
                cipher[i + k] += key[k][j] * (message[i + j] - 'A');

        for (int k = 0; k < SIZE; k++)
            cipher[i + k] = cipher[i + k] % 26;
    }

    printf("Encrypted Message: ");
    for (int i = 0; i < messageLen; i++)
        printf("%c", cipher[i] + 'A');
    printf("\n");
}

// Function to decrypt a message using the Hill cipher
void decrypt(int *cipher, int key[SIZE][SIZE], int messageLen) {
    int inverse = calcModInverse(calcDeterminant(key), 26);
    int plain[messageLen];

    inverse = (inverse + 26) % 26; // Ensure the inverse is positive

    for (int i = 0; i < messageLen; i += SIZE) {
        for (int j = 0; j < SIZE; j++)
            plain[i + j] = 0;

        for (int k = 0; k < SIZE; k++)
            for (int j = 0; j < SIZE; j++)
                plain[i + k] += key[k][j] * cipher[i + j];

        for (int k = 0; k < SIZE; k++)
            plain[i + k] = (plain[i + k] * inverse) % 26;
    }

    printf("Decrypted Message: ");
    for (int i = 0; i < messageLen; i++)
        printf("%c", plain[i] + 'A');
    printf("\n");
}

int main(){
    char message[100];
    int key[SIZE][SIZE] = {{6, 24, 1}, {13, 16, 10}, {20, 17, 15}};
    printf("Enter the message to encrypt: ");
    fgets(message, sizeof(message), stdin);

    // Remove newline character from the message
    if (strlen(message) > 0 && message[strlen(message) - 1] == '\n')
        message[strlen(message) - 1] = '\0';

    // Pad the message if its length is not a multiple of SIZE
    int messageLen = strlen(message);
    int padding = messageLen % SIZE;
    if (padding != 0) {
        for (int i = 0; i < SIZE - padding; i++)
            message[messageLen++] = 'X';
        message[messageLen] = '\0';
    }

    encrypt(message, key);

    // Convert the encrypted message to uppercase
    for (int i = 0; i < messageLen; i++)
        message[i] = toupper(message[i]);

    int cipher[messageLen];
    for (int i = 0; i < messageLen; i++)
        cipher[i] = message[i] - 'A';

    decrypt(cipher, key, messageLen);

    return 0;
}