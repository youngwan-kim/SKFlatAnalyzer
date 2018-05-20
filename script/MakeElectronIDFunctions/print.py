import os

lines = open('ids.txt').readlines()

for line in lines:
  line = line.strip('\n')

  #print '''  bool {0}() const {{return j_{0};}}'''.format(line)

  #print '''      electron_{0}->at(i),'''.format(line)

  print '''  if(ID=="{0}") return {0}();'''.format(line)
