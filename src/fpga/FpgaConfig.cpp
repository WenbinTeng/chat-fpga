#include "FpgaConfig.hpp"
#include <cassert>
#include <iostream>
#include <unistd.h>

FpgaConfig::FpgaConfig() {}

FpgaConfig::~FpgaConfig() {}

constexpr size_t RW_MAX_SIZE = 0x7ffff000;
const char *C2H[4] = {"/dev/xdma0_c2h_0","/dev/xdma0_c2h_1","/dev/xdma0_c2h_2","/dev/xdma0_c2h_3"};
const char *H2C[4] = {"/dev/xdma0_h2c_0","/dev/xdma0_h2c_1","/dev/xdma0_h2c_2","/dev/xdma0_h2c_3"};

int dmaRead(int fd, char *buffer, uint64_t size, uint64_t addr) {
    if (addr != lseek(fd, addr, SEEK_SET))
        return -1;
    if (size != read(fd, buffer, size))
        return -1;
    return 0;
}

int dmaWrite(int fd, char *buffer, uint64_t size, uint64_t addr) {
    std::cout << std::hex  << "addr: " << addr << std::dec << " size: " << size << std::endl;
    if (addr != lseek(fd, addr, SEEK_SET))
        return -1;
    if (size != write(fd, buffer, size))
        return -1;
    return 0;
}

void FpgaConfig::readFpga(void *varPtr, uint64_t size, uint64_t addr) {
    int fpgaFd = -1;
    char *buffer = nullptr;
    size_t pageSize = sysconf(_SC_PAGESIZE);

    fpgaFd = open(C2H[0], O_RDWR);
    assert(fpgaFd >= 0);

    posix_memalign(reinterpret_cast<void **>(&buffer), pageSize, size + pageSize);
    assert(buffer != nullptr);

    int r = dmaRead(fpgaFd, buffer, size, addr);
    assert(r == 0);
    memcpy(varPtr, buffer, size);
    close(fpgaFd);
    free(buffer);
}

void FpgaConfig::writeFpga(void *varPtr, uint64_t size, uint64_t addr) {
    int fpgaFd = -1;
    char *buffer = nullptr;
    size_t pageSize = sysconf(_SC_PAGESIZE);

    fpgaFd = open(H2C[0], O_RDWR);
    assert(fpgaFd >= 0);

    posix_memalign(reinterpret_cast<void **>(&buffer), pageSize, size + pageSize);
    assert(buffer != nullptr);

    memcpy(buffer, varPtr, size);
    int r = dmaWrite(fpgaFd, buffer, size, addr);
    assert(r == 0);
    close(fpgaFd);
    free(buffer);
}

void FpgaConfig::programFpga(const char *binPtr, uint64_t size, uint64_t addr) {
    int binFd = -1;
    int fpgaFd = -1;
    char *buffer = nullptr;
    size_t pageSize = sysconf(_SC_PAGESIZE);

    binFd = open(binPtr, O_RDONLY);
    assert(binFd >= 0);

    fpgaFd = open(H2C[1], O_RDWR);
    assert(fpgaFd >= 0);

    posix_memalign(reinterpret_cast<void **>(&buffer), pageSize, size + pageSize);
    assert(buffer != nullptr);

    dmaRead(binFd, buffer, size, 0);
    dmaWrite(fpgaFd, buffer, size, addr);
    close(fpgaFd);
    close(binFd);
    free(buffer);
}