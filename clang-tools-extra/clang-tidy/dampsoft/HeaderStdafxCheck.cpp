//===--- HeaderStdafxCheck.cpp - clang-tidy
//--------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "HeaderStdafxCheck.h"
#include "../utils/FileExtensionsUtils.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace dampsoft {

namespace {
class HeaderStdafxCheckPPCallbacks : public PPCallbacks {
public:
  HeaderStdafxCheckPPCallbacks(HeaderStdafxCheck *Check,
                               const SourceManager &SM,
                               utils::FileExtensionsSet HeaderExtensions)
      : HeaderExtensions(std::move(HeaderExtensions)), SM(SM), Check(Check) {}

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange, const FileEntry *File,
                          StringRef SearchPath, StringRef RelativePath,
                          const Module *Imported,
                          SrcMgr::CharacteristicKind FileType) override {
    if (FileName == "stdafx.h") {
      this->StdafxIncludes.push_back(std::move(HashLoc));
    }
  }

  void EndOfMainFile() override {
    const FileEntry *Entry = SM.getFileEntryForID(SM.getMainFileID());
    StringRef FileName = llvm::sys::path::filename(Entry->getName());
    if (utils::getFileExtension(FileName, this->HeaderExtensions)) {
      for (const auto &SL : this->StdafxIncludes) {
        this->Check->diag(SL, "#stdafx.h mustn't be included in headers!")
            << FileName;
      }
    }
  }

private:
  std::vector<SourceLocation> StdafxIncludes;
  utils::FileExtensionsSet HeaderExtensions;

  const SourceManager &SM;
  HeaderStdafxCheck *Check;
};
} // namespace

HeaderStdafxCheck::HeaderStdafxCheck(StringRef Name, ClangTidyContext *Context)
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

void HeaderStdafxCheck::registerPPCallbacks(const SourceManager &SM,
                                            Preprocessor *PP,
                                            Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<HeaderStdafxCheckPPCallbacks>(
      this, SM, this->HeaderFileExtensions));
}

void HeaderStdafxCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "HeaderFileExtensions",
                this->RawStringHeaderFileExtensions);
}

} // namespace dampsoft
} // namespace tidy
} // namespace clang
