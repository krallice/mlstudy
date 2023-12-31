#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "perceptron.h"
#include "mlp.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"

// Structure to map model names to functions
typedef struct {
    char *modelName;
    char *description;
    void (*function)();
} ModelMapping;

void model_x_gt_9(void) {

    // Modelling x > 9:
    const double training_features[][1] = {
        {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9},
        {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17}, {18}, {19}
    };
    const double training_labels[] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1
        };
    int training_rows = sizeof(training_features) / sizeof(training_features[0]);
    int training_columns = sizeof(training_features[0]) / sizeof(training_features[0][0]);

    perceptron_t *p = init_perceptron(1, sign_activation_function, 1000);

    printf("\n");

    printf("[ %sDETAILS%s ]\n", YELLOW, RESET);
    printf("Model: model_x_gt_9\n");
    printf("Aim: Train a neuron to fire when input vector x_1 > 9\n");
    printf("Architecture: Single Perceptron\n");
    printf("Input: A one dimensional input vector, x\n");
    printf("\t- x_1: Input value\n");
    printf("Activation: Sign Activation Function\n");
    printf("Loss Function: Perceptron Learning Rule\n\t(weight = weight + (learning_rate)(error := correct - predicted)(input))\n");

    printf("Training Strategy:\n");
    printf("\t1000 epochs of correctly labeled integers 0 -> 19 inclusive. No rational/fractional numbers, integers only.\n");
    printf("\tExpected outcome is a normal vector that defines a hyperplane that seperates a two dimensional vector space based on the x value being < 9.\n");
    
    printf("\n\n");

    printf("[ %sTRAINING%s ]\n", YELLOW, RESET);
    printf("Model execution starting now ...\n");
    printf("Training 1000 epochs now.\n");

    train_perceptron(p, training_rows, training_columns, training_features, training_labels, 0.1);

    printf("Training complete.\n");

    printf("\n\n");

    printf("[ %sTRAINING RESULTS%s ]\n", YELLOW, RESET);
    printf("Final weight vector w in R2 (w_0, w_1) = (%f, %f)\n", p->weights[0], p->bias_weight);
    printf("Vector w is a normal vector to a hyperplane/vector subspace defined by: %.2fx + %.2fy = 0\n", p->weights[0], p->bias_weight);
    printf("The hyperplane crosses y = 1 at x = %.2f\n", ((- p->bias_weight) / p->weights[0]));
    printf("\t(y = 1 is important as all input vectors sit at (x=x,y=1) due to the bias term lifting the one dimensional input vectors consisting of x_0 into R2)\n");
    printf("The perceptron will only fire for values that are >= %.2f\n", ((- p->bias_weight) / p->weights[0]));

    printf("\n\n");

    printf("[ %sPREDICTION%s ]\n", YELLOW, RESET);
    printf("Starting prediction test from x = -15 to 15\n");

    // Predict and check:
    int c = 1;
    for (int i = -15; i <= 14; i++) {
        const double prediction_features[] = {i};
        double prediction = perceptron_feedforward(p, prediction_features);
        double expected_value = i > 9 ? 1 : -1;
        //printf("[ %02d/30 %s ]: Input:%d Expected:%0.0f Prediction:%0.0f\n", c++, expected_value == prediction ? "SUCCESS" : "FAILURE", i, expected_value, prediction);
        //printf("STATUS: %s Feature: %d Prediction: %f\n", expected_value == prediction ? "SUCCESS" : "FAILURE", i, prediction);
        printf("[ %s%02d/30 %s%s ]: Input: %3d Expected: %2.0f Prediction: %2.0f\n", 
            expected_value == prediction ? GREEN : RED, c++, expected_value == prediction ? "SUCCESS" : "FAILURE", RESET, 
            i, expected_value, prediction);
    }

    destroy_perceptron(p);
    return;
}

