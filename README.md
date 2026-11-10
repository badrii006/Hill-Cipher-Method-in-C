# Hill Cipher Method in C

A C implementation of the **Hill cipher**, a classical polygraphic substitution cipher that uses linear algebra (matrix multiplication modulo 26) to encrypt and decrypt text.

## How It Works

The Hill cipher, invented by Lester S. Hill in 1929, encrypts blocks of letters at once instead of one letter at a time.

1. **Map letters to numbers:** A=0, B=1, ..., Z=25.
2. **Choose a key matrix** `K` of size n × n.
3. **Encrypt:** split the plaintext into blocks of n letters, treat each block as a column vector `P`, and compute:

   ```
   C = K · P  (mod 26)
   ```

4. **Decrypt:** multiply by the inverse of the key matrix modulo 26:

   ```
   P = K⁻¹ · C  (mod 26)
   ```

### Key requirement

The key matrix must be **invertible modulo 26**. This means `gcd(det(K), 26) = 1`. If the determinant shares a factor with 26 (i.e., it is even or a multiple of 13), the key is invalid and the message cannot be decrypted.

### Example (2 × 2 key)

Key:

```
K = | 3  3 |
    | 2  5 |
```

Plaintext `HELP` → blocks `HE` and `LP`

```
HE = [7, 4]   →  K · [7, 4]  mod 26 = [7, 10]  →  "HK"
LP = [11, 15] →  K · [11, 15] mod 26 = [10, 19] →  "KT"
```

Ciphertext: `HKKT`

## Features

- Encrypts plaintext using a key matrix
- Decrypts ciphertext using the modular inverse of the key matrix
- Validates that the key matrix is invertible mod 26
- Pads the message if its length is not a multiple of the block size

> Edit this list to match what your program actually does.

## Getting Started

### Prerequisites

- A C compiler such as `gcc` or `clang`

### Compile

```bash
gcc hill.c -o hill
```

> Replace `hill.c` with the name of your source file.

### Run

```bash
./hill
```

### Sample session

```
Enter the size of the key matrix: 2
Enter the key matrix:
3 3
2 5
Enter the plaintext: HELP

Ciphertext: HKKT
Decrypted text: HELP
```

> Replace this with real output from your program.

## Project Structure

```
.
├── hill.c       # Main source file
└── README.md
```

## Concepts Used

- Matrix multiplication
- Determinants and adjugate (cofactor) matrices
- Modular arithmetic and modular multiplicative inverse
- Extended Euclidean algorithm / greatest common divisor

## Limitations

- Only alphabetic characters (A-Z) are supported; spaces and punctuation are stripped or ignored.
- The Hill cipher is vulnerable to known-plaintext attacks, so it is meant for **learning**, not real security.

## License

This project is licensed under the MIT License. Add a `LICENSE` file if you choose this.

## Author

**Badri**
