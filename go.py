import json

from fast_preprocess import strip_excessive_repeats


s = 'abcccdddccce'
max_repeat = 2
drop_counts = {}
new_s = strip_excessive_repeats(s, max_repeat, drop_counts)
print s, max_repeat, drop_counts
print new_s
print json.dumps(drop_counts, indent=4)
