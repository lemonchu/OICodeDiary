#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>

struct DataPiece {
    uint8_t pixel[28][28];
    uint8_t label;
};

std::vector<DataPiece> training_set, test_set;

std::random_device rd;
std::mt19937 gen(rd());

double Activate(double x) {
    return 1 / (1 + exp(-x));
    // ReLU
    // return x > 0.0 ? x : 0.0;
}

double Activate_prime(double x) {
    double s = Activate(x);
    return s * (1 - s);
    // ReLU
    // return x > 0.0 ? 1.0 : 0.0;
}

// 用于读取MNIST数据集文件的函数
int ReverseInt(int i) {
    unsigned char c1, c2, c3, c4;
    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;
    return ((int) c1 << 24) + ((int) c2 << 16) + ((int) c3 << 8) + c4;
}

void ReadMNIST(std::vector<DataPiece> &arr, const std::string &images_file_path, const std::string &labels_file_path) {
    std::ifstream images_file(images_file_path, std::ios::binary);
    std::ifstream labels_file(labels_file_path, std::ios::binary);

    if (images_file.is_open() && labels_file.is_open()) {
        int magic_number = 0;
        int number_of_images = 0;
        int n_rows = 0;
        int n_cols = 0;

        // Reading magic number, number of images, rows and columns from images file
        images_file.read(reinterpret_cast<char *>(&magic_number), sizeof(magic_number));
        magic_number = ReverseInt(magic_number);
        images_file.read(reinterpret_cast<char *>(&number_of_images), sizeof(number_of_images));
        number_of_images = ReverseInt(number_of_images);
        arr.resize(number_of_images);

        images_file.read(reinterpret_cast<char *>(&n_rows), sizeof(n_rows));
        n_rows = ReverseInt(n_rows);
        images_file.read(reinterpret_cast<char *>(&n_cols), sizeof(n_cols));
        n_cols = ReverseInt(n_cols);

        // Reading magic number and number of items from labels file
        labels_file.read(reinterpret_cast<char *>(&magic_number), sizeof(magic_number));
        magic_number = ReverseInt(magic_number);
        labels_file.read(reinterpret_cast<char *>(&number_of_images), sizeof(number_of_images));
        number_of_images = ReverseInt(number_of_images);

        for (int i = 0; i < number_of_images; ++i) {
            // Reading image data
            for (int r = 0; r < n_rows; ++r) {
                for (int c = 0; c < n_cols; ++c) {
                    uint8_t temp = 0;
                    images_file.read(reinterpret_cast<char *>(&temp), sizeof(temp));
                    arr[i].pixel[r][c] = temp;
                }
            }

            // Reading label data
            uint8_t temp = 0;
            labels_file.read(reinterpret_cast<char *>(&temp), sizeof(temp));
            arr[i].label = temp;
        }
    } else {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    images_file.close();
    labels_file.close();
}

double edge_0_1[784][16];
double edge_1_2[16][16];
double edge_2_3[16][10];
double bias_1[16];
double bias_2[16];
double bias_3[10];

void init_NR() {
    std::normal_distribution<double> dis(0, 1);
    for (int i = 0; i < 784; ++i) {
        for (int j = 0; j < 16; ++j) {
            edge_0_1[i][j] = dis(gen);
        }
    }
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            edge_1_2[i][j] = dis(gen);
        }
    }
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 10; ++j) {
            edge_2_3[i][j] = dis(gen);
        }
    }
    for (int i = 0; i < 16; ++i) {
        bias_1[i] = dis(gen);
    }
    for (int i = 0; i < 16; ++i) {
        bias_2[i] = dis(gen);
    }
    for (int i = 0; i < 10; ++i) {
        bias_3[i] = dis(gen);
    }
}

