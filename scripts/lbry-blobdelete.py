import subprocess
import os
from operator import itemgetter

def writeblobs(fileName):
    run = ["lbrynet file list"]
    tmp = subprocess.check_output(run[0], shell=True).decode('utf8').split("\n")
    blobNum = tmp[len(tmp)-4].split(":")[1].strip()[:-1]

    w = open(fileName, "w")
    run.append(" --page_size=")
    run.append(blobNum)
    blobs = subprocess.check_output(''.join(run), shell=True).decode('utf8')
    w.flush()
    w.write(blobs)

def read(fileName):
    f = open(fileName, "r", encoding='utf-8-sig')
    out = list()
    data = list()

    if f.mode == 'r':
        lines = f.readlines()
        for l in lines:
            l = l.split(":")
            if l[0].strip() == "\"stream_hash\"":
                data.append(l[1].strip()[:-1])
            elif l[0].strip() == "\"total_bytes\"":
                data.append(int(l[1].strip()[:-1]))
            elif l[0].strip() == "\"title\"":
                data.append(l[1].strip())

    for i in range(2,len(data), 3):
        out.append((data[i], data[i-1], data[i-2]))
    return out

def deleteBlobs(blobs):
    answer = 0
    print("Press 1 to ask for each blob (in order of biggest to smallest), press 2 to delete the top 10 biggest blobs, Press 3 to delete all blobs, press 0 to exit");
    try:
        answer = int(input())
    except:
        print("Enter a valid integer from the above choices")
        deleteBlobs(blobs)
    if answer == 0:
        return
    elif answer == 1:
        for blob in blobs:
            print("SIZE: ", blob[0], " TITLE: ", blob[2])
            print("Press anything to delete, return to skip, interrupt (ctrl+c) to exit")
            try:
                answer = input()
            except:
                break
            if answer:
                cmd = ["lbrynet file delete"]
                cmd.append(" --stream_hash=")
                cmd.append(blob[1])
                subprocess.check_call(''.join(cmd), shell=True)
    elif answer == 2:
        for i in range(10):
            cmd = ["lbrynet file delete"]
            cmd.append(" --stream_hash=")
            cmd.append(blobs[i][1])
            print("DELETING ", blobs[i])
            subprocess.check_call(''.join(cmd), shell=True)
    elif answer == 3:
        for blob in blobs:
            cmd = ["lbrynet file delete"]
            cmd.append(" --stream_hash=")
            cmd.append(blob[1])
            print("DELETING ", blob)
            subprocess.check_call(''.join(cmd), shell=True)
    else:
        print("Not a valid choice")
        deleteBlobs(blobs)
    return

def main():
    # get the list of blobs and write them to a file
    blobfile = "tmp.txt"
    writeblobs(blobfile)

    # sort by filesize the blobs with metadata (total_bytes, stream_hash, title)
    blobentries = read(blobfile)
    blobentries.sort(key=itemgetter(0), reverse=True)

    # call function for user input and deleting blob
    deleteBlobs(blobentries)
    
    # delete the blobfile and terminate
    try:
        os.remove(blobfile)
    except:
        print("Couldn't remove temporary file")
    print("DONE WITH PROGRAM")

if __name__ == "__main__":
    main()
