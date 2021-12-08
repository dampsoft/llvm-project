//===--- StdLockGuardCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StdLockGuardCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

void StdLockGuardCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl().bind("x"), this);
}

void StdLockGuardCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("x");
  auto VarName = MatchedDecl->getName();
  auto MatchedType = MatchedDecl->getType();
  if (const auto *MatchedIdentifier = MatchedType.getBaseTypeIdentifier()) {
    auto VarType = MatchedIdentifier->getName();

    if (VarType != "lock_guard" || VarName == "") {
      return;
    }

    diag(MatchedDecl->getLocation(),
         "std::lock_guard used instead of std::scoped_lock: %0")
        << MatchedDecl;
  }
}

} // namespace dampsoft
} // namespace tidy
} // namespace clang
