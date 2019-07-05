#!/bin/env python

import os
import sys
import time
import xml.dom.minidom
import getopt

def hashnum(num, max):
    ret = 0
    pos = 0
    while pos < len(num):
        ret = ((37 * ret) & 0xffffffff) + ord(num[pos])
        pos += 1

    return (ret % max)

class Product():
    def __init__(self, line):
        line = line.strip(" ").split("  ")
        self.__id = line[0]
        self.__name = line[1]
        
    def hash(self):
        return self.__hash

    def id(self):
        return self.__id

    def name(self):
        return self.__name

    def create_hash(self, max):
        self.__hash = hashnum(self.__id, max)

class Vendor():

    def __init__(self, line):
        line = line.strip(" ").split("  ")
        self.__id = line[0]
        self.__name = line[1]
        self.__products = [ ]

    def hash(self):
        return self.__hash

    def id(self):
        return self.__id

    def name(self):
        return self.__name

    def add(self, line):
        _prod = Product(line)
        self.__products.extend([_prod])

    def create_hash(self, max):
        self.__hash = hashnum(self.__id, max)
        nr = len(self.__products)
        for p in self.__products[:]:
            p.create_hash(nr)

    def products(self):
        return self.__products

    def number_products(self):
        return len(self.__products)

class Vendors():

    def __init__(self):
        self.__vendors = [ ]
        self.__last_vendor = None

    def add(self, line):
        _vend = Vendor(line)
        self.__vendors.extend([_vend])
        self.__last_vendor = _vend
        return _vend

    def vendors(self):
        return self.__vendors

    def number(self):
        return len(self.__vendors)

    def create_hash(self):
        t1 = time.time()
        print "Calculating the hashes of %i vendors ... " % len(self.__vendors),
        _max = len(self.__vendors)
        for v in self.__vendors[:]:
            v.create_hash(_max)
        t2 = time.time()
        print "[OK]"
        print "Total time %.3f seconds" % (t2 - t1)

    def last_vendor(self):
        return self.__last_vendor


# -- Print usage infos
def Usage():
    print ""
    print "Converter of Libusb devices files to XML"
    print ""
    print 'Usage: %s [ OPTIONS ]' % os.path.basename(sys.argv[0])
    print ""
    print "\t-i, --input=      : Input IDS file (%s)" % INPUT_FILE
    print "\t-o, --output=     : Output XML file (%s)" % OUTPUT_FILE    
    print "\t-h, --help        : Print this help message"
    print ""
    

if __name__ == "__main__":

    # -- Default values
    ifile = INPUT_FILE = "usb.ids"
    ofile = OUTPUT_FILE = "usb.xml"

    try:
        opts, args = getopt.getopt(sys.argv[1:], "i:o:h",
                                   ["input=",
                                    "output=",
                                    "help"])
    except getopt.GetoptError, err:
        print "[ ERROR ] Parsing the input options -- %s" % err[1]
        Usage()
        sys.exit(1)

    for opt, arg in opts:
        if opt in ("-i", "--input"):
            ifile = arg
        elif opt in ("-o", "--outputt"):
            ofile = arg
        elif opt in ("-h", "--help"):
            Usage()
            sys.exit(0)
        else:
            print "[ ERROR ] Unknow input option: %s" % opt
            Usage()
            sys.exit(1)

    # -- Sanity checks
    if not os.path.exists(ifile):
        print "[ ERROR ] Input file '%s' doesn't exist." % ifile
        sys.exit(1)


    vendors = Vendors()

    # -- Read the input file first
    fd = open(ifile, "r")
    for line in fd.readlines():

        if line[0] == '#' or line[0] == '\n':
            continue

        line = line.strip(" \r\n")

        # -- Parse the devices if we already have a vendor
        if line[0] == '\t':
            if vendors.last_vendor() is None:
                continue

            vendors.last_vendor().add(line[1:])
            continue

        vendors.add(line)

    fd.close()

    # -- Create all the hashes
    vendors.create_hash()

    _doc = xml.dom.minidom.Document()
    _root = _doc.createElement('usb')
    _root.setAttribute("vendors", str(vendors.number()))
    _doc.appendChild(_root)

    # -- Create the output file
    print "Starting to create the XML file ... ",
    sys.stdout.flush()
    fdo = open(ofile, "w")
    for v in vendors.vendors()[:]:

        # -- Create the new vendor list with the below attributes
        _v = _doc.createElement("vendor")
        _v.setAttribute("index", str(v.hash()))
        _v.setAttribute("id", v.id())
        _v.setAttribute("name", v.name())
        _v.setAttribute("products", str(v.number_products()))
        _root.appendChild(_v)

        # -- Now add the corresponding products
        for p in v.products()[:]:
            _p = _doc.createElement("product")
            _p.setAttribute("index", str(p.hash()))
            _p.setAttribute("id", p.id())
            _p.setAttribute("name", p.name())
            _v.appendChild(_p)

    fdo.write(_doc.toprettyxml())
    fdo.close()
    print "[OK]"
    sys.exit(0)
