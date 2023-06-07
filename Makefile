compile:
	@ninja -C build
	@mv build/solver .

install:
	@mkdir -p $(HOME)/.local/bin/
	@cp build/solver $(HOME)/.local/bin/sudoku_solver
