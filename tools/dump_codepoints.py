def dump_help(name_of_program):
    print 'Usage:'
    print '* %s (input file)' % name_of_program
    print '* %s (input file) (output file)' % name_of_program
    print
    print 'Dumps the integer code points from (input file) to (output file), '
    print 'one per line.  If output file is not provided, uses stdout.'


def dump_codepoints(in_f_name, out_f_name):
    if out_f_name:
        out_f = open(out_f_name, 'wb')
    else:
        from sys import stdout
        out_f = stdout
    text = open(in_f_name).read().decode('utf-8')
    for c in text:
        out_f.write('%d\n' % ord(c))
    out_f.close()


def main():
    from sys import argv

    if len(argv) == 2:
        _, in_f = argv
        out_f = None
    elif len(argv) == 3:
        _, in_f, out_f = argv
    else:
        dump_help(argv[0])
        return

    dump_codepoints(in_f, out_f)


if __name__ == '__main__':
    main()
