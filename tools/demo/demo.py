import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import cv2

dir = "tools/demo/"
output = pd.read_csv(dir + "output.csv", index_col = None)

# Graph params
dpi = 120
w = 360
h = 1080

fig = plt.figure(figsize=(w/dpi,h/dpi),dpi=dpi)


def getRowFromFrame(frameNum):
    startFrame = 81
    endFrame = 1381
    startRow = 404
    endRow = 4504
    m = float(endRow-startRow)/float(endFrame-startFrame)
    c = startRow
    return int((frameNum - startFrame)*m + startRow)

def updateFig(frameNum):
    sensors = [['a',2000],['g',100],['m',200]]
    rowNum = getRowFromFrame(frameNum)
    row = output.iloc[rowNum]
    fig.clear()
    for i, sensor in enumerate(sensors):
        ax = fig.add_subplot(4,1,i+1,projection='3d',title=sensor[0])
        range = [-sensor[1], sensor[1]]
        ax.set_xlim(range)
        ax.set_ylim(range)
        ax.set_zlim(range)
        ax.quiver(0, 0, 0, row[sensor[0]+'X'], row[sensor[0]+'Y'], row[sensor[0]+'Z'])
        ax.annotate(str(round(row[sensor[0]])), (100,100), xycoords = 'axes pixels')
    ax = fig.add_subplot(414,title ='action') 
    inference = list(row['isRoll':'isNone'].values)
    label = ['Roll', 'Grasp', 'Drop', 'None']
    ax.set_ylim([0,1])
    ax.bar(label, inference)
    
    plt.tight_layout(h_pad = 3.0)
    fig.canvas.draw()
    # convert canvas to image
    img = np.frombuffer(fig.canvas.tostring_rgb(), dtype=np.uint8)
    img  = img.reshape(fig.canvas.get_width_height()[::-1] + (3,))
    img = cv2.cvtColor(img,cv2.COLOR_RGB2BGR)
    return img

videopath = dir + "demo.mp4"
outputpath = dir + "output.avi"

cap = cv2.VideoCapture(videopath)
print(cap.read())
print(cap.isOpened())
while cap.isOpened():
    ret, frame = cap.read()
    h, w, _ = frame.shape
    size = (w, h)
    print(size)
    break

if (cap.isOpened()== False): 
  print("Error opening video stream or file")
  quit() 

outputSize = (1920,1080)
out = cv2.VideoWriter(outputpath, cv2.VideoWriter_fourcc(*'MJPG'), 30, outputSize)

# Read until video is completed
frameNum = 0

while(cap.isOpened()):
    # Capture frame-by-frame
    frameNum += 1
    ret, frame = cap.read()
    # if frameNum % 5 != 0:
    #     continue
    if ret == True:
        #image = frame
        image = cv2.hconcat([frame[:,360:,:],updateFig(frameNum)])
        #image.flags.writeable = True
        # Display the resulting frame
        cv2.putText(image, str(frameNum), (10, 70), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 0), 3)
        cv2.putText(image, str(getRowFromFrame(frameNum)), (10, 140), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 0), 3)
        
        out.write(image)
        cv2.imshow('video',image)
        # Press Q on keyboard to  exit
        if cv2.waitKey(25) & 0xFF == ord('q'):
            print(frame.shape)
            break
    # Break the loop
    else: 
        break

# When everything done, release the video capture object
cap.release()
out.release()
 
# Closes all the frames
cv2.destroyAllWindows()

print(frameNum)

quit()

sensors = ['a','g','m']

for rowNum, row in output[:5].iterrows():
    fig = plt.figure()
    axes = []
    mag = []
    for i, sensor in enumerate(sensors):
        axes.append(fig.add_subplot(1,3,i+1,projection='3d',title=sensor))
        axes[-1].set_xlim([-1, 1])
        axes[-1].set_ylim([-1, 1])
        axes[-1].set_zlim([-1, 1])
        axes[-1].quiver(0, 0, 0, row[sensor+'X'], row[sensor+'Y'], row[sensor+'Z'])
        mag.append(row[sensor+'X']**2 + row[sensor+'Y']**2 + row[sensor+'Z']**2)
    #ax = fig.add_subplot(144, title="Magnitude")
    #ax.bar(sensors,mag)

    plt.show()




