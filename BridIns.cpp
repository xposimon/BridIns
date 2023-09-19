#include "clang/Frontend/FrontendActions.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp("\nMore help text...\n");

StatementMatcher IfMatcher = ifStmt().bind("ifStmt");
StatementMatcher ForMatcher = forStmt().bind("forStmt");
StatementMatcher WhileMatcher = whileStmt().bind("whileStmt");
StatementMatcher SwitchMatcher = switchStmt().bind("switchStmt");
DeclarationMatcher mainFunctionMatcher = functionDecl(hasName("main"), isMain()).bind("mainFunc");


class IfStmtHandler : public MatchFinder::MatchCallback {
public:
  virtual void run(const MatchFinder::MatchResult &Result) {
    // Access the matched 'if' statement
    const clang::IfStmt *ifStmt = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt");
    SourceLocation ifStmtLoc;
    if (ifStmt) {
      // Process the 'if' statement here
      ifStmtLoc = ifStmt->getBeginLoc();
      llvm::outs() << "[If]" << ifStmtLoc.printToString(Result.Context->getSourceManager()) << "\n";
      //ifStmt->dump();
    }
  }
};

class ForStmtHandler : public MatchFinder::MatchCallback {
public:
  virtual void run(const MatchFinder::MatchResult &Result) {
    // Access the matched 'if' statement
    const clang::ForStmt *forStmt = Result.Nodes.getNodeAs<clang::ForStmt>("forStmt");
    SourceLocation forStmtLoc;
    if (forStmt) {
      // Process the 'if' statement here
      forStmtLoc = forStmt->getBeginLoc();
      llvm::outs() << "[For]" << forStmtLoc.printToString(Result.Context->getSourceManager()) << "\n";
    }
  }
};

class WhileStmtHandler : public MatchFinder::MatchCallback {
public:
  virtual void run(const MatchFinder::MatchResult &Result) {
    // Access the matched 'if' statement
    const clang::WhileStmt *whileStmt = Result.Nodes.getNodeAs<clang::WhileStmt>("whileStmt");
    SourceLocation whileStmtLoc;
    if (whileStmt) {
      whileStmtLoc = whileStmt->getBeginLoc();
      llvm::outs() << "[While]" << whileStmtLoc.printToString(Result.Context->getSourceManager()) << "\n";
    }
  }
};

class SwitchStmtHandler : public MatchFinder::MatchCallback {
public:
  virtual void run(const MatchFinder::MatchResult &Result) {
    // Access the matched 'if' statement
    const clang::SwitchStmt *switchStmt = Result.Nodes.getNodeAs<clang::SwitchStmt>("switchStmt");
    SourceLocation switchStmtLoc;
    if (switchStmt) {
      // Process the 'if' statement here
      switchStmtLoc = switchStmt->getBeginLoc();
      llvm::outs() << "[Switch]" << switchStmtLoc.printToString(Result.Context->getSourceManager()) << "\n";
    }
  }
};

class MainFunctionHandler : public MatchFinder::MatchCallback {
public:
  virtual void run(const MatchFinder::MatchResult &Result) {
    const clang::FunctionDecl *mainFunction = Result.Nodes.getNodeAs<clang::FunctionDecl>("mainFunc");
    SourceLocation mainFuncLoc;
    if (mainFunction) {
      mainFuncLoc = mainFunction->getBeginLoc();
      llvm::outs() << "[Main]" << mainFuncLoc.printToString(Result.Context->getSourceManager()) << "\n";
    }
  }
};


int main(int argc, const char **argv) {
  // Create an instance of ClangTool
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
    }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  // Create a MatchFinder
  IfStmtHandler ifStmtHandler;
  ForStmtHandler forStmtHandler;
  WhileStmtHandler whileStmtHandler;
  SwitchStmtHandler switchStmtHandler;
  MainFunctionHandler mainFunctionHandler;

  MatchFinder Finder;

  // Register your matcher with the Finder
  Finder.addMatcher(IfMatcher, &ifStmtHandler);
  Finder.addMatcher(ForMatcher, &forStmtHandler);
  Finder.addMatcher(WhileMatcher, &whileStmtHandler);
  Finder.addMatcher(SwitchMatcher, &switchStmtHandler);
  Finder.addMatcher(mainFunctionMatcher, &mainFunctionHandler);

  // Run the tool, and let the Finder find matches in the AST
  int result = Tool.run(newFrontendActionFactory(&Finder).get());
  return result;
}
