.PHONY: all test test-c test-cpp test-rust clean

all: test

test: test-c test-cpp test-rust
	@echo ""
	@echo "======================================================"
	@echo "  ALL TESTS PASSED ACROSS C, C++, AND RUST SUITES!  "
	@echo "======================================================"

test-c:
	@echo "--- [1/3] Building & Running C Tests ---"
	@$(MAKE) -C c test

test-cpp:
	@echo "--- [2/3] Building & Running C++ Tests ---"
	@$(MAKE) -C cpp test

test-rust:
	@echo "--- [3/3] Building & Running Rust Tests ---"
	@cargo test --manifest-path rust/Cargo.toml --quiet

clean:
	@$(MAKE) -C c clean
	@$(MAKE) -C cpp clean
	@cargo clean --manifest-path rust/Cargo.toml