void model_linear(void) {
    
    const int feature_count = 50;
    const int point_lower_bound = -20;
    const int point_upper_bound = 20;

    double training_features[feature_count][2];
    double training_labels[feature_count];

    printf("\n");
    printf("[ %sDETAILS%s ]\n", YELLOW, RESET);
    printf("Model: model_linear\n");
    printf("Aim: Train a neuron to linearly separate input vector x in R2 against equation: y = x/2 + 5\n");
    printf("Architecture: Single Perceptron\n");
    printf("Input: A two dimensional input vector, x\n");
    printf("\t- x_1: Input value, mapped conceptually to the x axis\n");
    printf("\t- x_2: Input value, mapped conceptually to the y axis\n");
    printf("Activation: Sign Activation Function\n");
    printf("Loss Function: Perceptron Learning Rule\n\t(weight = weight + (learning_rate)(error := correct - predicted)(input))\n");

    printf("Training Strategy:\n");
    printf("\tTraining data randomly generated of %d entries, with domain: (%d <= x <= %d) and range (%d <= x <= %d)\n", 
        feature_count, point_lower_bound, point_upper_bound, point_lower_bound, point_upper_bound);
    printf("\tPerceptron trained with 100 epochs of the dataset.\n");
    
    printf("\n\n");

    printf("[ %sGENERATE TRAINING DATA%s ]\n", YELLOW, RESET);
    for (int i = 0; i < feature_count; i++) {

        // x co-ord:
        training_features[i][0] = point_lower_bound + rand() % (point_upper_bound - point_lower_bound + 1);
        // y co-ord:
        training_features[i][1] = point_lower_bound + rand() % (point_upper_bound - point_lower_bound + 1);

        // Model linear seperation using
        // y = x/2 + 5, if we are above the line, fire
        int y = (training_features[i][0] / 2) + 5;
        training_labels[i] = (training_features[i][1] >= y) ? 1 : -1;
    
        printf("Generated Random Point (%d/%d): (x,y): (%d, %d). Point is %s the line.\n", i + 1, feature_count, 
            (int)training_features[i][0], (int)training_features[i][1],
            training_labels[i] == 1 ? "above" : "below");
    }

    printf("\n\n");
    printf("[ %sTRAINING%s ]\n", YELLOW, RESET);
    printf("Model execution starting now ...\n");
    printf("Training 100 epochs now.\n");
    perceptron_t *p = init_perceptron(2, sign_activation_function, 100);
    train_perceptron(p, feature_count, 2, training_features, training_labels, 0.1);

    printf("\n\n");
    printf("[ %sTRAINING RESULTS%s ]\n", YELLOW, RESET);
    printf("Final weight vector w in R2 (w_0, w_1) = (%0.2f, %0.2f)\n", p->weights[0], p->weights[1]);
    printf("Final bias value b = %0.2f\n", p->bias_weight);
    printf("This defines an equation: %.2fx + %.2fy + %.2f = 0\n", p->weights[0], p->weights[1], p->bias_weight);
    printf("Re-arranged for y: y = %.2fx + %.2f\n", ((- p->weights[0])/ p->weights[1]), ((- p->bias_weight)/ p->weights[1]));

    printf("\n\n");
    printf("[ %sPREDICTION%s ]\n", YELLOW, RESET);
    const int predict_count = 20;
    for (int i = 1; i <= predict_count; i++) {

        double predict_features[2];
        int predict_feature_label;

        // x co-ord:
        predict_features[0] = point_lower_bound + rand() % (point_upper_bound - point_lower_bound + 1);
        // y co-ord:
        predict_features[1] = point_lower_bound + rand() % (point_upper_bound - point_lower_bound + 1);

        // Generate result:
        int y = (predict_features[0] / 2) + 5;
        predict_feature_label = (predict_features[1] >= y) ? 1 : -1;

        double prediction = perceptron_feedforward(p, predict_features);

        printf("[ %s%02d/%d %s%s ]: Input: (%3d,%3d) Expected: %2d (Point is %s the line) Prediction: %2.0f\n", 
            (predict_feature_label == prediction) ? GREEN : RED,
            i, predict_count, 
            (predict_feature_label == prediction) ? "SUCCESS" : "ERROR  ", RESET,
            (int)predict_features[0], (int)predict_features[1], predict_feature_label, 
            (predict_feature_label == 1) ? "above" : "below", prediction);
    }

    destroy_perceptron(p);
    return;
}

