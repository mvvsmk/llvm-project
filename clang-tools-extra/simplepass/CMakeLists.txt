set(LLVM_LINK_COMPONENTS
	Support
)

add_clang_executable(simplepass
simplepass.cpp
)

target_link_libraries(simplepass
	PRIVATE
	clangAST
	clangASTMatchers
	clangBasic
	clangFrontend
	clangCodeGen
	clangTooling
)