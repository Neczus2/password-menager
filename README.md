# Password Manager (C)

A clean, command-line password manager built in C. This tool allows you to add, list, search, and delete website credentials, generate random passwords, and manage your accounts seamlessly with local file persistence.

> **Note:** This is an educational project. Credentials are stored in plain text in `passwords.txt`. Do not use it for real secrets.

## Features

- **Account Management** – Add, list, search, and delete website credentials with optional username/email support.
- **Case-Insensitive Search** – Quickly find stored accounts by website name.
- **Password Generator** – Generate random passwords (8–32 characters) containing letters, numbers, and symbols.
- **Data Persistence** – Automatically saves accounts to a local file (`passwords.txt`).

## Project Structure

```text
password-manager/
├── main.c              # Application source code

└── README.md           # Project documentation
```

## Running the Application

### 1. Compile the code
Using `gcc`:
```bash
gcc -std=c99 -Wall -Wextra -O2 main.c -o password_manager
```

### 2. Run the program
```bash
./password_manager
```
*(On Windows PowerShell, use `.\password_manager.exe`)*

## Usage

The program provides an interactive command-line menu:

```text
=== PASSWORD MANAGER ===
1. Add account
2. List accounts
3. Search account
4. Generate password
5. Delete account
6. Exit
Choose an option: 
```

## Security Disclaimer

`rand()` is not cryptographically secure and credentials are stored unencrypted in plain text. This project is meant for learning and demonstration purposes only.
