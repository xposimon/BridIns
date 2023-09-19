#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

class IfStatementMatcher : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
        const clang::IfStmt *ifStmt = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt");
        if (ifStmt) {
            // Process the matched if statement
            ifStmt->dump(); // Example: Dump the AST of the matched if statement
        }
    }
};

class IfStmtHandler : public MatchFinder::MatchCallback {
public:
  virtual void run(const MatchFinder::MatchResult &Result) {
    // Access the matched 'if' statement
    const clang::IfStmt *ifStmt = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt");
    
    if (ifStmt) {
      // Process the 'if' statement here
      llvm::outs() << "Found an 'if' statement:\n";
      ifStmt->dump();
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
  IfStatementMatcher ifStmtMatcher;
  MatchFinder Finder;

  // Register your matcher with the Finder
  Finder.addMatcher(ifStmtMatcher, &ifStmtHandler);

  // Run the tool, and let the Finder find matches in the AST
  int result = Tool.run(newFrontendActionFactory(&Finder).get());

  return result;
}
