#!/bin/bash
# Build and run the test suite from the project root: bash tests/run_tests.sh
set -e
cd "$(dirname "$0")/.."

g++ -std=c++17 -Wall -Wextra -o tests/test_receipt \
    tests/test_receipt.cpp src/receipt.cpp src/lottery_number.cpp

./tests/test_receipt
