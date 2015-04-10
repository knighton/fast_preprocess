go:
	#CFLAGS="-std=c++0x" python setup.py build_ext -i
	CFLAGS="-std=c++11" python setup.py build_ext -i
	python go.py

run_test:
	clang++ test/unicode_test.cpp test/util.cpp cpp_ext/unicode.cpp -o test/unicode.test
	./test/unicode.test
