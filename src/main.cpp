#include <bits/stdint-uintn.h>
#include <iostream>
#include <unistd.h>

#include "fpga/FpgaConfig.hpp"
#include "gten/gten_types.h"

static uint64_t S_AXILITE_ADDR          = 0x80000000;
static size_t INPUT_CTRL_OFFSET         = 0x10;
static size_t FC1_BIAS_CTRL_OFFSET      = 0x1c;
static size_t FC1_WEIGHT_CTRL_OFFSET    = 0x28;
static size_t FC2_BIAS_CTRL_OFFSET      = 0x34;
static size_t FC2_WEIGHT_CTRL_OFFSET    = 0x40;
static size_t OUTPUT_CTRL_OFFSET        = 0x4c;

static uint64_t INPUT_ADDR              = 0x00000000;
static size_t   INPUT_SIZE              = 1*64*768;
static uint64_t FC1_BIAS_ADDR           = 0x10000000;
static size_t   FC1_BIAS_SIZE           = 3072;
static uint64_t FC1_WEIGHT_ADDR         = 0x20000000;
static size_t   FC1_WEIGHT_SIZE         = 3072*768;
static uint64_t FC2_BIAS_ADDR           = 0x30000000;
static size_t   FC2_BIAS_SIZE           = 768;
static uint64_t FC2_WEIGHT_ADDR         = 0x40000000;
static size_t   FC2_WEIGHT_SIZE         = 768*3072;
static uint64_t OUTPUT_ADDR             = 0x50000000;
static size_t   OUTPUT_SIZE             = 1*64*768;

using float16_t = gten::Float16;

int main(int argc, char const **argv) {
    float v = 1.0;
    float16_t init_value = gten::fp32_to_fp16(v);
    
    auto input_token = new float16_t[1][64][768];
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 768; j++) {
            input_token[0][i][j] = init_value;
        }
    }

    auto fc1_bias = new float16_t[3072];
    for (int i = 0; i < 3072; i++) {
        fc1_bias[i] = init_value;
    }

    auto fc1_weight = new float16_t[3072][768];
    for (int i = 0; i < 3072; i++) {
        for (int j = 0; j < 768; j++) {
            fc1_weight[i][j] = init_value;
        }
    }

    auto fc2_bias = new float16_t[768];
    for (int i = 0; i < 768; i++) {
        fc2_bias[i] = init_value;
    }

    auto fc2_weight = new float16_t[768][3072];
    for (int i = 0; i < 768; i++) {
        for (int j = 0; j < 3072; j++) {
            fc2_weight[i][j] = init_value;
        }
    }

    auto output_token = new float16_t[1][64][768];
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 768; j++) {
            output_token[0][i][j] = init_value;
        }
    }

    std::cout << "[INFO] Writing data to FPGA" << std::endl;
    // write data
    std::cout << "[0/5] writing input_token, size: " << INPUT_SIZE << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(input_token), INPUT_SIZE, INPUT_ADDR);
    std::cout << "[1/5] writing fc1_bias, size: " << FC1_BIAS_SIZE << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(fc1_bias), FC1_BIAS_SIZE, FC1_BIAS_ADDR);
    std::cout << "[2/5] writing fc1_weight, size: " << FC1_WEIGHT_SIZE << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(fc1_weight), FC1_WEIGHT_SIZE, FC1_WEIGHT_ADDR);
    std::cout << "[3/5] writing fc2_bias, size: " << FC2_BIAS_SIZE << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(fc2_bias), FC2_BIAS_SIZE, FC2_BIAS_ADDR);
    std::cout << "[4/5] writing fc2_weight, size: " << FC2_WEIGHT_SIZE << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(fc2_weight), FC2_WEIGHT_SIZE, FC2_WEIGHT_ADDR);
    std::cout << "[5/5] All data written to FPGA." << std::endl;
    
    std::cout << "[INFO] Writing data address to FPGA" << std::endl;
    // write data address
    std::cout << "[0/6] writing INPUT_ADDR: " << std::hex << INPUT_ADDR << std::dec << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(&INPUT_ADDR),      8, S_AXILITE_ADDR + INPUT_CTRL_OFFSET);
    std::cout << "[1/6] writing FC1_BIAS_ADDR: " << std::hex << FC1_BIAS_ADDR << std::dec << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(&FC1_BIAS_ADDR),   8, S_AXILITE_ADDR + FC1_BIAS_CTRL_OFFSET);
    std::cout << "[2/6] writing FC1_WEIGHT_ADDR: " << std::hex << FC1_WEIGHT_ADDR << std::dec << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(&FC1_WEIGHT_ADDR), 8, S_AXILITE_ADDR + FC1_WEIGHT_CTRL_OFFSET);
    std::cout << "[3/6] writing FC2_BIAS_ADDR: " << std::hex << FC2_BIAS_ADDR << std::dec << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(&FC2_BIAS_ADDR),   8, S_AXILITE_ADDR + FC2_BIAS_CTRL_OFFSET);
    std::cout << "[4/6] writing FC2_WEIGHT_ADDR: " << std::hex << FC2_WEIGHT_ADDR << std::dec << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(&FC2_WEIGHT_ADDR), 8, S_AXILITE_ADDR + FC2_WEIGHT_CTRL_OFFSET);
    std::cout << "[5/6] writing OUTPUT_ADDR: " << std::hex << OUTPUT_ADDR << std::dec << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(&OUTPUT_ADDR),     8, S_AXILITE_ADDR + OUTPUT_CTRL_OFFSET);

    std::cout << "[INFO] Starting IP" << std::endl;
    // start ip
    uint32_t ap_ctrl_reg;
    FpgaConfig::readFpga(reinterpret_cast<void*>(&ap_ctrl_reg), 4, S_AXILITE_ADDR);
    ap_ctrl_reg = ap_ctrl_reg & 0x80;
    ap_ctrl_reg = ap_ctrl_reg | 0x01;
    std:: cout << std::hex << ap_ctrl_reg << std::endl;
    FpgaConfig::writeFpga(reinterpret_cast<void*>(&ap_ctrl_reg), 4, S_AXILITE_ADDR);
    std:: cout << std::hex << ap_ctrl_reg << std::endl;

    std::cout << "[INFO] Waiting for IP to finish" << std::endl;
    // wait for finish
    for (int i = 0; i < 100; i++) {
        FpgaConfig::readFpga(reinterpret_cast<void*>(&ap_ctrl_reg), 4, S_AXILITE_ADDR);
        uint32_t ap_ctrl_isready = (ap_ctrl_reg >> 3) & 0x1;
        if (ap_ctrl_isready)
            break;
        sleep(1);
        std:: cout << std::dec<< i << ": " << std::hex << ap_ctrl_reg << std::endl;
    }
    
    std::cout << "[INFO] Reading results from FPGA" << std::endl;
    // read results
    FpgaConfig::readFpga(reinterpret_cast<void*>(output_token), OUTPUT_SIZE, OUTPUT_ADDR);

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 768; j++) {
            std::cout << gten::fp16_to_fp32(output_token[0][i][j]) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
