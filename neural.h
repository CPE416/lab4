#ifndef NEURAL_H
#define NEURAL_H

#include <math.h>
#include <stdlib.h>
#include <time.h>


#define NODES 833

#define INPUT_NODES (2)
#define HIDDEN_NODES (NODES)
#define OUTPUT_NODES (NODES)
#define RAND_SEED (time(NULL))

#define INITIAL_WEIGHT ((2.0 *  rand()) / RAND_MAX)
#define INITIAL_BIAS ((2.0 * rand()) / RAND_MAX)

typedef struct {
    int size;
}input_layer_t;

typedef struct {
    int size;
    float bias[HIDDEN_NODES];
    float weights[HIDDEN_NODES][INPUT_NODES];
}hidden_layer_t;

typedef struct {
    int size;
    float bias[OUTPUT_NODES];
    float weights[OUTPUT_NODES][HIDDEN_NODES];
}output_layer_t;

typedef struct {
    float output_node_delta[OUTPUT_NODES];
    output_layer_t new_output_layer;
    hidden_layer_t new_hidden_layer;
}training_data_t;

typedef struct {
    float learning_rate;
    input_layer_t input_layer;
    hidden_layer_t hidden_layer;
    output_layer_t output_layer;
}neural_net_t;

typedef struct {
    float input[INPUT_NODES];
    float hidden[HIDDEN_NODES];
    float output[OUTPUT_NODES];
}net_outputs_t;

void init_net(neural_net_t *net, float learning_rate){
    srand(RAND_SEED);
    net->learning_rate = learning_rate;
    
    net->input_layer.size = INPUT_NODES;
    net->hidden_layer.size = HIDDEN_NODES;
    net->output_layer.size = OUTPUT_NODES;
    
    for(int i = 0; i < HIDDEN_NODES; i++){
        net->hidden_layer.bias[i] = INITIAL_BIAS;
        
        for (int j = 0; j < INPUT_NODES; j++){
            net->hidden_layer.weights[i][j] = INITIAL_WEIGHT;
        }
    }
    for(int i = 0; i < OUTPUT_NODES; i++){
        net->output_layer.bias[i] = INITIAL_BIAS; 

        for (int j = 0; j < HIDDEN_NODES; j++){
            net->output_layer.weights[i][j] = INITIAL_WEIGHT;
        }
    }
}

float sigmoid(float x){
    // http://www.ece.utep.edu/research/webfuzzy/docs/kk-thesis/kk-thesis-html/node72.html
    return 1 / (1 + exp((double) - x));
    // // Accurate approximation of sigmoid
    // return 1/(1 + pow(0.3678749025, x));
    // // Alternate, might be faster on avr
    // return (tanh(x) + 1) / 2;
    // // rectified linear
    // return x * (x > 0);
}

// Infer the output of an input layer
// input should be of size INPUT_NODES
// output should be of size INPUT_NODES
void infer_input_layer(float *input, neural_net_t net, float *output){
    // Input layer loop
    for(int index = 0; index < net.input_layer.size; index++){
        // Does no calculations, only places inputs in output 
        output[index] = input[index];
    }
}

// Infers the output of a hidden layer
void infer_hidden_layer(float *input, neural_net_t net, float *output){
    // Hidden layer loop
    for(int hidden_index = 0; hidden_index < net.hidden_layer.size; hidden_index++){
        float node_output = 0 - net.hidden_layer.bias[hidden_index];
        // Input layer Loop
        for(int input_index = 0; input_index < net.input_layer.size; input_index++){
            float weight = net.hidden_layer.weights[hidden_index][input_index];
            node_output += (input[input_index] * weight);
        }
        output[hidden_index] = sigmoid(node_output);
    }
}

// Infers the output of an output layer
void infer_output_layer(float *input, neural_net_t net, float *output){
    // Output layer loop
    for(int output_index = 0; output_index < net.output_layer.size; output_index++){
        float node_output = 0 - net.output_layer.bias[output_index];
        // Hidden Layer Weight Loop
        for (int hidden_index = 0; hidden_index < net.hidden_layer.size; hidden_index++){
            float weight = net.output_layer.weights[output_index][hidden_index];
            node_output += (input[hidden_index] * weight);
        }
        output[output_index] = sigmoid(node_output);
    }
}

// Setup data for use in inference 
void transform_input(line_data_t line_data, float *input){
    // Put line data between 0.0 and 1.0
    input[0] = ((float) line_data.left) / 255.0;
    input[1] = ((float) line_data.right) / 255.0;
}

// Run inference on a net
void infer_net(line_data_t line_data, neural_net_t net, net_outputs_t *outputs){
    float input[2];
    transform_input(line_data, input);

    infer_input_layer(input, net, outputs->input);
    infer_hidden_layer(outputs->input, net, outputs->hidden);
    infer_output_layer(outputs->hidden, net, outputs->output);
}

// Supplies training data for neural net based on motor commands
void transform_motor_to_target(motor_command_t motors, float *target){
    target[0] = motors.left / 100.0;
    target[1] = motors.right / 100.0;
}

// Translate output to motor commands
motor_command_t transform_output(float output[OUTPUT_NODES]){
    motor_command_t motors;
    motors.left = (u08)(output[0] * 100);
    motors.right = (u08)(output[1] * 100);
    return motors;
}

