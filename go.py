# -*- encoding: utf-8 -*-

from fast_preprocess import (destutter_unicode, destutter_utf8_encoded_bytes)


def show_table(s, indent):
    head = ['idx', 'dec', 'hex', 'chr']
    rows = [head]
    for i, n in enumerate(map(ord, s)):
        try:
            c = unichr(n)
        except:
            c = 'X'
        rows.append([str(i), str(n), hex(n), c])

    max_lens = [0] * 4
    for row in rows:
        for i, s in enumerate(row):
            max_lens[i] = max(max_lens[i], len(s))

    for row in rows:
        ss = [' ' * indent]
        for i, s in enumerate(row):
            ss.append(' ' * (max_lens[i] - len(s) + 1))
            ss.append(s)
        print ''.join(ss)


def show_bytes(s, max_repeat, new_s, drop_counts):
    u = s.decode('utf-8')
    new_u = new_s.decode('utf-8')
    print 'Test:'
    print '  %s (%d chrs in %d bytes), max_repeat=%d -> %s (%d chrs in %d bytes), %s' % (
        s, len(u), len(s), max_repeat, new_s, len(new_u), len(new_s), drop_counts)
    print '  Input as UTF-8:'
    show_table(u, 4)
    print '  Output as UTF-8:'
    show_table(new_u, 4)
    print


def show_unicode(u, max_repeat, new_u, drop_counts):
    print 'Test:'
    print '  %s (%d chrs), max_repeat=%d -> %s (%d chrs), %s' % (
        u, len(u), max_repeat, new_u, len(new_u), drop_counts)
    print '  Input:'
    show_table(u, 4)
    print '  Output:'
    show_table(new_u, 4)
    print


def main():
    max_repeat = 2

    asc = 'abcccdddccce'
    uni = '☃☃☃☃☃☃ fuuuuu'
    uni2 = u'\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2'.encode('utf-8')

    print
    print '-' * 80
    print 'str objects:'
    print

    for f in [destutter_utf8_encoded_bytes]:
        for s in [asc, uni, uni2]:
            drop_counts = {}
            new_s = f(s, max_repeat, drop_counts)
            show_bytes(s, max_repeat, new_s, drop_counts)

    print '-' * 80
    print 'unicode objects:'
    print

    for u in map(lambda s: s.decode('utf-8'), [asc, uni, uni2]):
        drop_counts = {}
        new_u = destutter_unicode(u, max_repeat, drop_counts)
        show_unicode(u, max_repeat, new_u, drop_counts)

    print '-' * 80
    print 'retry 32-bit unicode:'
    print

    if True:
        uni3 = u'\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2\U0001f4f2'
        drop_counts = {}
        new_uni3 = destutter_unicode(uni3, max_repeat, drop_counts)
        show_unicode(uni3, max_repeat, new_uni3, drop_counts)


if __name__ == '__main__':
    main()
