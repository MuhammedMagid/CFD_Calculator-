#include <iostream>
#include <cmath>
#include <iomanip>

double calcSettlingVelocity(double diameter, double particleDensity, double fluidDensity, double fluidViscosity, double dragFunction) {
    const double gravity = 9.81;
    return gravity * diameter * diameter * (particleDensity - fluidDensity) / (18 * fluidViscosity * dragFunction);
}

double calcReynoldsNumber(double fluidDensity, double settlingVelocity, double diameter, double fluidViscosity) {
    return (fluidDensity * settlingVelocity * diameter) / fluidViscosity;
}

double calcDragFunction(double reynoldsNumber) {
    if (reynoldsNumber <= 1000) {
        return 1 + 0.15 * pow(reynoldsNumber, 0.687);
    } else {
        return 0.0183 * reynoldsNumber;
    }
}

double calccd(double dragFunction, double reynoldsNumber) {
    return dragFunction * 24 / reynoldsNumber;
}

int main() {
    double diameterEntry, densityEntry;
    std::string fluidChoice;

    std::cout << "Enter particle diameter (m): ";
    std::cin >> diameterEntry;
    std::cout << "Enter particle density (kg/m^3): ";
    std::cin >> densityEntry;
    std::cout << "Enter fluid choice (Water or Air): ";
    std::cin >> fluidChoice;

    if (diameterEntry <= 0 || densityEntry <= 0 || diameterEntry > 0.01) {
        std::cout << "Invalid input. Ensure diameter is <1cm and values are positive." << std::endl;
        return 1;
    }

    double fluidDensity, fluidViscosity;
    if (fluidChoice == "Water" || fluidChoice == "water") {
        fluidDensity = 1000;
        fluidViscosity = 1e-3;
    } else if (fluidChoice == "Air" || fluidChoice == "air") {
        fluidDensity = 1.225;
        fluidViscosity = 1.81e-5;
    } else {
        std::cout << "Invalid fluid choice." << std::endl;
        return 1;
    }

    double settlingVelocity = calcSettlingVelocity(diameterEntry, densityEntry, fluidDensity, fluidViscosity, 1.0);
    double reynoldsNumber = calcReynoldsNumber(fluidDensity, settlingVelocity, diameterEntry, fluidViscosity);
    double dragFunction = calcDragFunction(reynoldsNumber);
    double cd = calccd(dragFunction, reynoldsNumber);

    double settlingVelocityOld = 1.0;
    double err = 1.0;
    int iterNum = 1;
    const double relaxationFactor = 0.5;

    while (iterNum < 30 && std::abs(err) > 1e-6) {
        settlingVelocityOld = settlingVelocity;
        double settlingVelocityNew = calcSettlingVelocity(diameterEntry, densityEntry, fluidDensity, fluidViscosity, dragFunction);
        settlingVelocity = (relaxationFactor * settlingVelocityOld) + ((1.0 - relaxationFactor) * settlingVelocityNew);

        reynoldsNumber = calcReynoldsNumber(fluidDensity, settlingVelocity, diameterEntry, fluidViscosity);
        dragFunction = calcDragFunction(reynoldsNumber);
        err = settlingVelocity - settlingVelocityOld;

        iterNum++;
    }

    std::cout << "Settling Velocity: " << std::scientific << std::setprecision(2) << settlingVelocity << " m/s\n";
    std::cout << "Reynolds Number: " << std::scientific << std::setprecision(2) << reynoldsNumber << "\n";
    std::cout << "Drag Function: " << dragFunction << "\n";
    std::cout << "Cd: " << std::fixed << std::setprecision(1) << cd << std::endl;

    return 0;
}

