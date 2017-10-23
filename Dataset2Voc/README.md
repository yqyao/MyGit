# 各种数据集到Voc格式

## VOC数据集
> 这里我们只看有关检测的部分、

### 以VOC2012为例
> 包含以下文件夹

* JPEGImages 
> 存储着图片

* Annotations
> 存储这相关的标签，以xml格式给出,下面为样例
```xml
<annotation>  
    <folder>VOC2012</folder>                             
    <filename>2007_000392.jpg</filename>                               //文件名  
    <source>                                                           //图像来源（不重要）  
        <database>The VOC2007 Database</database>  
        <annotation>PASCAL VOC2007</annotation>  
        <image>flickr</image>  
    </source>  
    <size>                                               //图像尺寸（长宽以及通道数）                        
        <width>500</width>  
        <height>332</height>  
        <depth>3</depth>  
    </size>  
    <segmented>1</segmented>                                   //是否用于分割（在图像物体识别中01无所谓）  
    <object>                                                           //检测到的物体  
        <name>horse</name>                                         //物体类别  
        <pose>Right</pose>                                         //拍摄角度  
        <truncated>0</truncated>                                   //是否被截断（0表示完整）  
        <difficult>0</difficult>                                   //目标是否难以识别（0表示容易识别）  
        <bndbox>                                                   //bounding-box（包含左下角和右上角xy坐标）  
            <xmin>100</xmin>  
            <ymin>96</ymin>  
            <xmax>355</xmax>  
            <ymax>324</ymax>  
        </bndbox>  
    </object>  
    <object>                                                           //检测到多个物体  
        <name>person</name>  
        <pose>Unspecified</pose>  
        <truncated>0</truncated>  
        <difficult>0</difficult>  
        <bndbox>  
            <xmin>198</xmin>  
            <ymin>58</ymin>  
            <xmax>286</xmax>  
            <ymax>197</ymax>  
        </bndbox>  
    </object>  
</annotation>  

```

* ImageSets
> 存放的是每一种类型的challenge对应的图像数据
对于检测我们关心的是 里面的Main文件夹，有每一类的train.txt 和val.txt以及test.txt

## Caltech Pedestrian数据集
> 这是个行人数据集，数据集提供的图片是以seq的形式存的，标签是以vbb的形式存储的
由于原作者是用matlab实现的，因此格式都是以matlab的形式给出。但是我习惯用python，因此
查找资料，找到了python版本的转化方式

### 生成对应图片以及对应的txt格式的标签

```python
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

```
### Caltech2Voc
> 为了保证和下面保持统一，因此这里先转化一份txt的label，然后用后面的统一脚本转化为xml个的voc格式label


## Kitti 数据集
> KITTI数据集由德国卡尔斯鲁厄理工学院和丰田美国技术研究院联合创办，是目前国际上最大的自动驾驶场景下的计算机视觉算法评测数据集。标签格式为

Valus | Name | Description
---|--- |---
|  1|   type| describes the type of oject
|  1 | truncated| float from 0(non-truncated) to 1(truncated)
| 1|   occuded| 0 -full visible, 1- partly occluded 2 -largely occluded 3-unknown 
| 1| alpha| observation angle of the object
| 4| bbox | 2D bounding bbox of object
|3| dimensions | 3D dimmensions
|3| locations| 3D object location
|1| rotations| Rotation
|1| score| results

### Kitti2Voc

* kitti的标签是以txt的格式给出，每张图片一个txt 标签，我们根据此txt转化为对应的xml即可.为了统一我们先将kitti的txt做一些转化，具体转化生成的txt格式为 type,xmin,ymin,xmax,ymax,height, width.方便后续转xml格式


