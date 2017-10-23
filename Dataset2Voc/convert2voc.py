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
    dataset_info = {"name": "TCDB", "desc": "TCDB dataset"}
    with open(osp.join(label_txt_dir, label), "rb") as f:
        split_lines = f.readlines()
        for split_line in split_lines[:1]:
            line = split_line.strip().split()
            image_info["width"] = line[-1]
            image_info["height"] = line[-2]
        generate_xml(filename, split_lines, class_ind, image_info, dataset_info)

if __name__ == '__main__':
    class_ind=('Pedestrian', 'Car', 'Cyclist')
    label_txt_dir = "/localSSD/yyq/tcdb/reshape_label"
    label_xml_dir = "/localSSD/yyq/test"

    pool = ThreadPool(processes=20)
    pool.map(convert, os.listdir(label_txt_dir))
    pool.close()
    pool.join()
