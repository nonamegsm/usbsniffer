#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "FTD3XX.h"
#include "ft601.h"

#include "common/testsuite.h"

void cdelay(int val)
{
    usleep(val);
}

static FT_HANDLE _gfd;

extern uint32_t eb_read_reg32(int fd, uint32_t addr);
extern void eb_write_reg32(int fd, uint32_t addr, uint32_t val);

void csr_writel(uint32_t value, uint32_t addr)
{
    eb_write_reg32(_gfd, addr, value);
}

uint32_t csr_readl(uint32_t addr)
{
    return eb_read_reg32(_gfd, addr);
}

int main(int argc, char **argv)
{
    struct event_base *base;
    int i;
    int ret;

#ifdef WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

    printf("USBSniffer Hardware Testsuite\n\n");

    ret = FT601_Open(&_gfd);
    if (!ret) {
        printf("Open failed: device not found\n");
        return ret;
    }

    /* Check BUS  */
    check_soc_identifier(_gfd);

    /* Check both ULPI chips */
    for (i=0; i<2; i++) {
        check_ulpi_scratch(_gfd, i);
    }

    /* Check SDRAM */
    check_sdram(_gfd);

    /* Check LEDs */
    for (i=0; i<2; i++) {
        check_leds(_gfd, i);
    }

    FT601_Close(_gfd);
    return 0;
}
