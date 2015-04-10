go:
	CFLAGS="-Wall -Wextra -Werror" python setup.py build_ext -i
	python go.py

run_test:
	clang++ test/unicode_test.cpp test/util.cpp cpp_ext/unicode.cpp -o test/unicode.test
	./test/unicode.test
