//===--- SociTransactionCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SociTransactionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

void SociTransactionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl().bind("x"), this);
}

void SociTransactionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("x");
  auto VarName = MatchedDecl->getName();
  auto MatchedType = MatchedDecl->getType();
  if (const auto *MatchedIdentifier = MatchedType.getBaseTypeIdentifier()) {
    auto VarType = MatchedIdentifier->getName();

    if (VarType != "transaction" || VarName == "") {
      return;
    }

    diag(MatchedDecl->getLocation(),
         "soci::transaction used instead of ds::Transaction: %0")
        << MatchedDecl;
  }
}

} // namespace dampsoft
} // namespace tidy
} // namespace clang
