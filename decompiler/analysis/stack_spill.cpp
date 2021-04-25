#include <stdexcept>
#include "third-party/fmt/core.h"
#include "stack_spill.h"
#include "decompiler/Disasm/DecompilerLabel.h"

namespace decompiler {

std::string StackSpillSlot::print() const {
  return fmt::format("[{:3d}] {}{}", offset, is_signed ? 's' : 'u', size * 8);
}

void StackSpillMap::add_access(const StackSpillSlot& access) {
  auto existing = m_slot_map.find(access.offset);
  if (existing != m_slot_map.end()) {
    if (access != existing->second) {
      throw std::runtime_error(fmt::format("Inconsistent stack access:\n{}\n{}\n",
                                           existing->second.print(), access.print()));
    }
  } else {
    m_slot_map.insert({access.offset, access});
  }
}

const StackSpillSlot& StackSpillMap::lookup(int offset) const {
  auto result = m_slot_map.find(offset);
  if (result == m_slot_map.end()) {
    throw std::runtime_error(fmt::format("unknown stack spill slot at offset {}", offset));
  }
  return result->second;
}

void StackSpillMap::finalize() {
  // how many variables exist at each byte. should be 1 or 0.
  int max_offset = 0;
  for (auto& slot : m_slot_map) {
    max_offset = std::max(max_offset, slot.second.offset + slot.second.size);
  }

  assert(max_offset < 4096);  // just a sanity check here
  std::vector<int> var_count(max_offset, 0);

  for (auto& slot : m_slot_map) {
    for (int i = 0; i < slot.second.size; i++) {
      var_count.at(slot.second.offset + i)++;
    }
  }

  for (size_t i = 0; i < var_count.size(); i++) {
    if (var_count[i] > 1) {
      throw std::runtime_error(
          fmt::format("There are {} variables at stack offset {}", var_count[i], i));
    }
  }
}

int StackSpillMap::size() const {
  return m_slot_map.size();
}

namespace {
struct StackInstrInfo {
  InstructionKind kind;
  bool is_load;
  int size;
  bool is_signed;
};

constexpr StackInstrInfo stack_instrs[] = {{InstructionKind::SQ, false, 16, false},
                                           {InstructionKind::LQ, true, 16, false}};
}  // namespace

StackSpillMap build_spill_map(const std::vector<Instruction>& instructions, Range<int> range) {
  StackSpillMap map;

  for (auto idx : range) {
    auto& instr = instructions.at(idx);

    for (auto& instr_template : stack_instrs) {
      if (instr.kind == instr_template.kind) {
        // we are the right kind.
        auto src_reg = instr.get_src(instr_template.is_load ? 1 : 2).get_reg();
        if (src_reg == Register(Reg::GPR, Reg::SP)) {
          StackSpillSlot slot;
          slot.offset = instr.get_src(instr_template.is_load ? 0 : 1).get_imm();
          slot.size = instr_template.size;
          slot.is_signed = instr_template.is_signed;
          map.add_access(slot);
        }
        break;
      }
    }
  }

  map.finalize();
  return map;
}
}  // namespace decompiler