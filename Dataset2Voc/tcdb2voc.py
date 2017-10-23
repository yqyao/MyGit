import os
import os.path as osp
import json
import cv2
import sys
import subprocess
from multiprocessing.dummy import Pool as ThreadPool
label_folder = "/localSSD/yyq/tcdb/labelData/train/tsinghuaDaimlerDataset"
image_folder = "/localSSD/yyq/tcdb/leftImg8bit/train/tsinghuaDaimlerDataset"
save_image_folder = "/localSSD/yyq/tcdb/reshape_image"
save_label_folder = "/localSSD/yyq/tcdb/reshape_label"

def convert2txt(origin_label):
    with open(osp.join(label_folder, origin_label), "rb") as f:
        label_dict = json.load(f)
        image_name = label_dict["imagename"]
        bbox_list = label_dict["children"]
        im = cv2.imread(osp.join(image_folder, image_name))
        im_left = im[:,:1024,:]
        im_left =  cv2.resize(im_left, (512, 512))
        im_right = im[:,1024:2048,:]
        im_right = cv2.resize(im_right, (512, 512))
        image_id = image_name.split('.')[0]
        cv2.imwrite(osp.join(save_image_folder, "".join([image_id, "_left", ".jpg"])), im_left)
        cv2.imwrite(osp.join(save_image_folder, "".join([image_id, "_right", ".jpg"])), im_right)
    left_label = open(osp.join(save_label_folder, "".join([image_id, "_left", ".txt"])), "ab")
    right_label = open(osp.join(save_label_folder, "".join([image_id, "_right", ".txt"])), "ab")        
    for bbox in bbox_list:
        if bbox["mincol"] > 1023:
            xmin = bbox["mincol"] - 1024
            xmax = bbox["maxcol"] - 1024
            ymin = bbox['minrow']
            ymax = bbox['maxrow']
            print >>right_label, "Cyclist", xmin / 2, ymin / 2, xmax / 2, ymax / 2, 512, 512
            continue
        if bbox["maxcol"] < 1024:
            xmin = bbox['mincol']
            xmax = bbox['maxcol']
            ymin = bbox['minrow']
            ymax = bbox['maxrow']
            print >>left_label, "Cyclist", xmin / 2, ymin / 2, xmax / 2, ymax / 2, 512, 512
            continue
    left_label.close()
    right_label.close()

pool = ThreadPool(processes=20)
pool.map(convert2txt, os.listdir(label_folder))
pool.close()
pool.join()

#remove no label txt and image
def removeNoneLabel(label):
# for label in os.listdir(save_label_folder):
    full_path = os.path.join(save_label_folder, label)
    img_path = os.path.join(save_image_folder, label.split('.')[0]+".jpg")
    fsize = os.path.getsize(full_path)
    if fsize == 0:
        subprocess.call(["rm", full_path])
        subprocess.call(["rm", img_path])

pool = ThreadPool(processes=20)
pool.map(removeNoneLabel, os.listdir(save_label_folder))
pool.close()
pool.join()
