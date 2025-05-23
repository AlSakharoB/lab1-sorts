/**
 * @file passenger_sorting.cpp
 * @brief Реализация сортировки записей о пассажирах разными алгоритмами и измерение времени их работы.
 *
 * Поддерживаются сортировки: выбором, вставками, быстрая и std::sort. Данные берутся из CSV-файла,
 * а результаты сохраняются в отдельные файлы. Также логируется время выполнения каждого алгоритма.
 */


#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>


/**
 * @struct Passenger
 * @brief Структура для хранения информации о пассажире
 */
struct Passenger {
    std::string fullName;       ///< Полное имя пассажира
    int cabinNumber;            ///< Номер каюты
    std::string cabinType;      ///< Тип каюты (Люкс, 1, 2, 3)
    std::string destinationPort;///< Порт назначения


    /**
     * @brief Оператор < для сравнения пассажиров
     * @details Сначала сравнивает по номеру каюты, затем по порту назначения, затем по ФИО
     * @param other Другой пассажир для сравнения
     * @return true, если текущий пассажир "меньше" другого по указанным критериям
     */
    bool operator<(const Passenger& other) const {
        if (cabinNumber != other.cabinNumber)
            return cabinNumber < other.cabinNumber;
        if (destinationPort != other.destinationPort)
            return destinationPort < other.destinationPort;
        return fullName < other.fullName;
    }


    /**
     * @brief Оператор > для сравнения пассажиров
     * @param other Другой пассажир
     * @return true, если текущий пассажир "больше" другого (на основе оператора <)
     */
    bool operator>(const Passenger& other) const { return other < *this; }
    /**
     * @brief Оператор <= для сравнения пассажиров
     * @param other Другой пассажир
     * @return true, если текущий пассажир не больше другого
     */
    bool operator<=(const Passenger& other) const { return !(other < *this); }
    /**
     * @brief Оператор >= для сравнения пассажиров
     * @param other Другой пассажир
     * @return true, если текущий пассажир не меньше другого
     */
    bool operator>=(const Passenger& other) const { return !(*this < other); }
};


/**
 * @brief Сортировка выбором массива пассажиров
 * @param arr Вектор пассажиров
 */
void selectionSort(std::vector<Passenger>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i; // индекс минимального элемента
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) { // ищем минимальный элемент
                minIndex = j;
            }
        }
        if (minIndex != i)
            std::swap(arr[i], arr[minIndex]); // меняем текущий и минимальный
    }
}


/**
 * @brief Сортировка вставками массива пассажиров
 * @param arr Вектор пассажиров
 */
void insertionSort(std::vector<Passenger>& arr) {
    for (int i = 1; i < arr.size(); ++i) {
        Passenger key = arr[i]; // текущий элемент для вставки
        int j = i - 1;
        while (j >= 0 && arr[j] > key) { // пока элемент больше key, сдвигаем
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key; // вставка элемента на правильное место
    }
}


/**
 * @brief Быстрая сортировка массива пассажиров
 * @param arr Вектор пассажиров
 */
void quickSortR(std::vector<Passenger>& arr) {
    long i = 0, j = arr.size() - 1;
    Passenger p = arr[arr.size() >> 1];
    do {
        while (arr[i] < p) i++;
        while (arr[j] > p) j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++; j--;
        }
    } while (i <= j);
    if (j > 0) {
        std::vector<Passenger> left(arr.begin(), arr.begin() + j + 1);
        quickSortR(left);
        std::copy(left.begin(), left.end(), arr.begin());
    }
    if (i < arr.size()) {
        std::vector<Passenger> right(arr.begin() + i, arr.end());
        quickSortR(right);
        std::copy(right.begin(), right.end(), arr.begin() + i);
    }
}


/**
 * @brief Загрузка списка пассажиров из CSV-файла
 * @param filename Имя входного файла
 * @return Вектор пассажиров
 */
std::vector<Passenger> loadPassengers(const std::string& filename) {
    std::vector<Passenger> passengers;
    std::ifstream file(filename);
    if (!file.is_open()) return passengers;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string fullName, cabinNumberStr, cabinType, destinationPort;
        std::getline(ss, fullName, ',');
        std::getline(ss, cabinNumberStr, ',');
        std::getline(ss, cabinType, ',');
        std::getline(ss, destinationPort, ',');

        Passenger p;
        p.fullName = fullName;
        p.cabinNumber = std::stoi(cabinNumberStr);
        p.cabinType = cabinType;
        p.destinationPort = destinationPort;

        passengers.push_back(p);
    }
    return passengers;
}


/**
 * @brief Сохранение списка пассажиров в CSV-файл
 * @param filename Имя выходного файла
 * @param passengers Вектор пассажиров
 */
void savePassengers(const std::string& filename, const std::vector<Passenger>& passengers) {
    std::ofstream file(filename);
    file << "ФИО пассажира,Номер каюты,Тип каюты,Порт назначения\n";
    for (const auto& p : passengers) {
        file << p.fullName << "," << p.cabinNumber << "," << p.cabinType << "," << p.destinationPort << "\n";
    }
}


/**
 * \brief Точка входа программы.
 */
int main() {
    const std::string inputFile = "passengers.csv";
    std::vector<int> sizes = {100, 1000, 3000, 5000, 7000, 10000, 20000, 30000, 50000, 70000, 100000};
    auto allPassengers = loadPassengers(inputFile);
    if (allPassengers.empty()) return 1;

    std::ofstream logFile("timings.csv");
    logFile << "Size,SelectionSort,InsertionSort,QuickSort,StdSort";

    for (int size : sizes) {
        std::vector<Passenger> passengers(allPassengers.begin(), allPassengers.begin() + size);

        std::vector<Passenger> selSorted = passengers;
        std::vector<Passenger> insSorted = passengers;
        std::vector<Passenger> qckSorted = passengers;
        std::vector<Passenger> stdSorted = passengers;

        auto start = std::chrono::high_resolution_clock::now();
        selectionSort(selSorted);
        auto end = std::chrono::high_resolution_clock::now();
        auto timing = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        logFile << size << "," << timing << ",";

        start = std::chrono::high_resolution_clock::now();
        insertionSort(insSorted);
        end = std::chrono::high_resolution_clock::now();
        timing = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        logFile << timing << ",";

        start = std::chrono::high_resolution_clock::now();
        quickSortR(qckSorted);
        end = std::chrono::high_resolution_clock::now();
        timing = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        logFile << timing << ",";

        start = std::chrono::high_resolution_clock::now();
        std::sort(stdSorted.begin(), stdSorted.end());
        end = std::chrono::high_resolution_clock::now();
        timing = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        logFile << timing << "\n";

        savePassengers("sorted/ss_" + std::to_string(size) + ".csv", selSorted);
        savePassengers("sorted/is_" + std::to_string(size) + ".csv", insSorted);
        savePassengers("sorted/qs_" + std::to_string(size) + ".csv", qckSorted);
    }
    return 0;
}
