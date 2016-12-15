Supervisord 使用文档
====
why 用这个？
---
>由于项目需求，需要开启大量的Tornado Server因此需要管理这些进程，手动管理很不方便。因此就去google<br>
发现在实际中Supervisord可以很好的满足需求<br>

Supervisor介绍
---
>Supervisor(http://supervisord.org/)是一个 Python 开发的 client/server 系统，可以管理和监控类 UNIX操作系统上面的进程。它可以同时启动，关闭多个进程，使用起来特别的方便

组成部分
---
> supervisord(server 部分)：主要负责管理子进程，响应客户端命令以及日志的输出等
> supervisorctl(client 部分)：命令行客户端，用户可以通过它与不同的 supervisord 进程联系，获取子进程的状态等
> web 页面部分（其实这个不算是一部分，但是也相对于前面独立）

安装与配置
---
>pip install supervisor (centos)
>生成指定的配置文件：echo_supervisord_conf > supervisord.conf
[unix_http_server]
;file=/tmp/supervisor.sock   ; (the path to the socket file)
;修改为 /var/run 目录，避免被系统删除
file=/var/run/supervisor.sock   ; (the path to the socket file)
;chmod=0700                 ; socket file mode (default 0700)
;chown=nobody:nogroup       ; socket file uid:gid owner
;username=user              ; (default is no username (open server))
;password=123               ; (default is no password (open server))
;[inet_http_server]         ; inet (TCP) server disabled by default(此项用于web 页面)
;port=127.0.0.1:9001        ; (ip_address:port specifier, *:port for ;all iface)
;username=user              ; (default is no username (open server))
;password=123               ; (default is no password (open server))
...
[supervisord]
;logfile=/tmp/supervisord.log ; (main log file;default $CWD/supervisord.log)
;修改为 /var/log 目录，避免被系统删除
logfile=/var/log/supervisor/supervisord.log ; (main log file;default $CWD/supervisord.log)
logfile_maxbytes=50MB        ; (max main logfile bytes b4 rotation;default 50MB)
logfile_backups=10           ; (num of main logfile rotation backups;default 10)
loglevel=info                ; (log level;default info; others: debug,warn,trace)
;pidfile=/tmp/supervisord.pid ; (supervisord pidfile;default supervisord.pid)
;修改为 /var/run 目录，避免被系统删除
pidfile=/var/run/supervisord.pid ; (supervisord pidfile;default supervisord.pid)
...
;设置启动supervisord的用户，一般情况下不要轻易用root用户来启动，除非你真的确定要这么做
;user=chrism                 ; (default is current user, required if root)
...
[supervisorctl]
; 必须和'unix_http_server'里面的设定匹配
;serverurl=unix:///tmp/supervisor.sock ; use a unix:// URL  for a unix socket
;修改为 /var/run 目录，避免被系统删除
serverurl=unix:///var/run/supervisor.sock ; use a unix:// URL  for a unix socket
;serverurl=http://127.0.0.1:9001 ; use an http:// url to specify an inet socket（与之前设置一致）
;username=chris              ; should be same as http_username if set
;password=123                ; should be same as http_password if set

命令详解
---
>supervisord  -c supervisord.conf (以指定路径下的配置文件启动supervisord,此时为当前路径)
supervisorctl stop programxxx，停止一个进程
supervisorctl start programxxx，开始一个进程
supervisorctl restart programxxx，重启一个进程
supervisorctl stop groupworker，停止一个组进程
supervisorctl stop groupworker：programxxx， 停止一个组中的一个进程
supervisorctl stop all，停止所有进程（注：start、restart、stop都不会载入最新的配置文件）
supervisorctl reload, 载入最新的配置文件
supervisorctl update，根据最新配置文件，启动新配置或有改动的进程，配置没有改动的进程不会受影响重启





