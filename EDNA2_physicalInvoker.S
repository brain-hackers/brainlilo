.align 4

.global EDNA2_physicalInvoker
EDNA2_physicalInvoker:
    // r0-r7=params
    // r8=proc address
    nop 
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    msr	cpsr_c, #211 // to supervisor mode
    mov	r9, #0
    mcr	p15,0,r9,c13,c0,0 // clear fcse PID
    mrc	p15,0,r9,c1,c0,0  // read ctrl regs
    bic	r9, r9, #5        // disable MMU/DCache
    bic	r9, r9, #4096     // disable ICache
    orr	r9, r9, #8192     // and reset vectors to upper
    mcr	p15,0,r9,c1,c0,0  // write ctrl regs
    mov	r9, #0
    mcr	p15,0,r9,c7,c7,0 // invalidate cache
    mcr	p15,0,r9,c8,c7,0 // invalidate tlb
    mov	pc, r8
    nop
    nop
