#ifndef __FPGA_CONFIG_HPP__
#define __FPGA_CONFIG_HPP__

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <unistd.h>

class FpgaConfig {
  private:
    /* data */
  public:
    FpgaConfig();
    ~FpgaConfig();
    static void readFpga(void *varPtr, uint64_t size, uint64_t addr); /* Read from FPGA address space */
    static void writeFpga(void *varPtr, uint64_t size, uint64_t addr); /* Write to FPGA address space */
    static void programFpga(const char *binPtr, uint64_t size, uint64_t addr); /* Write to FPGA ICAP */
    static bool verifyWriteFpga(void *varPtr, uint64_t size, uint64_t addr); /* Write to FPGA and verify by reading back */
};

#endif