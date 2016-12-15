Supervisord 使用文档
====
why 用这个？
---
由于项目需求，需要开启大量的Tornado Server因此需要管理这些进程，手动管理很不方便。因此就去google<br>
发现在实际中Supervisord可以很好的满足需求<br>

Supervisor介绍
---
[Supervisor](http://supervisord.org/ "点击") 是一个 Python 开发的 client/server 系统，可以管理和监控类 UNIX操作系统上面的进程。它可以同时启动，关闭多个进程，使用起来特别的方便

组成部分
---
supervisord(server 部分)：主要负责管理子进程，响应客户端命令以及日志的输出等<br>
supervisorctl(client 部分)：命令行客户端，用户可以通过它与不同的 supervisord 进程联系，获取子进程的状态等<br>
web 页面部分（其实这个不算是一部分，但是也相对于前面独立）<br>

安装与配置
---
###安装
pip install supervisor (centos)<br>
生成指定的配置文件：echo_supervisord_conf > supervisord.conf<br>
###配置
**[unix_http_server]**<br>
;file=/tmp/supervisor.sock   ; (the path to the socket file)<br>
;修改为 /var/run 目录，避免被系统删除<br>
file=/var/run/supervisor.sock   ; socket文件的路径，supervisorctl用XML_RPC和supervisord通信就是通过它进行<br>
;chmod=0700                 ; socket file mode (default 0700)<br>
;chown=nobody:nogroup       ; socket file uid:gid owner<br>
;username=user              ; (default is no username (open server))<br>
;password=123               ; (default is no password (open server))<br>
;[inet_http_server]         ; inet (TCP) server disabled by default(此项用于web 页面)<br>
;port=127.0.0.1:9001        ; (ip_address:port specifier, *:port for ;all iface)<br>
;username=user              ; (default is no username (open server))<br>
;pasword=123               ; (default is no password (open server))<br>
...<br><br>

**[supervisord]** <br>
logfile=/home/faceall/supervisord/supervisord.log ; (main log file;default $CWD/supervisord.log)<br>
logfile_maxbytes=50MB        ; (max main logfile bytes b4 rotation;default 50MB)<br>
logfile_backups=10           ; (num of main logfile rotation backups;default 10)<br>
loglevel=info                ; (log level;default info; others: debug,warn,trace)<br>
pidfile=/home/faceall/supervisord/supervisord.pid ; (supervisord pidfile;default supervisord.pid)<br>
nodaemon=false               ; (start in foreground if true;default false)<br>
minfds=1024                  ; (min. avail startup file descriptors;default 1024)<br>
minprocs=200                 ; (min. avail process descriptors;default 200)<br>
;umask=022                   ; (process file creation umask;default 022)<br>
;user=chrism                 ; (default is current user, required if root)<br>
;identifier=supervisor       ; (supervisord identifier, default is 'supervisor')<br>
;directory=/tmp              ; (default is not to cd during start)<br>
;nocleanup=true              ; (don't clean up tempfiles at start;default false)<br>
;childlogdir=/tmp            ; ('AUTO' child log dir, default $TEMP)<br>
;environment=KEY="value"     ; (key value pairs to add to environment)<br>
;strip_ansi=false            ; (strip ansi escape codes in logs; def. false)<br><br>
**[supervisorctl]**<br>
; 必须和'unix_http_server'里面的设定匹配<br>
;serverurl=unix:///tmp/supervisor.sock ; use a unix:// URL  for a unix socket<br>
;修改为 /var/run 目录，避免被系统删除<br>
serverurl=unix:///var/run/supervisor.sock ; use a unix:// URL  for a unix socket<br>
;serverurl=http://127.0.0.1:9001 ; use an http:// url to specify an inet socket（与之前设置一致）<br>
;username=chris              ; should be same as http_username if set<br>
;password=123                ; should be same as http_password if set<br><br>
**[group:tornadoes]**<br>
programs=tornado-11111<br>

**[program:tornado-11111]**<br>
command=sh start.sh<br> --执行命令
directory=/home/faceall/cython-sdk<br> --执行命令的根目录
user=faceall   --用户<br>
autorestart=false --是否自动重启<br>
startretries=1   --重启尝试次数<br>
redirect_stderr=true --重定向错误<br>
stdout_logfile=/home/faceall/cython-sdk/log/tornado_server.log -- 日志输出路径<br>
loglevel=info    --日志输出级别<br>

stopasgroup=true   ; (是否kill管理的子进程的子进程)send stop signal to the UNIX process group (default false)<br>
killasgroup=true   ; SIGKILL the UNIX process group (def false)<br>

命令详解
---
supervisord  -c supervisord.conf (以指定路径下的配置文件启动supervisord,此时为当前路径)<br>
supervisorctl stop programxxx，停止一个进程<br>
supervisorctl start programxxx，开始一个进程<br>
supervisorctl restart programxxx，重启一个进程<br>
supervisorctl stop groupworker，停止一个组进程<br>
supervisorctl stop groupworker：programxxx， 停止一个组中的一个进程<br>
supervisorctl stop all，停止所有进程（注：start、restart、stop都不会载入最新的配置文件）<br>
supervisorctl reload, 载入最新的配置文件<br>
supervisorctl update，根据最新配置文件，启动新配置或有改动的进程，配置没有改动的进程不会受影响重启<br>

扩展阅读
---
supervisord 只能实现单机的进程管理，如果需要实现多机器同时显示并管理<br>
官方推荐有几个开源的项目:<br>
[Nodevisor](https://github.com/TAKEALOT/nodervisor)<br>
[Supervisord-Monitor](https://github.com/mlazarov/supervisord-monitor)<br>





