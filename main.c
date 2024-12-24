#include <stdio.h>

void calculateLength(double c1, double dev_c1_max, double dev_c1_min, int num_reducing_dimensions, double reducing_dims[], double dev_reducing_dims[]) {
    double sum_reducing_dims = 0;
    double min_dev_reducing_dims = 0;
    double max_dev_reducing_dims = 0;
    
    // Calculate sum of reducing dimensions and min/max deviations
    for (int i = 0; i < num_reducing_dimensions; i++) {
        sum_reducing_dims += reducing_dims[i];
        min_dev_reducing_dims += dev_reducing_dims[i * 2 + 1];  // abaterile inferioare
        max_dev_reducing_dims += dev_reducing_dims[i * 2];  // abaterile superioare
    }

    // valoare nominala a cotei dorite
    double L_nominal = c1 - sum_reducing_dims;
    
    // valoarea maxima
    double L_max = (c1 + dev_c1_max) - (sum_reducing_dims + min_dev_reducing_dims);
    
    // valoarea minima
    double L_min = (c1 - dev_c1_min) - (sum_reducing_dims + max_dev_reducing_dims);

    // abaterile
    double max_deviation = L_max - L_nominal;
    double min_deviation = L_min - L_nominal;

    printf("\nValoare nominala (L) = %.2f mm\n", L_nominal);
    printf("Abatere superioara = %.2f mm\n", max_deviation);
    printf("Abatere inferioara = %.2f mm\n", min_deviation);
}

int main() {
    double c1, dev_c1_max, dev_c1_min;
    int num_reducing_dimensions;

    printf("Dimensiunea cotei maritoare: ");
    scanf("%lf", &c1);
    printf("Abaterea superioara: ");
    scanf("%lf", &dev_c1_max);
    printf("Abaterea inferioara: ");
    scanf("%lf", &dev_c1_min);

    printf("Cate cote micsoratoare sunt: ");
    scanf("%d", &num_reducing_dimensions);

    // Array pentru cotele micoratoare si abaterile lor
    double reducing_dims[num_reducing_dimensions];
    double dev_reducing_dims[num_reducing_dimensions * 2];  // fiecare valoare are o abatere maxima si una minima

    for (int i = 0; i < num_reducing_dimensions; i++) {
        printf("Introdu dimensiunea cotei miscoratoare %d: ", i + 1);
        scanf("%lf", &reducing_dims[i]);
        printf("Abaterea sa maxima %d: ", i + 1);
        scanf("%lf", &dev_reducing_dims[i * 2]);     
        printf("Abaterea sa minima %d: ", i + 1);
        scanf("%lf", &dev_reducing_dims[i * 2 + 1]); 
    }

    calculateLength(c1, dev_c1_max, dev_c1_min, num_reducing_dimensions, reducing_dims, dev_reducing_dims);

    return 0;
}