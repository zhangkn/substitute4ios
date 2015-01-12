#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "dis.h"

typedef struct { bool modify; } tdis_ret;
typedef struct tc {
    uint32_t pc;
    uint32_t op;
    uint32_t newop;
    uint32_t newval[4];

} *tdis_ctx;
#define P(x) P_##x

NOINLINE
static tdis_ret P_data(struct tc *ctx, unsigned o0, unsigned o1, unsigned o2, unsigned o3, unsigned out_mask) {
    printf("data: %08x\n", ctx->op);
    unsigned os[] = {o0, o1, o2, o3};
    for(size_t i = 0; i < 4; i++) {
        unsigned val = os[i];
        if(val == -1u)
            break;
        printf("    reg %x: %s\n", val, out_mask & (1 << i) ? "out" : "in");
        ctx->newval[i] = i;
    }
    return (tdis_ret) {true};
}

NOINLINE
static tdis_ret P_pcrel(struct tc *ctx, UNUSED uint32_t dpc, UNUSED unsigned reg, UNUSED bool is_load) {
    printf("adr: %08x\n", ctx->op);
    return (tdis_ret) {false};
}

NOINLINE
static tdis_ret P_ret(struct tc *ctx) {
    printf("ret: %08x\n", ctx->op);
    return (tdis_ret) {false};
}

NOINLINE
static tdis_ret P_branch(struct tc *ctx, UNUSED uint32_t dpc) {
    printf("branch: %08x\n", ctx->op);
    return (tdis_ret) {false};
}

NOINLINE
static tdis_ret P_unidentified(struct tc *ctx) {
    printf("unidentified: %08x\n", ctx->op);
    return (tdis_ret) {false};

}
#include "dis-arm.inc.h"

int main(UNUSED int argc, char **argv) {
    struct tc ctx;
    ctx.pc = 0xdead0000;
    ctx.op = (uint32_t) strtoll(argv[1] ? argv[1] : "deadbeef", NULL, 16);
    P_dis_arm(&ctx);
    printf("==> %x\n", ctx.newop);

}