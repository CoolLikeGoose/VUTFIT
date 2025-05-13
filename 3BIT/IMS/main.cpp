#include <iostream>
#include <cmath>
#include <fstream>
#include "data.h"


double findYearKnownValue(const std::vector<YearData>& data, int year, double YearData::*field) {
    for (int i = data.size() - 1; i >= 0; --i) {
        if (data[i].year == year && data[i].*field != -1) {
            return data[i].*field;
        }
    }
    return -1;
}

double calculateGDP(const std::vector<YearData>& data, int year) {
    double curGDP = findYearKnownValue(data, year, &YearData::gdp);

    if (curGDP != -1) {
        return curGDP;
    }

    double prevGDP = findYearKnownValue(data, year - 1, &YearData::gdp);

    return prevGDP * (1 + GDP_GROWTH);
}

double calculateHouseholds(const std::vector<YearData>& data, int year, double gdp) {
    double prevHouseholds = findYearKnownValue(data, year - 1, &YearData::households);
    double prevGDP = findYearKnownValue(data, year - 1, &YearData::gdp);

    return prevHouseholds * (1 + HOUSEHOLD_GROWTH * (gdp / prevGDP));
}

double calculateEnergyPrice(const std::vector<YearData>& data, int year) {
    double curPrice = findYearKnownValue(data, year, &YearData::energyPrice);

    if (curPrice != -1) {
        return curPrice;
    }

    double prevPrice = findYearKnownValue(data, year - 1, &YearData::energyPrice);
    return prevPrice * exp(PRICE_BETA);
}

double calculateEnergyConsumptionPerHousehold(double price, double gdp, double prevConsumption, double efficiency) {
    return prevConsumption 
            * pow(price, PRICE_ELASTICITY) 
            * pow(gdp, GDP_ELASTICITY)
            * pow(efficiency, EFF_ELASTICITY);
}

void addNewYearData(std::vector<YearData>& data, int year, double households, double energyConsumption, double gdp, double energyPrice) {
    // if year already exists then
    for (auto& entry : data) {
        if (entry.year == year) {
            // update year data
            entry.households = households;
            entry.energyConsumption = energyConsumption;
            entry.gdp = gdp;
            entry.energyPrice = energyPrice;
            return;
        }
    }

    // if year doesnt exist - add data
    data.push_back({year, households, energyConsumption, gdp, energyPrice});
}

void simulate(std::vector<YearData>& data, int startYear, int endYear) {
    std::ofstream outFile("e_cons.csv");

    outFile << "Year,Households,GDP,EnergyPrice,EnergyConsumptionPerHousehold,total_consumption\n";

    double efficiency = 1.0;
    double energyConsumption = BASE_CONSUMPTION;
    for (int year = startYear; year <= endYear; ++year) {
        double gdp = calculateGDP(data, year);
        double households = calculateHouseholds(data, year, gdp);
        double energyPrice = calculateEnergyPrice(data, year);

        energyConsumption = calculateEnergyConsumptionPerHousehold(energyPrice, gdp, energyConsumption, efficiency);
        double total_cons = energyConsumption * households;

        outFile << year << "," << households << "," << gdp << "," << energyPrice << "," << energyConsumption << total_cons << "\n";
        // Save data for next calculation
        addNewYearData(data, year, households, energyConsumption, gdp, energyPrice);
        efficiency *= (1.0+EFF_GROWTH);

        // Выводим данные
        std::cout << "Year: " << year
                  << " | Households: " << households
                  << " | GDP: $" << gdp
                  << " | Energy Price: $" << energyPrice
                  << " | E_Con per house: " << energyConsumption
                  << " kWh"
                  << " | Total cons: " << total_cons
                  << " kWh"
                  << std::endl;
    }

    outFile.close();
}

void calculateElasticityCoefficients(const std::vector<YearData>& data, int startYear, int endYear, 
                                     double& betaPrice, double& betaGDP, double& betaEff) {
    double sumLnP = 0.0, sumLnGDP = 0.0, sumLnE = 0.0, sumLnEff = 0.0;
    double sumLnP2 = 0.0, sumLnGDP2 = 0.0, sumLnPLnGDP = 0.0;
    double sumLnPLnE = 0.0, sumLnGDPLnE = 0.0, sumLnPLnEff = 0.0, sumLnGDPLnEff = 0.0;
    int n = 0;

    for (const auto& entry : data) {
        if (entry.year >= startYear && entry.year <= endYear) {
            double lnP = std::log(entry.energyPrice);
            double lnGDP = std::log(entry.gdp);
            double lnE = std::log(entry.energyConsumption);
            double lnEff = std::log(BASE_CONSUMPTION / entry.energyConsumption);

            sumLnP += lnP;
            sumLnGDP += lnGDP;
            sumLnE += lnE;
            sumLnEff += lnEff;

            sumLnP2 += lnP * lnP;
            sumLnGDP2 += lnGDP * lnGDP;
            sumLnPLnGDP += lnP * lnGDP;

            sumLnPLnE += lnP * lnE;
            sumLnGDPLnE += lnGDP * lnE;

            sumLnPLnEff += lnP * lnEff;
            sumLnGDPLnEff += lnGDP * lnEff;

            n++;
        }
    }

    double determinant = sumLnP2 * sumLnGDP2 - sumLnPLnGDP * sumLnPLnGDP;
    if (std::abs(determinant) < 1e-9) {
        std::cerr << "Determinant is too small, cannot solve regression!" << std::endl;
        return;
    }

    double invA11 = sumLnGDP2 / determinant;
    double invA12 = -sumLnPLnGDP / determinant;
    double invA21 = -sumLnPLnGDP / determinant;
    double invA22 = sumLnP2 / determinant;

    betaPrice = invA11 * sumLnPLnE + invA12 * sumLnGDPLnE;
    betaGDP = invA21 * sumLnPLnE + invA22 * sumLnGDPLnE;

    betaEff = (sumLnPLnEff * sumLnGDP2 - sumLnPLnGDP * sumLnGDPLnEff) / determinant;

    std::cout << "Elasticity Coefficients:" << std::endl;
    std::cout << "Price Elasticity (beta_P): " << betaPrice << std::endl;
    std::cout << "GDP Elasticity (beta_GDP): " << betaGDP << std::endl;
    std::cout << "Efficiency Elasticity (beta_Eff): " << betaEff << std::endl;
}


int main() {
    int startYear = 2001; // set start year of simulation
    int endYear = 2030; // set end year of simulation

    if (startYear < 2018) {
        startYear = 2018;
    }

    simulate(knownData, startYear, endYear);

    return 0;
}
