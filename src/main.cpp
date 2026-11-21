#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

int main() {

    std::ifstream file("data/latest_curve.csv");

    if (!file.is_open()) {
        std::cerr << "Could not open yield curve file.\n";
        return 1;
    }

    std::map<int, double> yields;

    std::string line;

    // Saltar header: maturity,ytm
    std::getline(file, line);

    while (std::getline(file, line)) {

        std::stringstream ss(line);

        std::string maturityText;
        std::string ytmText;

        std::getline(ss, maturityText, ',');
        std::getline(ss, ytmText, ',');

        int maturity = std::stoi(maturityText);
        double ytm = std::stod(ytmText);

        yields[maturity] = ytm;
    }

    file.close();

    std::cout << "2Y yield: " << yields[2] << '\n';
    std::cout << "10Y yield: " << yields[10] << '\n';

    return 0;
}