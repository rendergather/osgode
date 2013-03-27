#!/bin/env python
# -*- coding: iso-8859-1 -*-


import sys

def print_to_stdout(file_name):
    fd = open(file_name, "r")
    data = fd.read()
    fd.close()

    data = data.replace("à", "&agrave;")
    data = data.replace("è", "&egrave;")
    data = data.replace("ì", "&igrave;")
    data = data.replace("è", "&egrave;")
    data = data.replace("ò", "&ograve;")
    data = data.replace("ù", "&ugrave;")


    data = data.replace("é", "&eacute;")

    data = data.replace("È", "&Egrave;")

    print data




if __name__ == "__main__":
    print_to_stdout(sys.argv[1])
