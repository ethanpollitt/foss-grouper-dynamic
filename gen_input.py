#!/usr/bin/env python

import os

def ceil_div(num, denom):
    """Returns the result of division by integers rounded up"""
    return (num + denom - 1) / denom

def build_test_input(bits = 104, inputsize = 100, data_filename="%dKpackets.bin"):
    """Generates a random data file

    inputsize : size of random input in MB
    data_filename : filename template to use, must include a %%d specifier
    """
    # create a random binary file of inputsize megabytes
    data_filename = data_filename % inputsize
    print "** Building test input %s ..." % data_filename
    file_size = ceil_div((bits * inputsize * 1000), 8)
    os.system('head --bytes=%d /dev/urandom > %s' % 
              (file_size, data_filename))
    return data_filename

def main():
	build_test_input(16, 1)

main()

