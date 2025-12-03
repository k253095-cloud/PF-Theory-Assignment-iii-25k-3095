#include <stdio.h>

double calculateRepayment(double loan, double interestRate, double installment, int year, int currentYear) {
    if(year == 0 || loan <= 0) return 0;

    loan = loan * (1 + interestRate / 100.0);
    printf("Year %d: Remaining loan before payment = %.2f\n", currentYear, loan);

    loan -= installment;
    if(loan < 0) loan = 0;

    return installment + calculateRepayment(loan, interestRate, installment, year-1, currentYear+1);
}

int main() {
    double loan, interestRate, installment;
    int years;

    // User input
    printf("Enter loan amount: ");
    scanf("%lf", &loan);

    printf("Enter yearly interest rate (%%): ");
    scanf("%lf", &interestRate);

    printf("Enter yearly installment: ");
    scanf("%lf", &installment);

    printf("Enter number of years: ");
    scanf("%d", &years);

    printf("\nRepayment Schedule:\n");
    double totalPaid = calculateRepayment(loan, interestRate, installment, years, 1);

    printf("\nTotal repayment over %d years = %.2f\n", years, totalPaid);
    return 0;
}

