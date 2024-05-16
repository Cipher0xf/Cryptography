#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;
#define N 4
#define MAX_LENGTH 1000

int LFSR[N] = {0, 0, 0, 1};  // 4-bit LFSR
bool poly[N] = {1, 0, 0, 1}; // characteristic polynomial: x^4+x+1
// bool poly2[N]={0,0,1,1};//characteristic polynomial: x^4+x^3+1

void shift()
{ // the linear feeback shift operation
    int feedback = 0;
    for (int i = 0; i < N; i++)
    {
        if (poly[i] == 1)
        {
            feedback ^= LFSR[i];
        }
    }
    for (int i = N - 1; i >= 1; i--)
    {
        LFSR[i] = LFSR[i - 1];
    }
    LFSR[0] = feedback;
}

void output()
{ // output the current state of LFSR
    for (int i = 0; i < N; i++)
    {
        cout << LFSR[i];
    }
    cout << endl;
}

void printLFSR()
{ // output all possible states of LFSR
    output();
    int T = (1 << N) - 1; // the period of LFSR is 2^4-1=15
    for (int i = 1; i < T; i++)
    {
        shift();
        output();
    }
}

char key[MAX_LENGTH] = ""; // stream key
char message[MAX_LENGTH] = "";
char ciphertext[MAX_LENGTH] = "";
char plaintext[MAX_LENGTH] = "";

void keyGen(int length)
{
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            key[i] += key[i] + (LFSR[N - 1] << (7 - j));
            shift();
        }
    }
    key[length] = '\0';
}

void encrypt(char *message, char *key)
{
    int length = strlen(key);
    for (int i = 0; i < length; i++)
    {
        ciphertext[i] = message[i] ^ key[i];
    }
    ciphertext[length] = '\0';
}

void decrypt(char *ciphertext, char *key)
{
    int length = strlen(key);
    for (int i = 0; i < length; i++)
    {
        plaintext[i] = ciphertext[i] ^ key[i];
    }
    plaintext[length] = '\0';
}

void init()
{
    memset(key, '\0', sizeof(key));
    memset(message, '\0', sizeof(message));
    memset(ciphertext, '\0', sizeof(ciphertext));
    memset(plaintext, '\0', sizeof(plaintext));
}

int main()
{
    // cout<<"Please input the characteristic polynomial: \n";
    // cout<<"Please input the initial state of the LFSR: \n";
    // printLFSR();

    int count = 10;
    while (count--)
    {
        init();
        char c;
        cout << "\nEncrypt a file or a short message?(F/M)\n";
        cin >> c;
        if (c == 'F' || c == 'f')
        {
            char filename[] = {};
            cout << "\nPlease input the file name: \n";
            scanf("%s", filename);
            FILE *fp1 = fopen(filename, "r");
            FILE *fp2 = fopen("key.txt", "w");
            FILE *fp3 = fopen("ciphertext.txt", "w");
            if (fp1 == NULL)
            {
                printf("Message file not found!\n");
                continue;
            }
            fscanf(fp1, "%s", message);
            fclose(fp1);
            printf("\nmessage: %s\n", message);

            keyGen(strlen(message));
            fprintf(fp2, "%s", key);
            printf("key(%d-bit): %s\n", strlen(key) * 8, key);
            fclose(fp2);

            memset(key, '\0', sizeof(key));
            fp2 = fopen("key.txt", "r");
            fscanf(fp2, "%s", key);
            encrypt(message, key);
            fprintf(fp3, "%s", ciphertext);
            printf("ciphertext: %s\n", ciphertext);
            fclose(fp3);

            decrypt(ciphertext, key);
            printf("plaintext: %s\n", plaintext);
            fflush(stdout);
        }
        else if (c == 'M' || c == 'm')
        {
            cout << "\nPlease input the message: \n";
            scanf("%s", message);
            printf("\nmessage: %s\n", message);
            fflush(stdout);

            keyGen(strlen(message));
            printf("key:(%d) %s\n", strlen(key), key);
            fflush(stdout);

            encrypt(message, key);
            printf("ciphertext: %s\n", ciphertext);
            fflush(stdout);

            decrypt(ciphertext, key);
            printf("plaintext: %s\n", plaintext);
            fflush(stdout);
        }
    }
    system("pause");
    return 0;
}