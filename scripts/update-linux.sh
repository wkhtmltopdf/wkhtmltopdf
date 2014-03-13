#!/bin/bash
#
# Copyright 2014 wkhtmltopdf authors
#
# This file is part of wkhtmltopdf.
#
# wkhtmltopdf is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# wkhtmltopdf is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with wkhtmltopdf.  If not, see <http:#www.gnu.org/licenses/>.

if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

# update the host machine
apt-get update
apt-get dist-upgrade --assume-yes

# update the wheezy chroots
schroot -c wkhtmltopdf-wheezy-amd64 -- apt-get update
schroot -c wkhtmltopdf-wheezy-amd64 -- apt-get dist-upgrade --assume-yes
schroot -c wkhtmltopdf-wheezy-i386  -- apt-get update
schroot -c wkhtmltopdf-wheezy-i386  -- apt-get dist-upgrade --assume-yes

# update the centos 5 chroots
schroot -c wkhtmltopdf-centos5-amd64 -- yum update -y
schroot -c wkhtmltopdf-centos5-i386  -- yum update -y
