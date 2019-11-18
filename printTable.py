#!/usr/bin/python

from prettytable import from_csv
    
with open("output.txt", "r") as fp: 
    x = from_csv(fp)
    
print(x)