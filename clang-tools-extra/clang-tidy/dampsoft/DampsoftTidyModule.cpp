//===--- MiscTidyModule.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "AssertCheck.h"
#include "ChangeValueSameParametersCheck.h"
#include "InlineStaticVariableCheck.h"
#include "LoggersCheck.h"
#include "SociTransactionCheck.h"

namespace clang {
namespace tidy {
namespace dampsoft {

class DampsoftModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<AssertCheck>("dampsoft-assert");
    CheckFactories.registerCheck<ChangeValueSameParametersCheck>(
        "dampsoft-change-value-same-parameters");
    CheckFactories.registerCheck<InlineStaticVariableCheck>(
        "dampsoft-inline-static-variable");
    CheckFactories.registerCheck<LoggersCheck>(
        "dampsoft-loggers");
    CheckFactories.registerCheck<SociTransactionCheck>(
        "dampsoft-soci-transaction");
  }
};

} // namespace dampsoft

// Register the DampsoftTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<dampsoft::DampsoftModule>
    X("dampsoft-module", "Adds DAMPSOFT lint checks.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the DampsoftModule.
volatile int DampsoftModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
