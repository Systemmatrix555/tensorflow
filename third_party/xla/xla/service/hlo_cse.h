/* Copyright 2017 The OpenXLA Authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef XLA_SERVICE_HLO_CSE_H_
#define XLA_SERVICE_HLO_CSE_H_

#include "absl/container/flat_hash_set.h"
#include "xla/hlo/ir/hlo_instruction.h"
#include "xla/hlo/ir/hlo_module.h"
#include "xla/service/hlo_pass_interface.h"

namespace xla {

// A pass which performs common-subexpression elimination. Identical constants
// and identical instructions with the same operands are commoned. The pass
// iterates over the instructions in topological order which enables the pass to
// find arbitrarily large common expressions.
class HloCSE : public HloModulePass {
 public:
  // If is_layout_sensitive is true, then the simplifier preserves layout during
  // transformation. Otherwise, layout is ignored.
  // If ignore_control_dependencies is true, the pass will ignore control deps
  // when replacing instructions with their equivalents.
  explicit HloCSE(
      bool is_layout_sensitive, bool only_fusion_computations = false,
      bool ignore_control_dependencies = false, bool only_scalars = false,
      bool is_sharding_sensitive = true, bool allow_compatible_sharding = false,
      absl::flat_hash_set<const HloInstruction*> instructions_to_skip = {})
      : is_layout_sensitive_(is_layout_sensitive),
        only_fusion_computations_(only_fusion_computations),
        ignore_control_dependencies_(ignore_control_dependencies),
        only_scalars_(only_scalars),
        is_sharding_sensitive_(is_sharding_sensitive),
        allow_compatible_sharding_(allow_compatible_sharding),
        instructions_to_skip_(instructions_to_skip) {}
  ~HloCSE() override = default;
  absl::string_view name() const override { return "cse"; }

  // Run CSE on the given module. Returns whether the module was changed (common
  // subexpressions were found and eliminated).
  using HloPassInterface::Run;
  absl::StatusOr<bool> Run(
      HloModule* module,
      const absl::flat_hash_set<absl::string_view>& execution_threads) override;

 private:
  const bool is_layout_sensitive_;
  const bool only_fusion_computations_;
  const bool ignore_control_dependencies_;
  const bool only_scalars_;
  const bool is_sharding_sensitive_;
  const bool allow_compatible_sharding_;
  absl::flat_hash_set<const HloInstruction*> instructions_to_skip_;
};

}  // namespace xla

#endif  // XLA_SERVICE_HLO_CSE_H_
