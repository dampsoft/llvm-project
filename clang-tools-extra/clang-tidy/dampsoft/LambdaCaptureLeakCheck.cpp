//===--- LambdaCaptureLeakCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LambdaCaptureLeakCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

void LambdaCaptureLeakCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxMemberCallExpr().bind("x"), this);
}

void LambdaCaptureLeakCheck::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  const auto *MatchedDecl = Result.Nodes.getNodeAs<CXXMemberCallExpr>("x");
  if (auto *Obj = dyn_cast<DeclRefExpr>(
          MatchedDecl->getImplicitObjectArgument()->getExprStmt())) {
    const auto ObjName = Obj->getNameInfo().getAsString();
    for (unsigned int I = 0; I < MatchedDecl->getNumArgs(); I++) {
      auto *Arg = MatchedDecl->getArg(I);
      // Pre C++17 the Lambda-Term is wrapped in two additional layers
      if (auto *LambdaConstruct = dyn_cast<CXXConstructExpr>(Arg)) {
        if (auto *TemporaryExpr = dyn_cast<MaterializeTemporaryExpr>(
                LambdaConstruct->getArg(0))) {
          this->analyseLambda(MatchedDecl, TemporaryExpr->getSubExpr(),
                              ObjName);
        }
      }
      // In C++17 we can retrieve it directly
      this->analyseLambda(MatchedDecl, Arg, ObjName);
    }
  }
}

void LambdaCaptureLeakCheck::analyseLambda(const CXXMemberCallExpr *MatchedDecl,
                                           const Expr *LambdaTerm,
                                           const std::string &ObjName) {
  if (auto *Lambda = dyn_cast<LambdaExpr>(LambdaTerm)) {
    for (const auto &Capture : Lambda->captures()) {
      auto *Var = Capture.getCapturedVar();
      if (Var->getDeclName().getAsString() == ObjName) {
        diag(MatchedDecl->getExprLoc(),
             "Lambda leak (self scoped SharedPtr) found");
      }
    }
  }
}

} // namespace dampsoft
} // namespace tidy
} // namespace clang
