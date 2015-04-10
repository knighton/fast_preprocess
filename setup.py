from distutils.core import setup, Extension
 
c_ext = Extension(
    'fast_preprocess',
    [
        'c_ext/fast_preprocess_module.c',
        'c_ext/lib/hashmap.c',
        'c_ext/lib/unicode.c',
        'c_ext/lib/util.c',
    ]
)
 
setup(
    ext_modules=[c_ext],
)
