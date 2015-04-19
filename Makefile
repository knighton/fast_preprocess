EXT_TMP_BUILD_DIR=build/
EXT_OUT_DIR=fast_preprocess/build/
EXT_COMPILER_TYPE=unix

EXT_BASE_FLAGS=-fdiagnostics-color -O3 -std=c11
EXT_PARANOIA=-Wpedantic -Wall -Wextra -Werror
EXT_DISABLE_WARNINGS=\
	-Wno-exit-time-destructors \
	-Wno-global-constructors \
	-Wno-padded \

CFLAGS=$(EXT_BASE_FLAGS) $(EXT_PARANOIA) $(EXT_DISABLE_WARNINGS)

python_extension:
	@rm -rf $(EXT_OUT_DIR)
	@mkdir -p $(EXT_OUT_DIR)
	@touch $(EXT_OUT_DIR)/__init__.py
	@CFLAGS="$(CFLAGS)" python setup.py build_ext --build-temp $(EXT_TMP_BUILD_DIR) --build-lib $(EXT_OUT_DIR) > /dev/null
	@rm -rf $(EXT_TMP_BUILD_DIR)

clean:
	rm -rf $(EXT_TMP_BUILD_DIR)
	rm -rf $(EXT_OUT_DIR)

all: python_extension

go:
	python go.py
