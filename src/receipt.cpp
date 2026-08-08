#include "receipt.h"
#include <sstream>
#include <iomanip>

double Receipt::total() const {
    double t = itemAmount + vatAmount;
    if (feeApplied) t += LOTTERY_FEE;
    return t;
}

std::string Receipt::toCsvRow() const {
    std::ostringstream oss;
    oss << receiptId << ","
        << std::fixed << std::setprecision(2) << itemAmount << ","
        << std::fixed << std::setprecision(2) << vatAmount << ","
        << (feeApplied ? "yes" : "no") << ","
        << lotteryNumber << ","
        << date;
    return oss.str();
}

Receipt createReceipt(int receiptId, double itemAmount, const std::string& date,
                       LotteryNumberGenerator& generator) {
    Receipt r;
    r.receiptId = receiptId;
    r.itemAmount = itemAmount;
    r.vatAmount = itemAmount * VAT_RATE;
    r.feeApplied = (itemAmount >= FEE_THRESHOLD);
    r.date = date;
    r.lotteryNumber = r.feeApplied ? generator.generate() : 0;
    return r;
}

Receipt receiptFromCsvRow(const std::string& row) {
    Receipt r;
    std::stringstream ss(row);
    std::string field;

    std::getline(ss, field, ','); r.receiptId = std::stoi(field);
    std::getline(ss, field, ','); r.itemAmount = std::stod(field);
    std::getline(ss, field, ','); r.vatAmount = std::stod(field);
    std::getline(ss, field, ','); r.feeApplied = (field == "yes");
    std::getline(ss, field, ','); r.lotteryNumber = std::stoll(field);
    std::getline(ss, field, ','); r.date = field;

    return r;
}
