#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Tooling/Inclusions/HeaderIncludes.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/PreprocessorOptions.h"
#include <unordered_set>

using namespace std;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace llvm;

// Apply a custom category to all command-line options

static llvm::cl::OptionCategory simplepass("SimplePass Options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp("\n More help text");

DeclarationMatcher functiondeclmatcher = functionDecl().bind("function decl");
StatementMatcher functionCall = callExpr().bind("function call");
// auto fucntionCall_or_decl matcher =
// anyOf(expr(callExpr).bind("function call"),
//   decl(functionDecl().bind("function")));
// StatementMatcher LoopMatcher =
//     forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0)))).bind("initVarName")))),hasIncrement(unaryOperator(hasOperatorName("++"),hasUnaryOperand(declRefExpr(to(varDecl(hasType(isInteger())).bind("incVarName")))))),hasCondition(binaryOperator(hasOperatorName("<"),hasLHS(ignoringParenImpCasts(declRefExpr(to(varDecl(hasType(isInteger())).bind("condVarName"))))),hasRHS(expr(hasType(isInteger())))))).bind("forLoop");
class Simple_Pass_Handler : public MatchFinder::MatchCallback {

public:
  static std::unordered_set<const Decl *> function_decs;
  static std::unordered_set<const Decl *> function_calls;

  virtual void run(const MatchFinder::MatchResult &Result) override {
    if (const Decl *FD =
            Result.Nodes.getNodeAs<FunctionDecl>("function decl")) {
      if (function_decs.count(FD) == 0) {
        function_decs.insert(FD);
      }
    }
    if (const CallExpr *CE =
            Result.Nodes.getNodeAs<CallExpr>("function call")) {
      const Decl *FD = CE->getCalleeDecl();
      if (function_calls.count(FD) == 0) {
        function_calls.insert(FD);
      }
    }
  }
  virtual void onEndOfTranslationUnit() override {
    for (const Decl *DCL : function_decs) {
      if (function_calls.count(DCL) == 0) {
        const FunctionDecl *FD = static_cast<const FunctionDecl *>(DCL);
        outs() << "This function is not called :" << FD->getNameAsString()
               << "\n";
      }
    }
  }
};

std::unordered_set<const Decl *> Simple_Pass_Handler::function_calls = {};
std::unordered_set<const Decl *> Simple_Pass_Handler::function_decs = {};

int main(int argc, const char **argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, simplepass);
  if (!ExpectedParser) {
    errs() << ExpectedParser.takeError();
    return 1;
  }

  CommonOptionsParser &OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  Simple_Pass_Handler Handler;
  MatchFinder Finder;
  Finder.addMatcher(functiondeclmatcher, &Handler);
  Finder.addMatcher(functionCall, &Handler);
  Tool.run(newFrontendActionFactory(&Finder).get());
}
