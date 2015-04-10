from distutils.core import setup, Extension
 
c_ext = Extension(
    'fast_preprocess',
    [
        'c_ext/fast_preprocess_module.c',
        'c_ext/unicode.c',
    ]
)

cpp_ext = Extension(
    'fast_preprocess_cpp',
    [
        'cpp_ext/fast_preprocess_module.cpp',
        'cpp_ext/unicode.cpp',
    ]
)
 
setup(
    ext_modules=[c_ext, cpp_ext],
)
