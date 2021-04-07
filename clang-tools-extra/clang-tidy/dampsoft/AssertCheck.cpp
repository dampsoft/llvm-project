//===--- AssertCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AssertCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

void AssertCheck::registerMatchers(MatchFinder *Finder) {
  // C++ only check
  if (!getLangOpts().CPlusPlus) {
    return;
  }

  Finder->addMatcher(callExpr(callee(functionDecl())).bind("x"), this);
}

void AssertCheck::registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                      Preprocessor *ModuleExpanderPP) {}

void AssertCheck::check(const MatchFinder::MatchResult &Result) {
  const SourceManager &SM = *Result.SourceManager;
  const LangOptions LangOpts = getLangOpts();
  SourceLocation Loc = Result.Nodes.getNodeAs<Stmt>("x")->getBeginLoc();

  bool Found = false;
  while (Loc.isValid() && Loc.isMacroID()) {
    StringRef MacroName = Lexer::getImmediateMacroName(Loc, SM, LangOpts);

    if ("assert" == MacroName) {
      Found = true;
      break;
    }
    Loc = SM.getImmediateMacroCallerLoc(Loc);
  }
  if (!Found) {
    return;
  }

  diag(Loc, "assert() must not be used since it terminates the program!");
}

} // namespace dampsoft
} // namespace tidy
} // namespace clang
