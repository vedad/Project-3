"How does one use this crap?"

import os,sys
os.system('c++ -c *.cpp')
os.system('c++ -o %s -larmadillo -lblas -llapack *.o' % sys.argv[1])
