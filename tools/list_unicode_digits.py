# Dump every digit-like unicode character.

import regex
import sys


def main():
    re = regex.compile(u'\\d', regex.UNICODE)
    for i in xrange(sys.maxunicode):
        c = unichr(i)
        if re.search(c):
            print i


if __name__ == '__main__':
    main()
