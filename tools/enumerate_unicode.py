# Dump all Unicode characters to stdout in UTF-8.

import sys


def run(file_name):
    with open(file_name, 'wb') as f:
        for i in range(sys.maxunicode + 1):
            try:
                c = unichr(i)
            except:
                continue
            s = c.encode('utf-8')
            f.write(s)


def main():
    f = sys.argv[1]
    run(f)


if __name__ == '__main__':
    main()
