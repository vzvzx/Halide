#include "CodeGen_MIPS.h"
#include "Util.h"
#include "LLVM_Headers.h"

namespace Halide {
namespace Internal {

using std::vector;
using std::string;

using namespace llvm;

CodeGen_MIPS::CodeGen_MIPS(Target t) : CodeGen_Posix(t) {
    #if !(WITH_MIPS)
    user_error << "llvm build not configured with MIPS target enabled.\n";
    #endif
    user_assert(llvm_Mips_enabled) << "llvm build not configured with MIPS target enabled.\n";
}

string CodeGen_MIPS::mcpu() const {
    if (target.bits == 32) {
        return "";
    } else {
        return "";
    }
}

string CodeGen_MIPS::mattrs() const {
    if (target.bits == 32) {
        return "";
    } else {
        return "mips64r6";
    }
}

bool CodeGen_MIPS::use_soft_float_abi() const {
    return false;
}

int CodeGen_MIPS::native_vector_bits() const {
    return 128;
}

void CodeGen_MIPS::visit(const Call *op) {
    if (op->call_type == Call::Intrinsic &&
        op->name == Call::profiling_timer) {
        // LLVM doesn't know how to deal with read-cycle-counter on mips.
        internal_assert(op->args.size() == 1);
        Expr e = Call::make(UInt(64), "halide_current_time_ns", std::vector<Expr>(), Call::Extern);
        e.accept(this);
    } else {
        CodeGen_Posix::visit(op);
    }
}

}}