void model_AND(void) {

    // Modelling logical AND:
    const double training_features[][2] = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}
    };
    const double training_labels[] = {
        -1, -1, -1, 1
    };
    int training_rows = sizeof(training_features) / sizeof(training_features[0]);
    int training_columns = sizeof(training_features[0]) / sizeof(training_features[0][0]);

    printf("\n");
    printf("[ %sDETAILS%s ]\n", YELLOW, RESET);
    printf("Model: model_AND\n");
    printf("Aim: Train a neuron to model an AND gate, acting against an input vector x in R2\n");
    printf("Architecture: Single Perceptron\n");
    printf("Input: A two dimensional input vector, x\n");
    printf("\t- x_1: Input value, mapped conceptually to the x axis\n");
    printf("\t- x_2: Input value, mapped conceptually to the y axis\n");
    printf("Activation: Sign Activation Function\n");
    printf("Loss Function: Perceptron Learning Rule\n\t(weight = weight + (learning_rate)(error := correct - predicted)(input))\n");

    printf("Training Strategy:\n");
    printf("\tPerceptron trained with 100 epochs of the entire truth table of an AND gate.\n");
    
    perceptron_t *p = init_perceptron(training_columns, sign_activation_function, 100);

    printf("\n\n");
    printf("[ %sTRAINING%s ]\n", YELLOW, RESET);
    printf("Model execution starting now ...\n");
    printf("Training 100 epochs now.\n");

    train_perceptron(p, training_rows, training_columns, training_features, training_labels, 0.1);

    printf("\n\n");
    printf("[ %sTRAINING RESULTS%s ]\n", YELLOW, RESET);
    printf("Final weight vector w in R2 (w_0, w_1) = (%0.2f, %0.2f)\n", p->weights[0], p->weights[1]);
    printf("Final bias value b = %0.2f\n", p->bias_weight);
    printf("This defines an equation: %.2fx + %.2fy + %.2f = 0\n", p->weights[0], p->weights[1], p->bias_weight);
    printf("Re-arranged for y: y = %.2fx + %.2f\n", ((- p->weights[0])/ p->weights[1]), ((- p->bias_weight)/ p->weights[1]));
    printf("\n\n");

    printf("[ %sPREDICTION%s ]\n", YELLOW, RESET);
    for (int i = 0; i < training_rows; i++) {

        double correct_result = (double)((int)training_features[i][0] & (int)training_features[i][1]) == 0 ? -1 : 1;
        double prediction = perceptron_feedforward(p, training_features[i]);

        printf("[ %s%02d/04 %s%s ]: Input: (%2.f, %2.f) Expected: %2.f Prediction: %2.f\n", 
            (correct_result == prediction) ? GREEN : RED,
            i + 1, (correct_result == prediction) ? "SUCCESS" : "ERROR  ", RESET,
            training_features[i][0], training_features[i][1], correct_result, prediction);
    }

    destroy_perceptron(p);
    return;
}

void model_XOR(void) {

    // Modelling logical XOR:
    const double training_features[][2] = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}
    };
    const double training_labels[] = {
        -1, 1, 1, -1
    };
    int training_rows = sizeof(training_features) / sizeof(training_features[0]);
    int training_columns = sizeof(training_features[0]) / sizeof(training_features[0][0]);

    multilayer_perceptron_t *mlp = init_mlp(2, 2, 2, 100);

    train_mlp(mlp, training_rows, training_columns, training_features, training_labels, 0.1);

    destroy_mlp(mlp);
}

// Array of model mappings
ModelMapping modelMappings[] = {
    {"model_x_gt_9", "A single dimensional input to a single perceptron, trained on the dataset of x > 9", model_x_gt_9},
    {"model_linear", "A two dimensional input perceptron, trained to model y = x/2 + 5", model_linear},
    {"model_AND", "A two dimensional input perceptron, trained to operate as an AND gate", model_AND},
    {"model_XOR", "A multi-layer perceptron, modelling the XOR function", model_XOR}
};

int main(int argc, char *argv[]) {

    // Random Seed:
    srand(time(NULL)); 

    if (argc < 2) {
        printf("Usage: %s <modelName>\n", argv[0]);
        printf("Valid models:\n");
        for (int i = 0; i < sizeof(modelMappings) / sizeof(modelMappings[0]); i++) {
            printf("- %s - %s\n", modelMappings[i].modelName, modelMappings[i].description);
        }
        return 1;
    }

    char *modelName = argv[1];
    void (*selectedFunction)() = NULL;
    for (int i = 0; i < sizeof(modelMappings) / sizeof(modelMappings[0]); i++) {
        if (strcmp(modelName, modelMappings[i].modelName) == 0) {
            selectedFunction = modelMappings[i].function;
            break;
        }
    }

    if (selectedFunction != NULL) {
        selectedFunction();
    } else {
        printf("Invalid model name: %s\n", modelName);
        printf("Valid models:\n");
        for (int i = 0; i < sizeof(modelMappings) / sizeof(modelMappings[0]); i++) {
            printf("- %s - %s\n", modelMappings[i].modelName, modelMappings[i].description);
        }
        return 1;
    }
    return 0;
}
