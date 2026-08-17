#include <array>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

constexpr int rows = 2;
constexpr int columns = 50;

class SeatReservation {
  public:
    SeatReservation() : seats_{} {}
    bool reserve(int agency, std::mt19937 &random) {
        std::uniform_int_distribution<int> row(0, rows - 1);
        std::uniform_int_distribution<int> column(0, columns - 1);
        std::lock_guard lock(mutex_);
        for (int attempt = 0; attempt < rows * columns; ++attempt) {
            const int selected_row = row(random);
            const int selected_column = column(random);
            if (seats_[selected_row][selected_column] == 0) {
                seats_[selected_row][selected_column] = agency;
                return true;
            }
        }
        return false;
    }
    void print() const {
        std::lock_guard lock(mutex_);
        for (const auto &row : seats_) {
            for (int seat : row)
                std::cout << seat << ' ';
            std::cout << '\n';
        }
    }

  private:
    mutable std::mutex mutex_;
    std::array<std::array<int, columns>, rows> seats_;
};

int main(int argc, char **argv) {
    const unsigned seed = argc > 1 ? static_cast<unsigned>(std::strtoul(argv[1], nullptr, 10)) : 1;
    const int reservations_per_agency = argc > 2 ? std::atoi(argv[2]) : 50;
    if (reservations_per_agency < 0) {
        std::cerr << "reservation count must not be negative\n";
        return EXIT_FAILURE;
    }
    SeatReservation reservation;
    auto agency = [&](int id, unsigned agency_seed) {
        std::mt19937 random(agency_seed);
        int completed = 0;
        for (int attempt = 0; attempt < reservations_per_agency; ++attempt) {
            if (reservation.reserve(id, random))
                ++completed;
        }
        std::cout << "Agency " << id << " reserved " << completed << " seats\n";
    };
    std::thread first(agency, 1, seed);
    std::thread second(agency, 2, seed + 1);
    first.join();
    second.join();
    reservation.print();
    return EXIT_SUCCESS;
}
