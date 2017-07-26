## python cv2 连续读图片内存机制
由于项目测试需求， 需要连续用cv2读图片，通过htop发现内存不断增长，开始以为是内存泄露。查找了一圈，从可能出现内存泄露的地方开始查起，包括底层调c的模块，以及python部分。发现并没有内存泄露的情况，在测试的过程中发现连续发同一张图片内存不会增长，多张图片就可能会增长。由此我们猜测可能是由于opencv读图片的问题。
### 验证猜想
```  python
import time
image_list = []
import cv2
# img_list.txt 里面为图片的文件名
with open("img_list.txt", "r") as f:
    for line in f.readlines():
      image_list.append(line.strip())
for img in image_list[:1000]:
    cv2.imread(img)
    time.sleep(0.1)
```
* 测试策略如下：
每隔0.1秒用opencv读取图片，图片的大小各不相同，用htop观察内存使用情况
* 测试结果：
当读取比较大的图片，内存会增加，再读取较小图片，内存不会增大。

### 最终猜测结果
python cv2 读取连续图片可能的内存机制如下：
opencv 在连续读取图片时申请内存会根据读取的最大的图片按需来申请，同时读完后并不会释放，再读取下一张图片时不需要重复的申请内存。
如果遇到更大的图片opencv会申请更大的内存。
