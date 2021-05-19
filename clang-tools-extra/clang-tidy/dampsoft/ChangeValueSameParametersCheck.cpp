//===--- ChangeValueSameParametersCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ChangeValueSameParametersCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

static llvm::Optional<std::string> getArgName(const clang::Expr *Expr) {
  // Local variable
  if (auto *Arg = dyn_cast<DeclRefExpr>(Expr)) {
    return Arg->getNameInfo().getName().getAsString();
  }

  // Implicitly converted variable
  if (auto *Cast = dyn_cast<ImplicitCastExpr>(Expr)) {
    if (auto *Arg = dyn_cast<DeclRefExpr>(Cast->getSubExpr())) {
      return Arg->getNameInfo().getName().getAsString();
    }

    if (auto *Arg = dyn_cast<MemberExpr>(Cast->getSubExpr())) {
      return Arg->getMemberNameInfo().getName().getAsString();
    }
  }

  // Trivial to copy member variables (=> primitive data types &
  // pointers/references)
  if (auto *Arg = dyn_cast<MemberExpr>(Expr)) {
    return Arg->getMemberNameInfo().getName().getAsString();
  }

  // Non-trivial to copy member variables (where a (copy-)constructor is
  // called when passing the variable)
  if (auto *Constructor = dyn_cast<CXXConstructExpr>(Expr)) {
    if (Constructor->getNumArgs() > 0) {
      return getArgName(Constructor->getArg(0));
    }
  }

  return None;
}

static bool isMemberVariable(const clang::Expr *Expr) {
  // Trivial to copy member variables (=> primitive data types &
  // pointers/references)
  if (dyn_cast<MemberExpr>(Expr) != nullptr) {
    return true;
  }

  // Non-trivial to copy member variables (where a (copy-)constructor is
  // called when passing the variable)
  if (auto *Constructor = dyn_cast<CXXConstructExpr>(Expr)) {
    if (Constructor->getNumArgs() > 0) {
      auto *Arg = Constructor->getArg(0);
      // The argument to the (copy-)constructor might be implicitly casted (e.g.
      // to a reference)
      if (auto *Cast = dyn_cast<ImplicitCastExpr>(Arg)) {
        if (dyn_cast<MemberExpr>(Cast->getSubExpr()) != nullptr) {
          return true;
        }
      }

      if (dyn_cast<MemberExpr>(Arg) != nullptr) {
        return true;
      }
    }
  }

  return false;
}

void ChangeValueSameParametersCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("changeValue")))).bind("x"), this);
}

void ChangeValueSameParametersCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<CallExpr>("x");
  if (MatchedDecl->getNumArgs() == 2) {
    const auto *FirstArg = MatchedDecl->getArg(0);
    auto FirstArgName = getArgName(FirstArg);
    const auto *SecondArg = MatchedDecl->getArg(1);
    auto SecondArgName = getArgName(SecondArg);

    if (FirstArgName && SecondArgName) {
      if (FirstArgName.getValue() == SecondArgName.getValue() &&
          isMemberVariable(FirstArg) == isMemberVariable(SecondArg)) {
        diag(MatchedDecl->getExprLoc(),
             "changeValue mustn't be called with the same parameter twice!");
      }
    }
  }
}

} // namespace dampsoft
} // namespace tidy
} // namespace clang
