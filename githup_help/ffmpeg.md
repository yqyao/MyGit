## ffmpeg 从视频中截取图像帧

## 安装ffmpeg
* 安装环境
 centos 7.0
* 安装命令
```bash
yum -y install ffmpeg
```
## 使用方法
* 命令1
```bash
ffmpeg -i inputfile.avi -r 1 -f image2 image-%05d.jpeg
```
    -r 指定抽取的帧率，即从视频中每秒钟抽取图片的数量。1代表每秒抽取一帧
    -f 指定保存图片使用的格式，可忽略
    image-%05d.jpeg，指定文件的输出名字
* 命令2
```bash
ffmpeg -i inputfile.avi -r 1  -s 4cif -f image2 image-%05d.jpeg
```
    4cif 代表帧的尺寸为705x576.可以显性的设置为705x576
* 命令3
```bash
ffmpeg -i inputfile.avi -r 1 -t 4 -f image2 image-%05d.jpeg
```
    -t 代表持续时间，单位为秒
* 命令4
```bash
ffmpeg -i inputfile.avi -r 1 -ss 00:1:14 -f image2 image-%05d.jpeg
```
    -ss 指定起始时间
* 命令5
```bash
ffmpeg -i inputfile.avi -r 1  -ss 01:30:14 -vframes 120 4cif -f image2 image-%05d.jpeg
```
    -vframes 指定抽取的帧数
* 命令6
```bash
ffmpeg -i inputfile.avi -r 1 -ss 00:1:14 -f image2 image-%05d.jpeg
ffmpeg -i inputfile.avi -r 1 -ss 00:1:14 -f -vframes 1 image2 image-%05d.jpeg
```
    通过指定-ss，和-vframes也可以达到同样的效果。
    这时候-ss参数后跟的时间有两种写法,hh:mm:ss 或 直接写秒数 

