// simple example of a clang plugin tool
// which issues warning diagnostics on
// finding variables, functions and class names beginning with _

#include <unordered_set>

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/FileManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;

class NoUnderscoreVisitor : public RecursiveASTVisitor<NoUnderscoreVisitor> {
private:
	ASTContext *context;
	CompilerInstance& instance;

	DiagnosticsEngine& d;

	unsigned int warningID;

	bool isInHeader(Decl *decl) {
		auto loc = decl->getLocation();
		auto floc = context->getFullLoc(loc);
		if (floc.isInSystemHeader()) return true;
		auto entry = floc.getFileEntry()->getName();
		if (entry.endswith(".h") || entry.endswith(".hpp")) {
			return true;
		}
		return false;
	}

	void checkName(std::string name, Decl *decl) {
		if (name.at(0) == '_') {
			auto loc = context->getFullLoc(decl->getLocation());
			d.Report(loc, warningID) << name;
		}
	}

public:
	explicit NoUnderscoreVisitor(ASTContext *context, CompilerInstance& instance): 
			context(context), instance(instance), d(instance.getDiagnostics()) {
		warningID = d.getCustomDiagID(DiagnosticsEngine::Warning,
				"Name beginning with underscore: '%0'");
	}

	virtual bool VisitFunctionDecl(FunctionDecl *d) {
		if (isInHeader(d)) {
			return true;
		}
		checkName(d->getNameAsString(), d);
		return true;
	}

	virtual bool VisitVarDecl(VarDecl *d) {
		if (isInHeader(d)) {
			return true;
		}
		checkName(d->getNameAsString(), d);
		return true;
	}
};

class NoUnderscoreConsumer : public ASTConsumer {
	CompilerInstance& instance;
	NoUnderscoreVisitor visitor;

public:
	NoUnderscoreConsumer(CompilerInstance& instance)
			: instance(instance), visitor(&instance.getASTContext(), instance) {}

	virtual void HandleTranslationUnit(ASTContext &context) override {
		visitor.TraverseDecl(context.getTranslationUnitDecl());
	}
};

class NoUnderscoreAction : public PluginASTAction {
protected:
	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& instance, llvm::StringRef) override {
		return std::make_unique<NoUnderscoreConsumer>(instance);
	}

	virtual bool ParseArgs(const CompilerInstance&, const std::vector<std::string>&) override {
		return true;
	}

	virtual PluginASTAction::ActionType getActionType() override {
  		return PluginASTAction::AddAfterMainAction;
	}
};

static FrontendPluginRegistry::Add<NoUnderscoreAction> NoUnderscore("NoUnderscore", "Warn against underscores in function and variable names");

