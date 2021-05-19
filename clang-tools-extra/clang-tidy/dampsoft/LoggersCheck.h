//===--- LoggersCheck.h - clang-tidy ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_DAMPSOFT_LOGGERSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_DAMPSOFT_LOGGERSCHECK_H

#include "../utils/RenamerClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace dampsoft {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/dampsoft-loggers.html
class LoggersCheck : public RenamerClangTidyCheck {
public:
  LoggersCheck(StringRef Name, ClangTidyContext *Context)
      : RenamerClangTidyCheck(Name, Context) {}

private:
  llvm::Optional<FailureInfo>
  GetDeclFailureInfo(const NamedDecl *Decl,
                     const SourceManager &SM) const override;
  DiagInfo GetDiagInfo(const NamingCheckId &ID,
                       const NamingCheckFailure &Failure) const override;
  llvm::Optional<FailureInfo>
  GetMacroFailureInfo(const Token &MacroNameTok,
                      const SourceManager &SM) const override;
};

} // namespace dampsoft
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_DAMPSOFT_LOGGERSCHECK_H
