import os
import pickle
import sys
s="hello"
file=open('./pythonc/rel.pickle','wb')
pickle.dump(s,file)
file.close()
file=open('./pythonc/rel.pickle','rb')
s=pickle.load(file)
print(s)
file.close()
