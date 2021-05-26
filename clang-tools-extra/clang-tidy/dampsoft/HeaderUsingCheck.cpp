//===--- HeaderUsingCheck.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "HeaderUsingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

void HeaderUsingCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(usingDirectiveDecl().bind("x"), this);
}

void HeaderUsingCheck::check(const MatchFinder::MatchResult &Result) {
  auto *const MatchedDecl = Result.Nodes.getNodeAs<UsingDirectiveDecl>("x");
  auto *SM = Result.SourceManager;
  const FileEntry *Entry = SM->getFileEntryForID(SM->getMainFileID());
  StringRef FileName = llvm::sys::path::filename(Entry->getName());
  if (utils::getFileExtension(FileName, this->HeaderFileExtensions)) {
    diag(MatchedDecl->getLocation(),
         "'using namespace' mustn't be called in headers!");
  }
}

HeaderUsingCheck::HeaderUsingCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      RawStringHeaderFileExtensions(Options.getLocalOrGlobal(
          "HeaderFileExtensions", utils::defaultHeaderFileExtensions())) {
  if (!utils::parseFileExtensions(RawStringHeaderFileExtensions,
                                  HeaderFileExtensions,
                                  utils::defaultFileExtensionDelimiters())) {
    this->configurationDiag("Invalid header file extension: '%0'")
        << RawStringHeaderFileExtensions;
  }
}

void HeaderUsingCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "HeaderFileExtensions",
                this->RawStringHeaderFileExtensions);
}

} // namespace dampsoft
} // namespace tidy
} // namespace clang
