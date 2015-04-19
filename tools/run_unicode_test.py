from argparse import ArgumentParser
import os
import time


def parse_args():
    ap = ArgumentParser()
    ap.add_argument('--python', type=str, default='python')
    ap.add_argument('--tmp_dir', type=str, default='tmp')
    ap.add_argument('--utf8_files_root_dir', type=str,
                    default='testdata/unicode/')
    ap.add_argument('--c_bin', type=str, default='./bin/dump_codepoints')
    ap.add_argument('--py_script', type=str,
                    default='./tools/dump_codepoints.py')
    return ap.parse_args()


def cmd(s):
    return os.system(s)


def test_against_file(args, f):
    py_out_file = '%s/%s' % (args.tmp_dir, 'py_codepoints.txt')
    c_out_file = '%s/%s' % (args.tmp_dir, 'c_codepoints.txt')
    t0 = time.time()
    cmd('%s %s %s %s' % (args.python, args.py_script, f, py_out_file))
    t1 = time.time()
    cmd('%s %s %s' % (args.c_bin, f, c_out_file))
    t2 = time.time()
    py = open(py_out_file).read()
    c = open(c_out_file).read()
    py_ms = (t1 - t0) * 1000
    c_ms = (t2 - t1) * 1000
    ratio = py_ms / c_ms
    print '%8d %7.2f %8d %7.2f %5.2f %s' % (
        len(py.split()), py_ms, len(c.split()), c_ms, ratio, f)
    assert c == py


def run(args):
    os.mkdir(args.tmp_dir)
    print '          python                c  py/c'
    print ' unichrs      ms  unichrs      ms      '
    print '-------- ------- -------- ------- -----'
    for root, dd, ff in os.walk(args.utf8_files_root_dir):
        for f in ff:
            test_against_file(args, root + f)
    cmd('rm -rf %s' % args.tmp_dir)


if __name__ == '__main__':
    run(parse_args())
