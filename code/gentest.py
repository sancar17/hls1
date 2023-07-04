import numpy as np
import os,sys
import torch
from torch import nn

def write1D(x,f):
    s = " ".join(map(str,x[0][0].tolist())) + "\n"
    return s

def writeTest(x,w,r,f):
    s = ""
    s += write1D(x,f)
    s += write1D(w,f)
    s += write1D(r,f)
    f.write(s)

def genTest(outfile,N):
    f = open(outfile,"w")
    f.write(str(N) + "\n")
    for i in range(N):
        x = torch.rand(1,1,100,dtype=torch.float)/2 - 1
        w = torch.rand(1,1,3,dtype= torch.float)/2 - 1
        r = nn.functional.conv1d(x,w)
        writeTest(x,w,r,f)
    f.close()

genTest("test.dat",10)
