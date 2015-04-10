# -*- encoding: utf-8 -*-

from fast_preprocess import destutter_utf8_encoded_str


asc = 'abcccdddccce'
uni = '☃☃☃☃☃☃ fuuuuu'
uni2 = u'\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2'.encode('utf-8')
max_repeat = 2

for f in [destutter_utf8_encoded_str]:
    for s in [asc, uni, uni2]:
        drop_counts = {}
        new_s = f(s, max_repeat, drop_counts)
        print s, max_repeat, '->', new_s, drop_counts
