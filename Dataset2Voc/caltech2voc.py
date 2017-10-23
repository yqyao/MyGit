#encoding:utf-8
import os, glob
from scipy.io import loadmat
from collections import defaultdict
import numpy as np
from xml.dom.minidom import Document
import cv2
import os.path as osp
from multiprocessing.dummy import Pool as ThreadPool

def vbb_anno2dict(vbb_file, cam_id):
    filename = os.path.splitext(os.path.basename(vbb_file))[0]
    annos = defaultdict(dict)
    vbb = loadmat(vbb_file)
    # object info in each frame: id, pos, occlusion, lock, posv
    objLists = vbb['A'][0][0][1][0]
    objLbl = [str(v[0]) for v in vbb['A'][0][0][4][0]]
    # person index
    person_index_list = np.where(np.array(objLbl) == "person")[0]
    for frame_id, obj in enumerate(objLists):
        if len(obj) > 0:
            frame_name = str(cam_id) + "_" + str(filename) + "_I" + str(frame_id).zfill(5) + ".jpg"
            annos[frame_name] = defaultdict(list)
            annos[frame_name]["id"] = frame_name
            annos[frame_name]["label"] = "person"
            for id, pos, occl in zip(obj['id'][0], obj['pos'][0], obj['occl'][0]):
                id = int(id[0][0]) - 1  # for matlab start from 1 not 0
                if not id in person_index_list:  # only use bbox whose label is person
                    continue
                pos = pos[0].tolist()
                occl = int(occl[0][0])
                annos[frame_name]["occlusion"].append(occl)
                annos[frame_name]["bbox"].append(pos)
            if not annos[frame_name]["bbox"]:
                del annos[frame_name]
    return annos

anno_trans = {"person": "Pedestrian"}

def seq2img(annos, seq_file, outdir, cam_id):
    # read .seq file, and save the images into the savepath
    with open(seq_file,'rb') as f:
        string = str(f.read())
        splitstring = "\xFF\xD8\xFF\xE0\x00\x10\x4A\x46\x49\x46"
        # split .seq file into segment with the image prefix
        strlist=string.split(splitstring)
    index = 1
    for img in strlist[:]:
        v_id = os.path.splitext(os.path.basename(seq_file))[0]
        cap_frames_index = np.sort([int(os.path.splitext(id)[0].split("_")[2][2:]) for id in annos.keys()])
        if not index in cap_frames_index:
            index += 1
            continue
        outname = os.path.join(outdir, str(cam_id)+"_"+v_id+"_I"+str(index).zfill(5)+".jpg")
        with open(outname, "wb+") as f:
            f.write(splitstring)
            f.write(img)
        index += 1
    image_info = dict()
    image_info["height"] = "480"
    image_info["width"] = "640"
    image_info["type"] = ".jpg"
    return image_info
 
 #convert anno_file to txt_label 
def parse_anno_file(vbb_inputdir, seq_inputdir, image_outputdir, txt_label_dir):
    # annotation sub-directories in hda annotation input directory
    assert os.path.exists(vbb_inputdir)
    sub_dirs = os.listdir(vbb_inputdir)
    for sub_dir in sub_dirs[:]:
        print "Parsing annotations of camera: ", sub_dir
        cam_id = sub_dir
        vbb_files = glob.glob(os.path.join(vbb_inputdir, sub_dir, "*.vbb"))
        for vbb_file in vbb_files[:]:
            annos = vbb_anno2dict(vbb_file, cam_id)
            if annos:
                seq_file = os.path.join(seq_inputdir, sub_dir, os.path.splitext(os.path.basename(vbb_file))[0]+".seq")
                if not os.path.exists(image_outputdir):
                    os.makedirs(image_outputdir)
                image_info = seq2img(annos, seq_file, image_outputdir, cam_id)

                for filename, anno in sorted(annos.items(), key=lambda x: x[0])[:]:
                    if "bbox" in anno:
                        name = os.path.splitext(os.path.basename(vbb_file))[0]
                        fullname = os.path.join(txt_label_dir, filename.split(".")[0]+".txt")
                        with open(fullname, "ab") as f:
                            for bbox in anno["bbox"]:
                                print >>f, "Pedestrian", bbox[0], bbox[1], bbox[2], bbox[3], 480, 640

def main():
    dataset_info = {"name": "Caltech", "desc": "Caltech pedestrian"}
    seq_inputdir = "/home/safe_data_dir/caltech_pedestrian"
    vbb_inputdir = "/home/safe_data_dir/caltech_pedestrian/annotations"
    image_outputdir = "/localSSD/yyq/test"
    txt_label_dir = "/localSSD/yyq/test"
    parse_anno_file(vbb_inputdir, seq_inputdir, image_outputdir, txt_label_dir)

if __name__ == "__main__":
    main()