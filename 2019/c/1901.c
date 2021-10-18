#include <stdio.h>
#include <stdlib.h>

#define MAXBUFF 16

#define CALC_FUEL(val) val / 3 - 2

int32_t fuel_for_fuel(int32_t mass)
{
    int32_t fuel = CALC_FUEL(mass);
    return fuel > 0 ? fuel + fuel_for_fuel(fuel) : 0;
}

int main(void)
{
    printf("Advent of Code 2019 - Day 1\n");
    printf("The Tyranny of the Rocket Equation\n");

	int32_t total_fuel = 0;
	int32_t fuel_fuel = 0;

    char buff[MAXBUFF];
    while (fgets(buff, MAXBUFF, stdin)) {
        int32_t mass = atoi(buff);
        total_fuel += CALC_FUEL(mass);
        fuel_fuel += fuel_for_fuel(mass);
    }


	printf("Part 1: %d\n", total_fuel);
	printf("Part 2: %d\n", fuel_fuel);
}
