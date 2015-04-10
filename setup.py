from distutils.core import setup, Extension
 
c_ext = Extension(
    'fast_preprocess',
    [
        'cc_ext/fast_preprocess_module.cpp',
        'cc_ext/utf8.cpp',
    ]
)
 
setup(
    ext_modules=[c_ext],
)
