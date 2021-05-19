//===--- InlineStaticVariableCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "InlineStaticVariableCheck.h"
#include "../utils/LexerUtils.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

void InlineStaticVariableCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(isStaticStorageClass()).bind("x"), this);
}

void InlineStaticVariableCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("x");

  if (!MatchedDecl->isInline()) {
    return;
  }

  diag(MatchedDecl->getLocation(),
       "static inline variable definition detected: %0")
      << MatchedDecl;
  // FIXME: add fix
}

} // namespace dampsoft
} // namespace tidy
} // namespace clang
