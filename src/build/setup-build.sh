#!/bin/sh
mkdir shared
ln -s ../data data
wxformbuilder -g ../spinach.fbp
