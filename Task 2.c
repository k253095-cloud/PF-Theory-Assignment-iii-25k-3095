#include <stdio.h>

// Recursive function to calculate fuel
void calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets) {
    if (fuel <= 0) { // Base case: fuel exhausted
        printf("Planet %d: Fuel Remaining = 0\n", planet);
        printf("The spacecraft cannot complete its journey.\n");
        return;
    }
    if (planet > totalPlanets) { // Base case: all planets visited
        printf("The spacecraft has successfully completed its journey!\n");
        return;
    }

    // Fuel consumed at this planet
    fuel -= consumption;

    // Regeneration due to gravitational assist
    fuel += recharge;

    // Solar recharge every 4th planet
    if (planet % 4 == 0) {
        fuel += solarBonus;
    }

    if (fuel < 0) fuel = 0; // Ensure fuel doesn't go negative

    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);

    // Recursive call for next planet
    calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);
}

int main() {
    int initialFuel, consumption, recharge, solarBonus, totalPlanets;

    // User inputs
    printf("Enter initial fuel: ");
    scanf("%d", &initialFuel);

    printf("Enter fuel consumption per planet: ");
    scanf("%d", &consumption);

    printf("Enter fuel regained from gravitational assist: ");
    scanf("%d", &recharge);

    printf("Enter solar recharge bonus (every 4th planet): ");
    scanf("%d", &solarBonus);

    printf("Enter total number of planets: ");
    scanf("%d", &totalPlanets);

    // Start recursion from planet 1
    calculateFuel(initialFuel, consumption, recharge, solarBonus, 1, totalPlanets);

    return 0;
}