```python

#encoding:utf-8
'''
根据一个给定的XML Schema，使用DOM树的形式从空白文件生成一个XML。
'''
from xml.dom.minidom import Document
import cv2, os
import os.path as osp
from multiprocessing.dummy import Pool as ThreadPool

def generate_xml(name, split_lines, class_ind, image_info, dataset_info):
    doc = Document()  #创建DOM文档对象

    annotation = doc.createElement('annotation')
    doc.appendChild(annotation)

    title = doc.createElement('folder')
    title_text = doc.createTextNode(dataset_info["name"])
    title.appendChild(title_text)
    annotation.appendChild(title)

    img_name = name + image_info["type"]

    title = doc.createElement('filename')
    title_text = doc.createTextNode(img_name)
    title.appendChild(title_text)
    annotation.appendChild(title)

    source = doc.createElement('source')
    annotation.appendChild(source)

    title = doc.createElement('database')
    title_text = doc.createTextNode(dataset_info["desc"])
    title.appendChild(title_text)
    source.appendChild(title)

    title = doc.createElement('annotation')
    title_text = doc.createTextNode(dataset_info["name"])
    title.appendChild(title_text)
    source.appendChild(title)

    size = doc.createElement('size')
    annotation.appendChild(size)

    title = doc.createElement('width')
    title_text = doc.createTextNode(image_info["width"])
    title.appendChild(title_text)
    size.appendChild(title)

    title = doc.createElement('height')
    title_text = doc.createTextNode(image_info["height"])
    title.appendChild(title_text)
    size.appendChild(title)

    title = doc.createElement('depth')
    title_text = doc.createTextNode(str(3))
    title.appendChild(title_text)
    size.appendChild(title)



    for split_line in split_lines:
        line = split_line.strip().split()
        if line[0] in class_ind:
            object = doc.createElement('object')
            annotation.appendChild(object)

            title = doc.createElement('name')
            title_text = doc.createTextNode(line[0])
            title.appendChild(title_text)
            object.appendChild(title)

            bndbox = doc.createElement('bndbox')
            object.appendChild(bndbox)
            title = doc.createElement('xmin')
            title_text = doc.createTextNode(str(max(0, int(float(line[1])))))
            title.appendChild(title_text)
            bndbox.appendChild(title)
            title = doc.createElement('ymin')
            title_text = doc.createTextNode(str(max(0, int(float(line[2])))))
            title.appendChild(title_text)
            bndbox.appendChild(title)
            title = doc.createElement('xmax')
            title_text = doc.createTextNode(str(min(image_info["width"], int(float(line[3])))))
            title.appendChild(title_text)
            bndbox.appendChild(title)
            title = doc.createElement('ymax')
            title_text = doc.createTextNode(str(min(image_info["height"], int(float(line[4])))))
            title.appendChild(title_text)
            bndbox.appendChild(title)

    with open(os.path.join(label_xml_dir, name + '.xml'), 'wb') as f:
        f.write(doc.toprettyxml(indent = '\t'))

def convert(label):
    filename = label.split(".")[0]
    image_info = {"type": ".jpg"}
    with open(osp.join(label_txt_dir, label), "rb") as f:
        split_lines = f.readlines()
        for split_line in split_lines[:1]:
            line = split_line.strip().split()
            image_info["width"] = line[-1]
            image_info["height"] = line[-2]
        generate_xml(filename, split_lines, class_ind, image_info, dataset_info)


if __name__ == '__main__':
    class_ind=('Pedestrian', 'Car', 'Cyclist')
    label_txt_dir = "/your/txt_label_path"
    label_xml_dir = "/your/xml_out_label_path"
    dataset_info = {"name": "TCDB", "desc": "TCDB dataset"}
    pool = ThreadPool(processes=20)
    pool.map(convert, os.listdir(label_txt_dir))
    pool.close()
    pool.join()

```

## Tsinghua-Daimler Cyclist Benchmark
> 清华大学的一个自行车数据集，我们这里只用到其train的数据，[数据集链接](http://www.gavrila.net/Datasets/Daimler_Pedestrian_Benchmark_D/Tsinghua-Daimler_Cyclist_Detec/tsinghua-daimler_cyclist_detec.html)。其给出的label格式为json格式，比较方便解析

### 图片分割处理
> 原生的图片的尺寸为1024x2048，对于我来说太大，而且比例不大合适，因此我决定对图片进行裁剪。决定生成1024x1024的两张图片，只保留包含完整标注的图片和label

*代码*
```python
import os
import os.path as osp
import json
import cv2
import sys
import subprocess
from multiprocessing.dummy import Pool as ThreadPool
label_folder = "/yourpath/tcdb/labelData/train/tsinghuaDaimlerDataset"
image_folder = "/yourpath/tcdb/leftImg8bit/train/tsinghuaDaimlerDataset"
save_image_folder = "/yourpath/tcdb/reshape_image"
save_label_folder = "/yourpath/tcdb/reshape_label"

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

```

### tcdb2Voc
> 经过前一部分对图片和label进行处理我们最终得到两个文件夹。一个为image存放的是生成的图片，一个是label存放的生成的txt label。我们可以根据之前的脚本生成对应的xml格式

## detrac数据集
> 数据集主要拍摄于北京和天津的道路过街天桥，并手动标注了8250个车辆,标注格式以xml格式给出，但是它是从视频中截帧截出来的，因此一个xml里面包含的是从某个视频截出的所有图片标签。由于test没有标签，我们只用train数据集，包含60个sequences。

### 生成对应的txt_label
```python
#encoding:utf-8
import xml.etree.ElementTree as ET
import os
import os.path as osp

def origin_xml2txt():
    xml_folder = "/localSSD/yyq/detrac/DETRAC-Train-Annotations-XML"
    label_root = "/localSSD/yyq/detrac/label"

    for xml in os.listdir(xml_folder)[:]:
        label_folder = osp.join(label_root, xml.split('.')[0])
        if not os.path.exists(label_folder):
            os.makedirs(label_folder)
        tree = ET.ElementTree(file=osp.join(xml_folder, xml))
        root = tree.getroot()
        for child in root:
            if 'num' in child.attrib:
                image_id = "".join(["img", child.attrib["num"].zfill(5)])
                with open(osp.join(label_folder, "".join([image_id, ".txt"])), "ab") as fw:
                    for target in child:
                        for bbox in target:
                            xmin = max(0, float(bbox[0].attrib['left']))
                            ymin = max(0, float(bbox[0].attrib['top']))
                            xmax = min(float(bbox[0].attrib['left']) + float(bbox[0].attrib["width"]), 960.0)
                            ymax = min(float(bbox[0].attrib['top']) + float(bbox[0].attrib["height"]), 540.0)
                            print >>fw,"Car", xmin, ymin, str(xmax), str(ymax), 540, 960 

```

### detrac2Voc
> 根据上面生成的txt_label，再根据txt2xml的脚本生成对应的xml标注。

