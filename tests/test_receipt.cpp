// Basic assert-based tests. Run with: bash tests/run_tests.sh
#include <cassert>
#include <iostream>
#include "../src/receipt.h"
#include "../src/lottery_number.h"

void test_vat_calculation() {
    LotteryNumberGenerator gen;
    Receipt r = createReceipt(1, 1000.0, "2026-01-01", gen);
    assert(r.vatAmount == 150.0); // 15% of 1000
    std::cout << "test_vat_calculation passed\n";
}

void test_fee_applies_at_threshold() {
    LotteryNumberGenerator gen;
    Receipt exact = createReceipt(1, 100.0, "2026-01-01", gen);
    assert(exact.feeApplied == true);

    Receipt below = createReceipt(2, 99.99, "2026-01-01", gen);
    assert(below.feeApplied == false);
    assert(below.lotteryNumber == 0);

    std::cout << "test_fee_applies_at_threshold passed\n";
}

void test_total_includes_fee_only_when_applied() {
    LotteryNumberGenerator gen;
    Receipt qualifies = createReceipt(1, 100.0, "2026-01-01", gen);
    // 100 + 15 VAT + 1 fee = 116
    assert(qualifies.total() == 116.0);

    Receipt doesNot = createReceipt(2, 50.0, "2026-01-01", gen);
    // 50 + 7.5 VAT, no fee = 57.5
    assert(doesNot.total() == 57.5);

    std::cout << "test_total_includes_fee_only_when_applied passed\n";
}

void test_lottery_numbers_are_unique() {
    LotteryNumberGenerator gen;
    long long a = gen.generate();
    long long b = gen.generate();
    assert(a != b);
    assert(a >= 10000000 && a <= 99999999);
    assert(b >= 10000000 && b <= 99999999);
    std::cout << "test_lottery_numbers_are_unique passed\n";
}

void test_csv_round_trip() {
    LotteryNumberGenerator gen;
    Receipt original = createReceipt(7, 250.0, "2026-03-15", gen);
    std::string row = original.toCsvRow();
    Receipt reloaded = receiptFromCsvRow(row);

    assert(reloaded.receiptId == original.receiptId);
    assert(reloaded.itemAmount == original.itemAmount);
    assert(reloaded.feeApplied == original.feeApplied);
    assert(reloaded.lotteryNumber == original.lotteryNumber);

    std::cout << "test_csv_round_trip passed\n";
}

int main() {
    test_vat_calculation();
    test_fee_applies_at_threshold();
    test_total_includes_fee_only_when_applied();
    test_lottery_numbers_are_unique();
    test_csv_round_trip();
    std::cout << "\nAll tests passed!\n";
    return 0;
}
