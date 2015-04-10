import json

from fast_preprocess import (strip_excessive_repeats_ascii,
                             strip_excessive_repeats_utf8)


s = 'abcccdddccce'
max_repeat = 2

for f in [strip_excessive_repeats_ascii, strip_excessive_repeats_utf8]:
    drop_counts = {}
    new_s = f(s, max_repeat, drop_counts)
    print s, max_repeat, '->', new_s, drop_counts
