//===--- LoggersCheck.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LoggersCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

llvm::Optional<RenamerClangTidyCheck::FailureInfo>
LoggersCheck::GetDeclFailureInfo(const NamedDecl *Decl,
                                 const SourceManager &SM) const {
  if (const auto *VariableDecl = dyn_cast<VarDecl>(Decl)) {
    auto Name = Decl->getName();
    if (Name != "logger") {
      return None;
    }

    auto MatchedType = VariableDecl->getType();
    const auto *MatchedIdentifier = MatchedType.getBaseTypeIdentifier();
    auto VarType = MatchedIdentifier->getName();

    if (VarType != "Logger") {
      return None;
    }

    return RenamerClangTidyCheck::FailureInfo{"ds::log::Logger",
                                              "nonGenericName"};
  }

  return None;
} // namespace dampsoft

RenamerClangTidyCheck::DiagInfo
LoggersCheck::GetDiagInfo(const NamingCheckId &ID,
                          const NamingCheckFailure &Failure) const {
  return DiagInfo{"invalid (too generic) name for %0 '%1'",
                  [&](DiagnosticBuilder &Diag) {
                    Diag << Failure.Info.KindName << ID.second;
                  }};
}

llvm::Optional<LoggersCheck::FailureInfo>
LoggersCheck::GetMacroFailureInfo(const Token &MacroNameTok,
                                  const SourceManager &SM) const {
  return None;
}

/*
void LoggersCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("x");
  auto MatchedType = MatchedDecl->getType();
  const auto *MatchedIdentifier = MatchedType.getBaseTypeIdentifier();
  auto VarType = MatchedIdentifier->getName();

  diag(MatchedDecl->getLocation(), "type: ") << VarType;

  if (VarType != "ds::log::Logger") {
    return;
  }

  if (MatchedDecl->getStorageDuration() != SD_Static) {
    diag(MatchedDecl->getLocation(), "Non-static Logger found: %0")
        << MatchedDecl;
    diag(MatchedDecl->getLocation(), "insert 'static'", DiagnosticIDs::Note)
        << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "static ");
  }

  if (MatchedDecl->getName().lower() == "logger") {
    diag(MatchedDecl->getLocation(),
         "Logger with generic name (logger) found: %0")
        << MatchedDecl;
    diag(MatchedDecl->getLocation(), "append more specific name",
         DiagnosticIDs::Note)
        << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "someName");
  }
}
*/
} // namespace dampsoft
} // namespace tidy
} // namespace clang
