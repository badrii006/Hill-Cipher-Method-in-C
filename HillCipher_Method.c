#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define SIZE 10

/* modular inverse of a (mod 26), returns 0 if a has no inverse */
int modinv(int a)
{
    int x;
    a = ((a % 26) + 26) % 26;
    for (x = 1; x < 26; x++)
    {
        if ((a * x) % 26 == 1)
            return x;
    }
    return 0;
}

/* Gauss-Jordan inversion of K (mod 26).
   Fills inv with K^-1 and returns det(K) mod 26.
   Returns 0 if K can't be inverted mod 26 (det%2==0 or det%13==0). */
int gauss_jordan_inverse(int n, int k[SIZE][SIZE], int inv[SIZE][SIZE])
{
    int a[SIZE][2 * SIZE];
    int i, j, l, r, t, ratio, det = 1;

    /* Augmenting identity matrix of order n */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            a[i][j] = k[i][j] % 26;
            if (i == j)
                a[i][j + n] = 1;
            else
                a[i][j + n] = 0;
        }
    }

    /* Applying Gauss Jordan Elimination */
    for (i = 0; i < n; i++)
    {
        /* pivot must be invertible mod 26, else swap with a lower row that has one */
        if (modinv(a[i][i]) == 0)
        {
            r = i + 1;
            while (r < n && modinv(a[r][i]) == 0)
                r++;
            if (r == n)
                return 0;
            for (l = 0; l < 2 * n; l++)
            {
                t = a[i][l];
                a[i][l] = a[r][l];
                a[r][l] = t;
            }
            det = -det;
        }
        det = (det * a[i][i]) % 26;

        for (j = 0; j < n; j++)
        {
            if (i != j)
            {
                ratio = (a[j][i] * modinv(a[i][i])) % 26;
                for (l = 0; l < 2 * n; l++)
                    a[j][l] = ((a[j][l] - ratio * a[i][l]) % 26 + 26) % 26;
            }
        }
    }

    /* Row operation to make principal diagonal 1 */
    for (i = 0; i < n; i++)
    {
        t = modinv(a[i][i]);
        for (j = n; j < 2 * n; j++)
            a[i][j] = (a[i][j] * t) % 26;
    }

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            inv[i][j] = a[i][j + n];

    return ((det % 26) + 26) % 26;
}

/* value 1..26 (0 means 26) -> letter */
char letter(int v)
{
    v = ((v % 26) + 26) % 26;
    if (v == 0)
        v = 26;
    return 'A' + v - 1;
}

int main()
{
    char in[100];
    int p[SIZE], c[SIZE], d[SIZE];
    int k[SIZE][SIZE], kinv[SIZE][SIZE];
    int i, j, n, det, ok, choice, max = 50, min = 1;

    srand(time(NULL));

    do
    {
        printf("\n1. Encrypt\n2. Decrypt\n3. Exit\nEnter choice: ");
        if (scanf("%d", &choice) != 1)
            return 1;

        switch (choice)
        {
            case 1:
                printf("enter word (1 to %d letters)\n", SIZE);
                scanf("%99s", in);
                n = strlen(in);

                ok = (n >= 1 && n <= SIZE);
                for (i = 0; ok && i < n; i++)
                {
                    in[i] = toupper(in[i]);
                    if (in[i] < 'A' || in[i] > 'Z')
                        ok = 0;
                }
                if (!ok)
                {
                    printf("invalid input: use only letters A-Z, up to %d\n", SIZE);
                    break;
                }

                /* convert to matrix P */
                printf("\nP matrix:\n");
                for (i = 0; i < n; i++)
                {
                    p[i] = in[i] - 'A' + 1;
                    printf("%d ", p[i]);
                }
                printf("\n");

                /* generate key matrix K (n x n); new key if det%2==0 or det%13==0 */
                do
                {
                    for (i = 0; i < n; i++)
                    {
                        for (j = 0; j < n; j++)
                        {
                            k[i][j] = (rand() % (max - min + 1)) + min;
                        }
                    }
                    det = gauss_jordan_inverse(n, k, kinv);
                } while (det % 2 == 0 || det % 13 == 0);

                printf("\nKey matrix K:\n");
                for (i = 0; i < n; i++)
                {
                    for (j = 0; j < n; j++)
                        printf("%d ", k[i][j]);
                    printf("\n");
                }
                printf("det(K) mod 26 = %d\n", det);

                /* C = P*K mod 26 */
                for (j = 0; j < n; j++)
                {
                    c[j] = 0;
                    for (i = 0; i < n; i++)
                        c[j] += p[i] * k[i][j];
                    c[j] = c[j] % 26;
                }

                printf("\nEncrypted matrix C:\n");
                for (j = 0; j < n; j++)
                    printf("%d ", c[j]);
                printf("\nEncrypted text: ");
                for (j = 0; j < n; j++)
                    printf("%c", letter(c[j]));
                printf("\n");

                printf("\nDecryption matrix K-1 (mod 26):\n");
                for (i = 0; i < n; i++)
                {
                    for (j = 0; j < n; j++)
                        printf("%d ", kinv[i][j]);
                    printf("\n");
                }
                break;

            case 2:
                printf("enter number of letters (1 to %d): ", SIZE);
                if (scanf("%d", &n) != 1 || n < 1 || n > SIZE)
                {
                    printf("invalid size\n");
                    return 1;
                }

                printf("enter encrypted matrix C (%d values):\n", n);
                for (i = 0; i < n; i++)
                    scanf("%d", &c[i]);

                printf("enter decryption matrix K-1 (%d x %d):\n", n, n);
                for (i = 0; i < n; i++)
                    for (j = 0; j < n; j++)
                        scanf("%d", &kinv[i][j]);

                /* P = C*K-1 mod 26 */
                for (j = 0; j < n; j++)
                {
                    d[j] = 0;
                    for (i = 0; i < n; i++)
                        d[j] += c[i] * kinv[i][j];
                    d[j] = ((d[j] % 26) + 26) % 26;
                }

                /* convert to letters */
                printf("\nDecrypted letters: ");
                for (j = 0; j < n; j++)
                    printf("%c", letter(d[j]));
                printf("\n");
                break;

            case 3:
                printf("Exiting\n");
                break;

            default:
                printf("invalid choice\n");
        }
    } while (choice != 3);

    return 0;
}