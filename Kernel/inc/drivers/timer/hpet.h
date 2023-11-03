#ifndef __KERNEL_DRIVERS_TIMER_HPET_H
#define __KERNEL_DRIVERS_TIMER_HPET_H

#include <stdint.h>

typedef union {
  struct {
    uint8_t revID: 8;
    uint8_t numTimCap: 5;
    bool countSizeCap: 1;
    bool reserved: 1;
    bool legacyReplaceCap: 1;
    uint16_t vendorID: 16;
    uint32_t counterClkPeriod: 32;
  };

  uint64_t data;
} hpet_gen_cap_reg_t;

typedef union {
  struct {
    bool enable: 1;        // overall enable
    bool legacyReplace: 1; // legacy replace enable
    uint64_t reserved: 62;
  };

  uint64_t data;
} hpet_gen_conf_reg_t;

typedef union {
  struct {
    bool reserved1: 1;
    bool intType: 1;
    bool intEnable: 1;
    bool type: 1;
    bool perIntCap: 1;
    bool sizeCap: 1;
    bool periodicSet: 1;
    bool reserved2: 1;
    bool mode32: 1;
    uint8_t intRoute: 5;
    bool fsbEn: 1;
    bool fsbIntDelCap: 1;
    uint16_t reserved3: 16;
    uint32_t intRouteCap: 32;
  };

  uint64_t data;
} hpet_timer_conf_reg_t;

void hpet_init();
// sleep for N/100 ns
void hpet_sleep(uint64_t ns);
uint64_t hpet_time();

static inline void udelay(uint64_t us) { hpet_sleep(us * 1000); }

static inline void mdelay(uint64_t ms) { udelay(ms * 1000); }

#endif // __KERNEL_DRIVERS_TIMER_HPET_H
