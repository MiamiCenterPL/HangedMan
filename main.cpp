#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif

// Stała reprezentująca hasło
std::string password = "test";
std::wstring hanger[] = {
    L"        \n        \n        \n        \n        \n        \n         ", //0
    L"        \n        \n        \n        \n        \n        \n═══════╩|", //1
    L"        \n       ║\n       ║\n       ║\n       ║\n       ║\n═══════╩|", //2
    L"  ╒════╗\n       ║\n       ║\n       ║\n       ║\n       ║\n═══════╩|", //3
    L"  ╒════╗\n  │    ║\n       ║\n       ║\n       ║\n       ║\n═══════╩|", //4
    L"  ╒════╗\n  │    ║\n  O    ║\n       ║\n       ║\n       ║\n═══════╩|", //5
    L"  ╒════╗\n  │    ║\n  O    ║\n  |    ║\n       ║\n       ║\n═══════╩|", //6
    L"  ╒════╗\n  │    ║\n  O    ║\n └|    ║\n       ║\n       ║\n═══════╩|", //7
    L"  ╒════╗\n  │    ║\n  O    ║\n └|┘   ║\n       ║\n       ║\n═══════╩|", //8
    L"  ╒════╗\n  │    ║\n  O    ║\n └|┘   ║\n ┌     ║\n       ║\n═══════╩|", //9
    L"  ╒════╗\n  │    ║\n  O    ║\n └|┘   ║\n ┌▼    ║\n       ║\n═══════╩|", //10
    L"  ╒════╗\n  │    ║\n  O    ║\n └|┘   ║\n ┌▼┐   ║\n       ║\n═══════╩|", //11
};
std::vector <char> guessedLetters;

// Funkcja rysująca wisielca
void PrintGame(int bledy) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::wcout << hanger[bledy] << std::endl;
    if (bledy > 10) {
        std::wcout << L"Przegrałeś! Chcesz zagrac jeszcze raz?" << std::endl;
        return;
    }
}

// Funkcja sprawdzająca czy litera jest w haśle
bool CheckLetter(char letter) {
    letter = tolower(letter);
    for (int i = 0; i < password.length(); i++) {
        if (tolower(password[i]) == letter) {
            return true;
        }
    }
    return false;
}

// Funkcja pokazujaca odgadniete litery w hasle, np. a _ _ a _ a _ a _ _ a
void ShowPassword() {
    for (int i = 0; i < password.length(); i++) {
        if (std::find(guessedLetters.begin(), guessedLetters.end(), tolower(password[i])) != guessedLetters.end()) {
            std::wcout << password[i] << " ";
        }
        else {
            std::wcout << L"_ ";
        }
    }
    std::wcout << std::endl;
}

int GetPlayerInputChoice() {
    std::wcout << L"Co chcesz teraz zrobic?" << std::endl;
    std::wcout << L"1. Podac litere" << std::endl;
    std::wcout << L"2. Odgadnac haslo" << std::endl;
    std::wcout << L"3. Poddac sie" << std::endl;
    std::string playerInput;
    while (1) {
        std::cin >> playerInput;
        std::regex reg("[1-3]");
        if (!std::regex_match(playerInput, reg)) {
            std::wcout << L"Podaj liczbe od 1 do 3!" << std::endl;
            continue;
        }
        return std::stoi(playerInput);
    }
}

char GetPlayerInputChar() {
    std::wcout << L"Podaj litere: ";
    std::string playerInput;
    while (1) {
        std::cin >> playerInput;
        std::regex reg("[a-zA-Z]");
        if (!std::regex_match(playerInput, reg)) {
            std::wcout << L"Podaj litere!" << std::endl;
            continue;
        }
        return tolower(playerInput[0]);
    }
}

void GetPlayerPassword() {
    std::string playerInput;
    std::wcout << L"Podaj haslo: ";
    std::cin >> playerInput;
    std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);
    std::string lowerPassword = password;
    std::transform(lowerPassword.begin(), lowerPassword.end(), lowerPassword.begin(), ::tolower);
    if (playerInput == lowerPassword) {
        std::wcout << L"Gratulacje! Odgadles haslo!" << std::endl;
    }
    else {
        PrintGame(11);
        std::wcout << L"Przegrales! Haslo to: " << std::wstring(password.begin(), password.end()) << std::endl;
    }
}

bool GetPlayerInputYN() {
    std::string playerInput;
    while (1) {
        std::cin >> playerInput;
        std::regex reg("[TtNn]");
        if (!std::regex_match(playerInput, reg)) {
            std::wcout << L"Podaj T lub N!" << std::endl;
            continue;
        }
        return playerInput == "T" || playerInput == "t";
    }
}

void SetRandomPassword() {
    std::ifstream file("passwords.txt");
    if (!file.is_open()) {
        std::wcout << L"Nie można otworzyć pliku passwords.txt" << std::endl;
        return;
    }

    std::vector<std::string> passwords;
    std::string line;
    while (std::getline(file, line)) {
        passwords.push_back(line);
    }
    file.close();

    if (passwords.empty()) {
        std::wcout << L"Plik passwords.txt jest pusty" << std::endl;
        return;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int randomIndex = std::rand() % passwords.size();
    password = passwords[randomIndex];
}

//Check if all letters are guessed
bool CheckWin() {
    for (int i = 0; i < password.length(); i++) {
        if (std::find(guessedLetters.begin(), guessedLetters.end(), tolower(password[i])) == guessedLetters.end()) {
            return false;
        }
    }
    return true;
}

bool GetStartNewGame() {
    std::wcout << L"Czy chcesz zagrac jeszcze raz? (T/N)" << std::endl;
    guessedLetters.clear();
    SetRandomPassword();
    return GetPlayerInputYN();
}

int main() {
    // Ustawienia kodowania konsoli na UTF-8
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U16TEXT);
#endif
    int b = 0;
    bool inGame = true;
    bool newGame = true;
    while (inGame) { //Main Loop
        if (newGame) {
            PrintGame(0);
            b = 0;
            std::wcout << L"Witaj w grze 'Wisielec'!" << std::endl;
            SetRandomPassword();
            ShowPassword();
            newGame = false;
        }
        switch (GetPlayerInputChoice())
        {
        case 1:
        {
            char letter = GetPlayerInputChar();
            if (CheckLetter(letter)) {
                guessedLetters.push_back(letter);
                if (CheckWin()) {
                    std::wcout << L"Gratulacje! Odgadles haslo!" << std::endl;
                    inGame = GetStartNewGame();
					newGame = true;
                    break;
                }
            }
            else {
                b++;
            }
            PrintGame(b);
            ShowPassword();
            break;
        }
        case 2:
            GetPlayerPassword();
            inGame = GetStartNewGame();
            newGame = true;
            break;
        case 3:
            PrintGame(11);
            std::wcout << L"Przegrales! Haslo to: " << std::wstring(password.begin(), password.end()) << std::endl;
            inGame = GetStartNewGame();
            newGame = true;
            break;
        default:
            break;
        }
    }

    return 0;
}