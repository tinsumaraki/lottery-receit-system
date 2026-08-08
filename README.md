# Receipt-Linked Lottery System

A C++ console application that simulates a receipt-linked lottery system inspired by Ethiopia's official VAT receipt infrastructure — and by Taiwan's real-world **Uniform Invoice** lottery, which has run since 1951.

## The idea

For every purchase where the pre-VAT amount is **100 birr or more**, the receipt gets:
- a flat **1 birr** lottery fee added to the total, and
- a unique, randomly generated **8-digit lottery number**.

Every quarter, a draw is run: **10 winners** are picked at random from all qualifying receipts, each winning **1,000,000 birr**. Whatever remains of the collected fees after paying the fixed 10,000,000-birr prize pool is tracked as the operator's surplus.

This project doesn't touch real money or a real VAT system — it's a self-contained simulation built to practice core C++ skills: file I/O, randomization, data modeling, and modular program design.

## Example

```
Item price:                 3,000.00 birr
VAT (15%):                    450.00 birr
Lottery fee (>=100 birr):       1.00 birr
---------------------------------------
Total charged:               3,451.00 birr
Lottery number issued:      84213097
```

## Project structure

```
lottery-receipt-system/
├── src/
│   ├── main.cpp                          # console menu, ties everything together
│   ├── receipt.h / receipt.cpp           # receipt creation, VAT + fee logic
│   ├── lottery_number.h / .cpp           # unique 8-digit number generation
│   ├── file_manager.h / .cpp             # CSV persistence (receipts & winners)
│   └── draw_engine.h / .cpp              # draw logic, prize payout, surplus
├── data/
│   ├── receipts.csv                      # created automatically on first run
│   └── winners.csv                       # created automatically on first run
├── tests/
│   ├── test_receipt.cpp                  # assert-based unit tests
│   └── run_tests.sh                      # build + run the tests
└── README.md
```

## Building and running

Requires a C++17 compiler (e.g. g++).

```bash
g++ -std=c++17 -Wall -Wextra -o lottery_app \
    src/main.cpp src/receipt.cpp src/lottery_number.cpp \
    src/file_manager.cpp src/draw_engine.cpp

./lottery_app
```

You'll see a menu:

```
===== Receipt-Linked Lottery System =====
1. Add a new receipt
2. Run quarterly draw
3. View report
4. Exit
```

Data is saved to `data/receipts.csv` and `data/winners.csv` and persists between runs.

## Running the tests

```bash
bash tests/run_tests.sh
```

## Design notes

- **Fee threshold:** only receipts with a pre-VAT amount ≥ 100 birr qualify for a lottery number.
- **Uniqueness:** lottery numbers are checked against every number ever issued (loaded from `receipts.csv` on startup), so no two receipts can share a number, even across separate runs of the program.
- **Draw fairness:** the draw engine shuffles all eligible lottery numbers and takes the first *N* as winners, guaranteeing no duplicate winners in a single draw.
- **Prize model:** each draw pays a fixed structure (10 winners × 1,000,000 birr). The difference between total fees collected and total prizes paid is tracked as the operator's surplus — mirroring how real-world receipt lotteries like Taiwan's Uniform Invoice actually work.

## Background

Ethiopia's VAT system requires most registered businesses to issue a printed, QR-coded receipt for eligible sales, with 15% VAT charged on top of the sale price. This project explores reusing that existing paper trail — without changing the receipt format itself — to fund a small public benefit at very low cost per transaction.

## Author

Tinsae Solomon — 2nd-year Computer Science student.
