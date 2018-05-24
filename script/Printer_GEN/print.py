import os

lines = open('vars.txt').readlines()

for line in lines:

  words = line.split()
  vartype = words[0]
  varname = words[1]

  #print varname.replace('gen_','j_')+',',
  #print '  inline void SetGenStatusFlag_'+varname.replace('gen_','')+'(int i);'
  #print '  inline void SetGenStatusFlag_'+varname.replace('gen_','')+'(int i){ '+varname.replace('gen_','j_')+' = i; }'
  #print '    gen.SetGenStatusFlag_'+varname.replace('gen_','')+'( '+varname+'->at(i) );'
  print '  inline bool '+varname.replace('gen_','')+'() const { return '+varname.replace('gen_','j_')+'; }'
  
