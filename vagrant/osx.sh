#!/bin/bash
xcodebuild -license accept
# http://stackoverflow.com/a/22355874
ARCHFLAGS=-Wno-error=unused-command-line-argument-hard-error-in-future gem install fpm --no-ri --no-rdoc

