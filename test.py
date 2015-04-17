#!/usr/bin/python
out = './c.out '
ip = '10.37.129.3 '
port = '10901 '
name1 = 'Jack '
name2 = 'Alan '
name3 = 'Becky '
name4 = 'Iris '
name5 = 'Tammy '
name6 = 'Rebecca '
name7 = 'Celestin '
name8 = 'Tony '
name9 = 'Cristin '
name10 = 'Katherine '

year1 = '1 '
year2 = '2 '
year3 = '3 '
year4 = '4 '

process = '&'

command1 = out + ip + port + name1 + year1 + process
command2 = out + ip + port + name2 + year2 + process
command3 = out + ip + port + name3 + year3 + process
command4 = out + ip + port + name4 + year4 + process
command5 = out + ip + port + name5 + year2 + process
command6 = out + ip + port + name6 + year3 + process
command7 = out + ip + port + name7 + year2 + process
command8 = out + ip + port + name8 + year4 + process
command9 = out + ip + port + name9 + year1 + process
command10 = out + ip + port + name10 + year2 + process

import os
os.system(command1);
os.system(command2);
os.system(command3);
os.system(command4);
os.system(command5);
os.system(command6);
os.system(command7);
os.system(command8);
os.system(command9);
os.system(command10);
