//===--- ThisCaptureThreadFunctionCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ThisCaptureThreadFunctionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

void ThisCaptureThreadFunctionCheck::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(
      cxxConstructExpr(hasType(recordDecl(hasName("::ds::ThreadFunction"))))
          .bind("x"),
      this);
}

void ThisCaptureThreadFunctionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<CXXConstructExpr>("x");

  if (MatchedDecl->getNumArgs() > 0) {
    // Pre C++17 the Lambda-Term is wrapped in two additional layers
    if (auto *LambdaConstruct =
            dyn_cast<CXXConstructExpr>(MatchedDecl->getArg(0))) {
      if (auto *TemporaryExpr =
              dyn_cast<MaterializeTemporaryExpr>(LambdaConstruct->getArg(0))) {
        this->analyseLambda(MatchedDecl, TemporaryExpr->getSubExpr());
      }
    }
    // In C++17 we can retrieve it directly
    this->analyseLambda(MatchedDecl, MatchedDecl->getArg(0));
  }
}

void ThisCaptureThreadFunctionCheck::analyseLambda(
    const CXXConstructExpr *MatchedDecl, const Expr *LambdaTerm) {
  if (auto *Lambda = dyn_cast<LambdaExpr>(LambdaTerm)) {
    for (const auto &Capture : Lambda->captures()) {
      if (Capture.capturesThis()) {
        diag(MatchedDecl->getExprLoc(),
             "ThreadFunction mustn't capture 'this'!");
      }
    }
  }
}

} // namespace dampsoft
} // namespace tidy
} // namespace clang
