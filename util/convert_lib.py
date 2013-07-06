import glob
import os
import shutil

def main():
    files = glob.glob("./*");
    print files
    for filename in files:
        try:
            fname = filename
            while fname[0] == '.' or fname[0] == '\\' or fname[0] == '/':
                fname = fname[1:]
            fname, ext = fname.split('.', 1)
            fsplit = fname.split('-')
            del fsplit[1]
            del fsplit[-1]
            nfname = "./" + "-".join(fsplit) + '.' + ext
            os.rename(filename, nfname)
            if "dll" in ext:
                if not os.path.exists("./dynamic/"):
                    os.mkdir("./dynamic/")
                shutil.move(nfname, "./dynamic/"+nfname)
        except:
            pass

if __name__ == '__main__':
   main()
