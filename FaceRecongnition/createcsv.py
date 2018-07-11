# coding=utf-8
import sys;
import os;

if __name__ == "__main__":

  if len(sys.argv) != 2:
    print "usage: create_csv <base_path>"
    sys.exit(1)

  BASE_PATH=sys.argv[1]
  SEPARATOR=";"

  label = 0
  with open('./face.txt', 'w') as f:
    for dirname, dirs, filenames in os.walk(BASE_PATH):
      print dirname, dirs, filenames
      for filename in filenames:
        abs_path = "%s/%s" % (dirname, filename)
        labstring = filename.split(".")[0]
        label = int(labstring[7:])
        f.write("%s;%d\n"%(abs_path,label))
  