void Predict(const uint8_t *image, double *output) {

    // Calculate the output of the first layer
    double pre_sig_layer_1[16] = {0};
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 784; ++j) {
            pre_sig_layer_1[i] += (image[j] * edge_0_1[j][i]) / 256.0;
        }
        pre_sig_layer_1[i] += bias_1[i];
        // layer_1[i] = Activiate(layer_1[i]);
    }
    // Calculate the output of the second layer
    double pre_sig_layer_2[16] = {0};
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            pre_sig_layer_2[i] += Activate(pre_sig_layer_1[j]) * edge_1_2[j][i];
        }
        pre_sig_layer_2[i] += bias_2[i];
        // layer_2[i] = Activiate(layer_2[i]);
    }
    // Calculate the output of the third layer
    for (int i = 0; i < 10; i++) {
        output[i] = 0;
        for (int j = 0; j < 16; j++) {
            output[i] += Activate(pre_sig_layer_2[j]) * edge_2_3[j][i];
        }
        output[i] += bias_3[i];
        output[i] = Activate(output[i]);
    }
}

void Train(const DataPiece &data) {
    auto *image = reinterpret_cast<const uint8_t *>(data.pixel);

    // Calculate the output of the first layer
    double pre_sig_layer_1[16] = {0};
    double layer_1[16] = {0};
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 784; ++j) {
            pre_sig_layer_1[i] += (image[j] * edge_0_1[j][i]) / 256.0;
        }
        pre_sig_layer_1[i] += bias_1[i];
        layer_1[i] = Activate(layer_1[i]);
    }

    // Calculate the output of the second layer
    double pre_sig_layer_2[16] = {0};
    double layer_2[16] = {0};
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            pre_sig_layer_2[i] += Activate(pre_sig_layer_1[j]) * edge_1_2[j][i];
        }
        pre_sig_layer_2[i] += bias_2[i];
        layer_2[i] = Activate(layer_2[i]);
    }

    // Calculate the output of the third layer
    double pre_sig_layer_3[10] = {0};
    double output[10] = {0};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 16; j++) {
            output[i] += Activate(pre_sig_layer_2[j]) * edge_2_3[j][i];
        }
        output[i] += bias_3[i];
        pre_sig_layer_3[i] = output[i];
        output[i] = Activate(output[i]);
    }

    // Calculate the error of the third layer
    // The Loss Function: L = 1/2 * SUM(0, 9, (output[i] - (i == label ? 1 : 0))^2)

    double PC_PA3[10] = {0};
    double PC_PA2[16] = {0};
    double PC_PA1[16] = {0};
    for (int i = 0; i < 10; i++) {
        PC_PA3[i] = output[i] - (i == data.label ? 1 : 0);
    }
    // Calculate PC_PA2
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 10; j++) {
            PC_PA2[i] += PC_PA3[j] * edge_2_3[i][j] * Activate_prime(pre_sig_layer_3[j]);
        }
    }
    // Calculate PC_PA1
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            PC_PA1[i] += PC_PA2[j] * edge_1_2[i][j] * Activate_prime(pre_sig_layer_2[j]);
        }
    }
    // Update the weights and biases
    double learning_rate = 0.05;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 10; j++) {
            edge_2_3[i][j] -= learning_rate * PC_PA3[j] * Activate_prime(pre_sig_layer_3[j]) *
                              Activate(pre_sig_layer_2[i]);
        }
    }
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            edge_1_2[i][j] -= learning_rate * PC_PA2[j] * Activate_prime(pre_sig_layer_2[j]) *
                              Activate(pre_sig_layer_1[i]);
        }
    }
    for (int i = 0; i < 784; i++) {
        for (int j = 0; j < 16; j++) {
            edge_0_1[i][j] -= learning_rate * PC_PA1[j] * Activate_prime(pre_sig_layer_1[j]) * image[i] / 256.0;
        }
    }
    for (int i = 0; i < 10; i++) {
        bias_3[i] -= learning_rate * PC_PA3[i] * Activate_prime(pre_sig_layer_3[i]);
    }
    for (int i = 0; i < 16; i++) {
        bias_2[i] -= learning_rate * PC_PA2[i] * Activate_prime(pre_sig_layer_2[i]);
    }
    for (int i = 0; i < 16; i++) {
        bias_1[i] -= learning_rate * PC_PA1[i] * Activate_prime(pre_sig_layer_1[i]);
    }
}


