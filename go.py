# -*- encoding: utf-8 -*-

from fast_preprocess import (destutter_unicode, destutter_utf8_encoded_bytes)


def show_bytes(s, max_repeat, new_s, drop_counts):
    print '%s, max_repeat=%d -> %s, %s' % (s, max_repeat, new_s, drop_counts)


def show_unicode(u, max_repeat, new_u, drop_counts):
    print '%s, max_repeat=%d -> %s, %s' % (u, max_repeat, new_u, drop_counts)


def main():
    max_repeat = 2

    asc = 'abcccdddccce'
    uni = '☃☃☃☃☃☃ fuuuuu'
    uni2 = u'\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2'.encode('utf-8')

    for f in [destutter_utf8_encoded_bytes]:
        for s in [asc, uni, uni2]:
            drop_counts = {}
            new_s = f(s, max_repeat, drop_counts)
            show_bytes(s, max_repeat, new_s, drop_counts)

    for u in map(lambda s: s.decode('utf-8'), [asc, uni, uni2]):
        drop_counts = {}
        new_u = destutter_unicode(u, max_repeat, drop_counts)
        show_unicode(u, max_repeat, new_u, drop_counts)


if __name__ == '__main__':
    main()
