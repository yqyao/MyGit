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


def generate_xml(name, split_lines, class_ind):
    from xml.dom.minidom import Document
    doc = Document()  #创建DOM文档对象

    annotation = doc.createElement('annotation')
    doc.appendChild(annotation)

    title = doc.createElement('folder')
    title_text = doc.createTextNode('DETRAC')
    title.appendChild(title_text)
    annotation.appendChild(title)

    img_name = name+'.jpg'

    title = doc.createElement('filename')
    title_text = doc.createTextNode(img_name)
    title.appendChild(title_text)
    annotation.appendChild(title)

    source = doc.createElement('source')
    annotation.appendChild(source)

    title = doc.createElement('database')
    title_text = doc.createTextNode('The DETRAC Database')
    title.appendChild(title_text)
    source.appendChild(title)

    title = doc.createElement('annotation')
    title_text = doc.createTextNode('DETRAC')
    title.appendChild(title_text)
    source.appendChild(title)

    size = doc.createElement('size')
    annotation.appendChild(size)

    title = doc.createElement('width')
    title_text = doc.createTextNode(str(960))
    title.appendChild(title_text)
    size.appendChild(title)

    title = doc.createElement('height')
    title_text = doc.createTextNode(str(540))
    title.appendChild(title_text)
    size.appendChild(title)

    title = doc.createElement('depth')
    title_text = doc.createTextNode(str(3))
    title.appendChild(title_text)
    size.appendChild(title)



    for split_line in split_lines:
        line=split_line.strip().split()
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
            title_text = doc.createTextNode(str(int(float(line[1]))))
            title.appendChild(title_text)
            bndbox.appendChild(title)
            title = doc.createElement('ymin')
            title_text = doc.createTextNode(str(int(float(line[2]))))
            title.appendChild(title_text)
            bndbox.appendChild(title)
            title = doc.createElement('xmax')
            title_text = doc.createTextNode(str(int(float(line[3]))))
            title.appendChild(title_text)
            bndbox.appendChild(title)
            title = doc.createElement('ymax')
            title_text = doc.createTextNode(str(int(float(line[4]))))
            title.appendChild(title_text)
            bndbox.appendChild(title)

########### 将DOM对象doc写入文件
    with open('/localSSD/yyq/detrac/Annotations/'+name+'.xml','wb') as f:
        f.write(doc.toprettyxml(indent = ''))

def detrac2voc():
    class_ind=('Pedestrian', 'Car', 'Cyclist')
    label_dir = "/localSSD/yyq/detrac/label"
    anno_dir = "/localSSD/yyq/detrac/Annotations"
    for folder in os.listdir(label_dir)[:]:
        label_folder = osp.join(anno_dir, folder)
        if not os.path.exists(label_folder):
            os.makedirs(label_folder)
        for txt in os.listdir(osp.join(label_dir, folder))[:]:
            full_path = osp.join(label_dir, folder, txt)
            with open(full_path, "rb") as f:
                split_lines = f.readlines()
                filename = osp.join(folder, txt.split('.')[0])
                generate_xml(filename, split_lines, class_ind)



if __name__ == '__main__':
    # origin_xml2txt()
    detrac2voc()