void SaveModel(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file for saving model: " << filename << std::endl;
        return;
    }

    file.write(reinterpret_cast<char *>(edge_0_1), sizeof(edge_0_1));
    file.write(reinterpret_cast<char *>(edge_1_2), sizeof(edge_1_2));
    file.write(reinterpret_cast<char *>(edge_2_3), sizeof(edge_2_3));
    file.write(reinterpret_cast<char *>(bias_1), sizeof(bias_1));
    file.write(reinterpret_cast<char *>(bias_2), sizeof(bias_2));
    file.write(reinterpret_cast<char *>(bias_3), sizeof(bias_3));

    file.close();
}

void LoadModel(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file for loading model: " << filename << std::endl;
        return;
    }

    file.read(reinterpret_cast<char *>(edge_0_1), sizeof(edge_0_1));
    file.read(reinterpret_cast<char *>(edge_1_2), sizeof(edge_1_2));
    file.read(reinterpret_cast<char *>(edge_2_3), sizeof(edge_2_3));
    file.read(reinterpret_cast<char *>(bias_1), sizeof(bias_1));
    file.read(reinterpret_cast<char *>(bias_2), sizeof(bias_2));
    file.read(reinterpret_cast<char *>(bias_3), sizeof(bias_3));

    file.close();
}

void stratgey(int ch) {
    switch (ch) {
        case 1:
            SaveModel("model.bin");
            break;
        case 2:
            LoadModel("model.bin");
            break;
        default:
            break;
    }
}

void PrintPiece(DataPiece &piece) {
    for (int i = 0; i < 28; ++i) {
        for (int j = 0; j < 28; ++j) {
            std::cout << (piece.pixel[i][j] > 128 ? "1" : " ");
        }
        std::cout << std::endl;
    }
    std::cout << "Label: " << (int) piece.label << std::endl;
}

int main() {

    ReadMNIST(training_set, "train-images-idx3-ubyte", "train-labels-idx1-ubyte");
    std::cout << "Loaded " << training_set.size() << " training images" << std::endl;
    std::shuffle(training_set.begin(), training_set.end(), gen);
    /*
    // Train the model
    init_NR();
    for (int k = 0; k < 6; ++k) {
        for (int i = 0; i < training_set.size(); ++i) {
            Train(training_set[i]);
            if (i % 1000 == 0) {
                std::cout << "Trained " << i << " images" << std::endl;
            }
        }
    }

    double predict[10];
    Predict(reinterpret_cast<uint8_t*>(training_set[0].pixel), predict);
    PrintPiece(training_set[0]);

    for (int i = 0; i < 10; ++i) {
        std::cout << i << ": " << predict[i] << std::endl;
    }
    std::cout << "Label: " << (int) training_set[0].label << std::endl;

    SaveModel("../model.bin");
    */

    LoadModel("model.bin");
    ReadMNIST(test_set, "t10k-images-idx3-ubyte", "t10k-labels-idx1-ubyte");
    std::shuffle(test_set.begin(), test_set.end(), gen);

    double predict_test[10];
    for (int i = 0; i < 10; ++i) {
        Predict(reinterpret_cast<uint8_t *>(test_set[i].pixel), predict_test);
        PrintPiece(test_set[i]);
        for (int j = 0; j < 10; ++j) {
            std::cout << j << ": " << predict_test[j] << std::endl;
        }
        std::cout << "Label: " << (int) test_set[i].label << std::endl;
    }

    // Get the accuracy of the model
    int correct = 0;
    for (int i = 0; i < test_set.size(); ++i) {
        double predict[10];
        Predict(reinterpret_cast<uint8_t *>(test_set[i].pixel), predict);
        int max_index = 0;
        for (int j = 1; j < 10; ++j) {
            if (predict[j] > predict[max_index]) {
                max_index = j;
            }
        }
        if (max_index == test_set[i].label) {
            correct++;
        }
    }
    std::cout << "Accuracy: " << (double) correct / test_set.size() << std::endl;

    return 0;
}