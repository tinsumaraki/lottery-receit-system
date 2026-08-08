#include "file_manager.h"
#include <fstream>
#include <sstream>
#include <iomanip>

std::string WinnerRecord::toCsvRow() const {
    std::ostringstream oss;
    oss << drawDate << ","
        << lotteryNumber << ","
        << std::fixed << std::setprecision(2) << prizeAmount;
    return oss.str();
}

WinnerRecord winnerFromCsvRow(const std::string& row) {
    WinnerRecord w;
    std::stringstream ss(row);
    std::string field;

    std::getline(ss, field, ','); w.drawDate = field;
    std::getline(ss, field, ','); w.lotteryNumber = std::stoll(field);
    std::getline(ss, field, ','); w.prizeAmount = std::stod(field);

    return w;
}

FileManager::FileManager(const std::string& receiptsPath, const std::string& winnersPath)
    : receiptsPath(receiptsPath), winnersPath(winnersPath) {
    ensureFileWithHeader(receiptsPath, "receiptId,itemAmount,vatAmount,feeApplied,lotteryNumber,date");
    ensureFileWithHeader(winnersPath, "drawDate,lotteryNumber,prizeAmount");
}

void FileManager::ensureFileWithHeader(const std::string& path, const std::string& header) const {
    std::ifstream check(path);
    if (check.good()) return; // file already exists, leave it alone

    std::ofstream create(path);
    create << header << "\n";
}

std::vector<Receipt> FileManager::loadReceipts() const {
    std::vector<Receipt> receipts;
    std::ifstream in(receiptsPath);
    std::string line;

    std::getline(in, line); // skip header
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        receipts.push_back(receiptFromCsvRow(line));
    }
    return receipts;
}

std::unordered_set<long long> FileManager::loadUsedLotteryNumbers() const {
    std::unordered_set<long long> used;
    for (const Receipt& r : loadReceipts()) {
        if (r.lotteryNumber != 0) used.insert(r.lotteryNumber);
    }
    return used;
}

int FileManager::nextReceiptId() const {
    auto receipts = loadReceipts();
    int maxId = 0;
    for (const Receipt& r : receipts) {
        if (r.receiptId > maxId) maxId = r.receiptId;
    }
    return maxId + 1;
}

void FileManager::appendReceipt(const Receipt& r) const {
    std::ofstream out(receiptsPath, std::ios::app);
    out << r.toCsvRow() << "\n";
}

std::vector<WinnerRecord> FileManager::loadWinners() const {
    std::vector<WinnerRecord> winners;
    std::ifstream in(winnersPath);
    std::string line;

    std::getline(in, line); // skip header
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        winners.push_back(winnerFromCsvRow(line));
    }
    return winners;
}

void FileManager::appendWinner(const WinnerRecord& w) const {
    std::ofstream out(winnersPath, std::ios::app);
    out << w.toCsvRow() << "\n";
}
