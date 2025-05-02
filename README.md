# Multi-Currency ATM Simulator (C++)

A simple console-based ATM simulation written in C++, supporting UAH, USD and EUR operations.

## Features

* **Multi-language interface**: Ukrainian & English prompts
* **Multi-currency withdraw/deposit** with real-time conversion (1 USD = 8 UAH, 1 EUR = 9 UAH)
* **Admin mode** to view and configure note stock levels
* **PIN management**: user PIN change, admin PIN for maintenance
* **Denomination splitting** for deposits and withdrawals
* **Error handling** for invalid input, insufficient funds, note shortages and note‑count limits (max 50 notes)

## Getting Started

### Prerequisites

* A C++17‑compatible compiler (e.g. g++, clang++)
* C++ standard library

### Build

```bash
# Compile
g++ -std=c++17 -o atm atm.cpp

# Run
./atm
```

## Usage

1. **Select language**: Ukrainian or English.
2. **Enter PIN** (default user PIN is `7777`, admin PIN is `0`).
3. **Main menu**:

   * Withdraw (зняти готівку)
   * Deposit (внести готівку)
   * Check Balance (перевірити баланс)
   * Change PIN (змінити PIN)
   * Restart (повернутися до початку)
   * Exit (вихід)
4. **Admin mode** (enter PIN `0`):

   * View current note stocks
   * Configure note counts per denomination

## Code Structure

* `main()` handles language/PIN/menu loops
* `withdraw()`, `deposit()`, `checkBalance()`, `changePIN()` for user operations
* `adminMode()`, `viewStocks()`, `configureStocks()` for maintenance
* `dispense()` handles note splitting and error messages

## Customization

* Adjust exchange rates (`RATE_USD`, `RATE_EUR`) at top of `atm.cpp`
* Modify supported denominations in `uahDen`, `usdDen`, `eurDen` vectors
* Change max notes limit via `MAX_NOTES`

## License

MIT © \Jhon_Fine and Leaf