// Calculate square of average differnce between output and target
float calculate_loss(motor_command_t motors,  float output_layer_output[OUTPUT_NODES]){
    float target[OUTPUT_NODES];
    transform_motor_to_target(motors, target);

    float error = 0;
    for (int i = 0; i < OUTPUT_NODES; i++){
        error += pow2(target[i] - output_layer_output[i]);
    }
    return error * 0.5;
}

void copy_hidden_weights(hidden_layer_t new_layer, neural_net_t *net){
    // Hidden layer loop
    for(int hidden_index = 0; hidden_index < net->hidden_layer.size; hidden_index ++){
        net->hidden_layer.bias[hidden_index] = new_layer.bias[hidden_index];

        // Input layer loop
        for(int input_index = 0; input_index < net->input_layer.size; input_index ++){
            net->hidden_layer.weights[hidden_index][input_index] = new_layer.weights[hidden_index][input_index];
        }
    }
}

void copy_output_weights(output_layer_t new_layer, neural_net_t *net){
    // Output layer loop
    for(int output_index = 0; output_index < net->hidden_layer.size; output_index ++){
        net->output_layer.bias[output_index] = new_layer.bias[output_index];

        // Hidden layer loop
        for(int hidden_index = 0; hidden_index < net->hidden_layer.size; hidden_index ++){
            net->output_layer.weights[output_index][hidden_index] = new_layer.weights[output_index][hidden_index];
        }
    }
}

void train_output_layer(net_outputs_t outputs, neural_net_t net, float *target, training_data_t *training_data){
    // Output layer loop
    for(int output_index = 0; output_index < net.output_layer.size; output_index++){
        float out_min_target = outputs.output[output_index] - target[output_index]; // d Error / d out k
        float out_prime = outputs.output[output_index] * (1 - outputs.output[output_index]); // d out k / d net k
        float node_delta = out_min_target * out_prime; // node delta = dE/doutk * doutk/dnetk
        // Store node delta
        training_data->output_node_delta[output_index] = node_delta;
        // Calculate and store new bias for output node
        float old_bias = net.output_layer.bias[output_index];
        float new_bias = old_bias - (net.learning_rate * node_delta * -1.0);
        training_data->new_output_layer.bias[output_index] = new_bias;
        // Hidden layer loop
        for (int hidden_index = 0; hidden_index < net.hidden_layer.size; hidden_index++){
            float hidden_out = outputs.hidden[hidden_index];
            float old_weight = net.output_layer.weights[output_index][hidden_index];

            float new_weight = old_weight - (net.learning_rate * node_delta * hidden_out);
            training_data->new_output_layer.weights[output_index][hidden_index] = new_weight;
        }
    }
}

void train_hidden_layer(net_outputs_t outputs, neural_net_t net, float *target, training_data_t *training_data){
    // Hidden layer loop
    for(int hidden_index = 0; hidden_index < net.hidden_layer.size; hidden_index++){
        float node_output = outputs.hidden[hidden_index];

        float accumulator = 0;  // Sum of weights coming out
        // Output layer loop
        for (int output_index = 0; output_index < net.output_layer.size; output_index++){
            float out_node_weight = net.output_layer.weights[output_index][hidden_index];
            float out_node_delta = training_data->output_node_delta[output_index];

            accumulator += out_node_weight * out_node_delta;
        }
        float dedoj = accumulator;  //d E d out j = SUM( d E / d out k )[k, l]

        // Drivative of sigmoid hidden node output
        float dodnj = node_output * (1 - node_output);  // d out j / d net j 

        // Calculate and store new bias in training data
        float old_bias = net.hidden_layer.bias[hidden_index];
        float new_bias = old_bias - (net.learning_rate * dedoj * dodnj * -1.0);
        training_data->new_hidden_layer.bias[hidden_index] = new_bias;

        // Input layer loop to calculate hidden weights
        for (int input_index = 0; input_index < net.input_layer.size; input_index++){
            float input = outputs.input[input_index]; 
            float old_weight = net.hidden_layer.weights[hidden_index][input_index];

            float new_weight = old_weight - (net.learning_rate * dedoj * dodnj * input);
            training_data->new_hidden_layer.weights[hidden_index][input_index] = new_weight;
        }
    }
}

// Run a round of training on a neural net based on input data
void train_net(line_data_t line_data, neural_net_t *net, motor_command_t target_motors){
    net_outputs_t net_outputs;
    infer_net(line_data, *net, &net_outputs);
    
    float target[net->output_layer.size];
    transform_motor_to_target(target_motors, target);

    training_data_t training_data;
    
    train_output_layer(net_outputs, *net, target, &training_data);
    train_hidden_layer(net_outputs, *net, target, &training_data);

    copy_hidden_weights(training_data.new_hidden_layer, net);
    copy_output_weights(training_data.new_output_layer, net);

    // ya done now
}

// Runs inference on a neural net to produce motor commands
motor_command_t compute_neural_network(line_data_t line_data, neural_net_t net){
    net_outputs_t outputs;
    // Run inference on the net
    infer_net(line_data, net, &outputs);
    return transform_output(outputs.output);
}

#endif