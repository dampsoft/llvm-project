//===--- HeaderStdafxCheck.h - clang-tidy ------------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_DAMPSOFT_HEADERSTDAFXCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_DAMPSOFT_HEADERSTDAFXCHECK_H

#include "../ClangTidyCheck.h"
#include "../utils/FileExtensionsUtils.h"

namespace clang {
namespace tidy {
namespace dampsoft {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/dampsoft-header-stdafx.html
class HeaderStdafxCheck : public ClangTidyCheck {
public:
  HeaderStdafxCheck(StringRef Name, ClangTidyContext *Context);
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;

private:
  utils::FileExtensionsSet HeaderFileExtensions;
  const std::string RawStringHeaderFileExtensions;
};

} // namespace dampsoft
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_DAMPSOFT_HEADERSTDAFXCHECK_H
