# -*- coding: utf-8 -*- 
# file: detect_server.py
# Author: Faceall Group Limited
# brief: detect server file
# Copyright (c) 2015-2016, Faceall Group Limited. All Rights Reserved
# time: 2016.12.20
import time
import os.path
import json
import tornado.auth
import tornado.escape
import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web
from tornado.web import asynchronous
from tornado.options import define, options
from tornado.escape import json_decode
import tornado.concurrent
import cv2
import numpy as np
import sys
import logging

define('port', default=8089, help='run on the given port', type=int)


class Application(tornado.web.Application):
    def __init__(self):
        handlers = [
        (r"/get", GetHandler),
        (r"/post_json", PostJsonHandler),
        (r"/post_form", PostFormHandler),
        (r"/put_json", PutJsonHandler),
        (r"/put_form", PutformHandler)
        ]
        tornado.web.Application.__init__(self, handlers)

class GetHandler(tornado.web.RequestHandler):

    def get(self):
        self.write("get success!!!")

class PostJsonHandler(tornado.web.RequestHandler):

    def post(self):
        post_json = json.loads(self.request.body)
        print post_json
        temp = {"name": "yao"}
        temp = json.dumps(temp)
        self.write(temp)

class PostFormHandler(tornado.web.RequestHandler):

    def post(self):
        post_form = self.get_argument("post_form", None)
        post_id = self.get_argument("post_id", None)
        print post_form
        print post_id
        self.write("post form success!!!")

class PutJsonHandler(tornado.web.RequestHandler):

    def put(self):

        put_json = json.loads(self.request.body)
        print put_json
        self.write("put json success!!!")

class PutformHandler(tornado.web.RequestHandler):

    def put(self):
        put_form = self.get_argument("put_form", None)
        put_id = self.get_argument("put_id", None)
        print put_form
        print put_id
        self.write("put form success!!!")


def main():
    tornado.options.parse_command_line()
    http_server = tornado.httpserver.HTTPServer(Application())
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()

if __name__ == "__main__":
    main()
