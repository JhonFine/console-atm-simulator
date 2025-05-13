#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <clocale>
#include <numeric>
#include <limits>
using namespace std;

// creators: Leaf and Jhon_Fine
// Курси валют та обмеження
const int RATE_USD   = 8;
const int RATE_EUR   = 9;
const int MAX_NOTES  = 50;  // максимальна кількість банкнот за раз

// Функція для зчитування цілих чисел з перевіркою вводу
int readInt(bool ukrainian, const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        } else {
            cout << (ukrainian ? "Некоректний ввід. Спробуйте ще раз." : "Invalid input. Try again.") << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Функція для зчитування дійсних чисел з перевіркою вводу
double readDouble(bool ukrainian, const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        } else {
            cout << (ukrainian ? "Некоректний ввід. Спробуйте ще раз." : "Invalid input. Try again.") << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Перегляд запасів банкнот
void viewStocks(const vector<int>& denom, const vector<int>& count, const string& name) {
    cout << name << ":\n";
    for (size_t i = 0; i < denom.size(); ++i)
        cout << denom[i] << " x " << count[i] << endl;
}

// Налаштування запасів банкнот для обраної валюти
void configureStocks(const vector<int>& denom, vector<int>& count, bool ukrainian) {
    for (size_t i = 0; i < denom.size(); ++i) {
        string prompt = to_string(denom[i]) + (ukrainian ? " номінал: " : " denomination: ");
        count[i] = readInt(ukrainian, prompt);
    }
}

// Режим адміністратора
void adminMode(vector<int>& uahCnt, vector<int>& usdCnt, vector<int>& eurCnt,
               const vector<int>& uahDen, const vector<int>& usdDen, const vector<int>& eurDen,
               bool ukrainian)
{
    while (true) {
        cout << (ukrainian ? "=== АДМІН-РЕЖИМ ===\n" : "=== ADMIN MODE ===\n");
        cout << (ukrainian
            ? "1. Перегляд запасів банкнот\n2. Налаштувати запас банкнот\n3. Вихід з адм. режиму\nВибір: "
            : "1. View note stocks\n2. Configure note stocks\n3. Exit admin mode\nChoice: ");
        int choice = readInt(ukrainian, "");
        if (choice == 1) {
            viewStocks(uahDen, uahCnt, "UAH");
            viewStocks(usdDen, usdCnt, "USD");
            viewStocks(eurDen, eurCnt, "EUR");
        }
        else if (choice == 2) {
            cout << (ukrainian ? "Оберіть валюту (1-UAH,2-USD,3-EUR): " : "Choose currency (1-UAH,2-USD,3-EUR): ");
            int cur = readInt(ukrainian, "");
            if      (cur == 1) configureStocks(uahDen, uahCnt, ukrainian);
            else if (cur == 2) configureStocks(usdDen, usdCnt, ukrainian);
            else if (cur == 3) configureStocks(eurDen, eurCnt, ukrainian);
            else cout << (ukrainian ? "Невірний вибір" : "Invalid choice") << endl;
        }
        else if (choice == 3) {
            break;
        } else {
            cout << (ukrainian ? "Невірний вибір" : "Invalid choice") << endl;
        }
    }
}

// Видача банкнот із повідомленням доступної суми при помилці
bool dispense(const vector<int>& denom, vector<int>& count, int amount,
              bool ukrainian, const string& currency)
{
    vector<int> toDisp(denom.size());
    int rem = amount;
    for (size_t i = 0; i < denom.size(); ++i) {
        int use = min(rem / denom[i], count[i]);
        toDisp[i] = use;
        rem -= use * denom[i];
    }
    int avail = 0;
    for (size_t i = 0; i < denom.size(); ++i)
        avail += denom[i] * count[i];

    if (rem != 0) {
        cout << (ukrainian ? "Нестача банкнот" : "Not enough banknotes")
             << (ukrainian ? " Доступно: " : " Available: ")
             << avail << " " << currency << endl;
        return false;
    }
    int totalNotes = accumulate(toDisp.begin(), toDisp.end(), 0);
    if (totalNotes > MAX_NOTES) {
        cout << (ukrainian ? "Ліміт банкнот — 50. Спробуйте меншу суму." 
                           : "Note limit is 50. Try smaller amount.") << endl;
        cout << (ukrainian ? "Всього доступно: " : "Total available: ")
             << avail << " " << currency << endl;
        return false;
    }
    cout << (ukrainian ? "Видача:\n" : "Dispense:\n");
    for (size_t i = 0; i < denom.size(); ++i) {
        if (toDisp[i] > 0) {
            cout << denom[i] << " x " << toDisp[i]
                 << (ukrainian ? " шт" : " pcs") << endl;
            count[i] -= toDisp[i];
        }
    }
    return true;
}

// Операція зняття
void withdraw(vector<int>& uahCnt, vector<int>& usdCnt, vector<int>& eurCnt,
              const vector<int>& uahDen, const vector<int>& usdDen, const vector<int>& eurDen,
              int& balance, bool ukrainian, vector<string>& history)
{
    cout << (ukrainian ? "\nКурс: 1 USD = " : "\nRate: 1 USD = ")
         << RATE_USD << (ukrainian ? " UAH, 1 EUR = " : " UAH, 1 EUR = ")
         << RATE_EUR << " UAH" << endl;

    int cur;
    while (true) {
        cur = readInt(ukrainian, (ukrainian ? "1 UAH,2 USD,3 EUR. Вибір: " : "1 UAH,2 USD,3 EUR. Choice: "));
        if (cur >= 1 && cur <= 3) break;
        cout << (ukrainian ? "Введіть коректне число" : "Enter a valid number") << endl;
    }

    double amt = readDouble(ukrainian, (ukrainian ? "Введіть суму: " : "Enter amount: "));
    int costUah = int(amt * (cur == 2 ? RATE_USD : cur == 3 ? RATE_EUR : 1));
    if (costUah > balance) {
        cout << (ukrainian ? "Недостатньо коштів" : "Insufficient funds") << endl;
        return;
    }

    bool ok = false;
    if (cur == 1)    ok = dispense(uahDen, uahCnt, costUah, ukrainian, "UAH");
    else if (cur == 2) ok = dispense(usdDen, usdCnt, int(amt), ukrainian, "USD");
    else              ok = dispense(eurDen, eurCnt, int(amt), ukrainian, "EUR");

    if (ok) {
        balance -= costUah;
        cout << (ukrainian ? "Успішно. Баланс: " : "Done. Balance: ")
             << balance << " UAH" << endl;
        history.push_back(
            (ukrainian ? "Зняття: " : "Withdraw: ")
            + to_string(int(amt)) + (cur==1 ? " UAH" : cur==2 ? " USD" : " EUR")
        );
    }
}

// Операція внесення (UAH)
void deposit(vector<int>& count, const vector<int>& denom, int& balance,
             bool ukrainian, vector<string>& history)
{
    int dep = readInt(ukrainian, (ukrainian ? "Введіть суму UAH: " : "Enter UAH amount: "));
    if (dep < 1) return;

    vector<int> put(denom.size());
    int rem = dep;
    for (size_t i = 0; i < denom.size(); ++i) {
        put[i] = rem / denom[i];
        rem %= denom[i];
    }
    if (rem != 0) {
        cout << (ukrainian ? "Неможливо розділити на номінали" : "Cannot split into denominations") << endl;
        return;
    }
    cout << (ukrainian ? "Отримані купюри:\n" : "Received notes:\n");
    for (size_t i = 0; i < put.size(); ++i) {
        if (put[i] > 0) {
            cout << denom[i] << " x " << put[i]
                 << (ukrainian ? " номінал" : " pcs") << endl;
            count[i] += put[i];
        }
    }
    balance += dep;
    cout << (ukrainian ? "Успішно. Баланс: " : "Done. Balance: ")
         << balance << " UAH" << endl;
    history.push_back((ukrainian ? "Внесення: " : "Deposit: ") + to_string(dep) + " UAH");
}

// Перевірка балансу
void checkBalance(int balance, bool ukrainian, vector<string>& history) {
    cout << (ukrainian ? "Поточний баланс: " : "Current balance: ")
         << balance << " UAH" << endl;
    history.push_back((ukrainian ? "Перевірка балансу: " : "Checked balance: ")
                      + to_string(balance) + " UAH");
}

// Зміна PIN
void changePIN(int& validPin, bool ukrainian, vector<string>& history) {
    int oldp = readInt(ukrainian, (ukrainian ? "Введіть старий PIN: " : "Enter old PIN: "));
    if (oldp != validPin) {
        cout << (ukrainian ? "Невірний PIN" : "Wrong PIN") << endl;
        return;
    }
    int newp = readInt(ukrainian, (ukrainian ? "Введіть новий PIN: " : "Enter new PIN: "));
    int newp2 = readInt(ukrainian, (ukrainian ? "Підтвердіть PIN: " : "Confirm PIN: "));
    if (newp != newp2) {
        cout << (ukrainian ? "PIN не збігаються" : "PINs do not match") << endl;
        return;
    }
    validPin = newp;
    cout << (ukrainian ? "PIN змінено успішно" : "PIN changed successfully") << endl;
    history.push_back(ukrainian ? "Зміна PIN" : "Changed PIN");
}

// Перегляд історії операцій
void viewHistory(const vector<string>& history, bool ukrainian) {
    cout << (ukrainian ? "\n=== Історія операцій ===\n" : "\n=== Transaction History ===\n");
    if (history.empty()) {
        cout << (ukrainian ? "Немає записів." : "No records.") << endl;
    } else {
        for (size_t i = 0; i < history.size(); ++i) {
            cout << i+1 << ". " << history[i] << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    srand((unsigned)time(nullptr));

    vector<int> uahDen = {1000,500,200,100,50,20,10,5,2,1};
    vector<int> usdDen = {100,50,20,10,5,2,1};
    vector<int> eurDen = {100,50,20,10,5,2,1};
    vector<int> uahCnt(uahDen.size()), usdCnt(usdDen.size()), eurCnt(eurDen.size());
    for (auto& c : uahCnt) c = rand() % 501;
    for (auto& c : usdCnt) c = rand() % 501;
    for (auto& c : eurCnt) c = rand() % 501;

    int balance      = rand() % 10000;
    const int ADMIN_PIN = 0;
    int validPin     = 7777;
    vector<string> history;    // зберігаємо історію операцій

    while (true) {
        int lang = readInt(true, "Виберіть мову / Select language (1. Українська, 2. English): ");
        bool ua = (lang == 1);
        
        int pin;
        int attempts = 3;
        while (true) {
            pin = readInt(ua, (ua ? "\nВведіть PIN: " : "\nEnter PIN: "));
            if (pin == ADMIN_PIN) {
                adminMode(uahCnt, usdCnt, eurCnt, uahDen, usdDen, eurDen, ua);
                continue;
            }
            if (pin == validPin) break;
            if (--attempts > 0) {
                cout << (ua ? "Невірний PIN. Спроб лишилося: " : "Wrong PIN. Attempts left: ") << attempts << endl;
            } else {
                cout << (ua ? "Блокування. Зверніться до підтримки." : "Locked. Contact support.") << endl;
                return 0;
            }
        }

        int choice;
        do {
            choice = readInt(ua, (ua ? "\nМеню:\n1. Зняти готівку\n2. Внести готівку\n3. Перевірити баланс\n4. Змінити PIN\n5. Історія операцій\n6. Повернутися до вибору мови\n7. Вихід\nВибір: "
                                    : "\nMenu:\n1. Withdraw\n2. Deposit\n3. Check Balance\n4. Change PIN\n5. Transaction History\n6. Restart to language selection\n7. Exit\nChoice: "));
            switch (choice) {
            case 1: withdraw(uahCnt, usdCnt, eurCnt, uahDen, usdDen, eurDen, balance, ua, history); break;
            case 2: deposit(uahCnt, uahDen, balance, ua, history); break;
            case 3: checkBalance(balance, ua, history); break;
            case 4: changePIN(validPin, ua, history); break;
            case 5: viewHistory(history, ua); break;
            case 6: break;  // повернення до вибору мови
            case 7:
                cout << (ua ? "До побачення!" : "Goodbye!") << endl;
                return 0;
            default: cout << (ua ? "Невірний вибір" : "Invalid choice") << endl; break;
            }
        } while (choice != 6);
    }

    return 0;
}
