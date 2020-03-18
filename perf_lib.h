#ifndef PERF_LIB_H
#define PERF_LIB_H

#include <stdio.h>

// Performance Monitors Cycle Count Register
static inline uint32_t
rdtsc64(void)
{
#if defined(__GNUC__)
        uint64_t r = 0;
#if  defined __aarch64__     
	asm volatile("mrs %0, pmccntr_el0" : "=r" (r)); 
#elif defined(__ARM_ARCH_7A__)
        asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
#else
#error Unsupported architecture/compiler!
#endif
        return r;
#endif
}

static inline void
reset_counter(void) {
#if defined(__GNUC__)
        uint32_t r = 0;
#if  defined __aarch64__     
	asm volatile("mrs %0, PMCR_EL0	" : "=r" (r)); 
	asm volatile("msr PMCR_EL0, %0" : : "r" (r|6));
#else
#error Unsupported architecture/compiler!
#endif
#endif	
}

#define ARMV8_PMEVTYPER_P              (1 << 31) /* EL1 modes filtering bit */
#define ARMV8_PMEVTYPER_U              (1 << 30) /* EL0 filtering bit */
#define ARMV8_PMEVTYPER_NSK            (1 << 29) /* Non-secure EL1 (kernel) modes filtering bit */
#define ARMV8_PMEVTYPER_NSU            (1 << 28) /* Non-secure User mode filtering bit */
#define ARMV8_PMEVTYPER_NSH            (1 << 27) /* Non-secure Hyp modes filtering bit */
#define ARMV8_PMEVTYPER_M              (1 << 26) /* Secure EL3 filtering bit */
#define ARMV8_PMEVTYPER_MT             (1 << 25) /* Multithreading */
#define ARMV8_PMEVTYPER_EVTCOUNT_MASK  0x3ff

// Table 11.5. Common Event Identification Register 0 bit assignments

// Bit	Name	Event number	Value	Event implemented if bit set to 1 or not implemented if bit set to 0
// [31]	-		0x1F	0	Reserved, res0.
// [30]	CH		0x1E	1	Chain.[a] An odd-numbered counter increments when an overflow occurs on the preceding even-numbered counter. For even-numbered counters, does not count.
// [29]	BC		0x1D	1	Bus cycle.
// [28]	TW		0x1C	1	TTBR write, architecturally executed, condition check pass - write to translation table base.
// [27]	IS		0x1B	1	Instruction speculatively executed.
// [26]	ME		0x1A	1	Local memory error.
// [25]	BA		0x19	1	Bus access.
// [24]	DC2W	0x18	1	Level 2 data cache Write-Back.
// [23]	DC2R	0x17	1	Level 2 data cache refill.
// [22]	DC2A	0x16	1	Level 2 data cache access.
// [21]	DC1W	0x15	1	Level 1 data cache Write-Back.
// [20]	IC1A	0x14	1	Level 1 instruction cache access.
// [19]	MA		0x13	1	Data memory access.
// [18]	BP		0x12	1	Predictable branch speculatively executed.
// [17]	CC		0x11	1	Cycle.
// [16]	BM		0x10	1	Mispredicted or not predicted branch speculatively executed.
// [15]	UL		0x0F	0	Instruction architecturally executed, condition check pass - unaligned load or store.
// [14]	BR		0x0E	0	Instruction architecturally executed, condition check pass - procedure return.
// [13]	BI		0x0D	0	Instruction architecturally executed - immediate branch.
// [12]	PW		0x0C	0	Instruction architecturally executed, condition check pass - software change of the PC.
// [11]	CW		0x0B	1	Instruction architecturally executed, condition check pass - write to CONTEXTIDR.
// [10]	ER		0x0A	1	Instruction architecturally executed, condition check pass - exception return.
// [9]	ET		0x09	1	Exception taken.
// [8]	IA		0x08	1	Instruction architecturally executed.
// [7]	ST		0x07	0	Instruction architecturally executed, condition check pass - store.
// [6]	LD		0x06	0	Instruction architecturally executed, condition check pass - load.
// [5]	DT1R	0x05	1	Level 1 data TLB refill.This event is implemented.
// [4]	DC1A	0x04	1	Level 1 data cache access.
// [3]	DC1R	0x03	1	Level 1 data cache refill.
// [2]	IT1R	0x02	1	Level 1 instruction TLB refill.
// [1]	IC1R	0x01	1	Level 1 instruction cache refill.
// [0]	SI		0x00	1	Instruction architecturally executed, condition check pass - software increment.

// Enable pmu event
static inline void
enable_pmu_event(uint32_t evtCount)
{
#if defined(__GNUC__) && defined __aarch64__
	evtCount &= ARMV8_PMEVTYPER_EVTCOUNT_MASK;
	asm volatile("isb");
	/* Just use counter 0 */
	asm volatile("msr pmevtyper0_el0, %0" : : "r" (evtCount));
	asm volatile("msr pmevtyper1_el0, %0" : : "r" (0x1e)); // chain counter 0&1
	/*   Performance Monitors Count Enable Set register set bit 31&1&0 */
	asm volatile("msr pmcntenset_el0, %0" : : "r" (80000000|3));
	// uint32_t r = 0;
	// asm volatile("mrs %0, pmcntenset_el0" : "=r" (r));
	// printf("after pmu enable %x\n", r);
#else
#error Unsupported architecture/compiler!
#endif
}

// Read event counter
static inline uint64_t
read_pmu(void)
{
#if defined(__GNUC__) && defined __aarch64__
        uint32_t r0 = 0;
        uint32_t r1 = 0;
        uint64_t r = 0;
	asm volatile("mrs %0, pmevcntr0_el0" : "=r" (r0)); 
	asm volatile("mrs %0, pmevcntr1_el0" : "=r" (r1)); 
	r = (uint64_t)r1 << 32 | r0;
	return r;
#else
#error Unsupported architecture/compiler!
#endif
}

// Disable pmu event
static inline void
disable_pmu_event(uint32_t evtCount)
{
#if defined(__GNUC__) && defined __aarch64__
	/*   Performance Monitors Count Enable Clr register: set bit 31&0&1 */
	asm volatile("msr pmcntenclr_el0, %0" : : "r" (80000000|3));
	// uint32_t r = 0;
	// asm volatile("mrs %0, pmcntenclr_el0" : "=r" (r));
	// printf("after pmu disable %x\n", r);
#else
#error Unsupported architecture/compiler!
#endif
}


#endif /* ARMPMU_LIB_H */
