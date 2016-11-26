# -*- mode: ruby -*-

Vagrant.configure(2) do |config|
    config.vm.box_check_update = false
    config.vm.provider "virtualbox" do |p|
      p.memory = 2048
    end

    config.vm.define "osx" do |osx|
        osx.vm.box           = "AndrewDryga/vagrant-box-osx"
        osx.vm.box_version   = "0.1.0"
        osx.vm.synced_folder ".", "/source", type: 'rsync',
            rsync__exclude: ".git/"
        osx.vm.provision    "shell", inline: <<-OSX_PROVISION
            sudo xcodebuild -license accept
            # http://stackoverflow.com/a/22355874
            sudo ARCHFLAGS=-Wno-error=unused-command-line-argument-hard-error-in-future gem install fpm --no-ri --no-rdoc
        OSX_PROVISION
    end

    config.vm.define "linux64" do |linux|
        linux.vm.box           = "bento/centos-6.7"
        linux.vm.synced_folder ".", "/source"
        linux.vm.provision    "shell", inline: <<-LINUX_PROVISION
            sudo cat <<REPO > /etc/yum.repos.d/slc6.repo
[slc6-scl]
name=Scientific Linux CERN (SLC6) - SCL addons
baseurl=http://linuxsoft.cern.ch/cern/scl/slc6X/x86_64/yum/scl/
gpgcheck=0
enabled=1
REPO
            sudo yum clean all && yum update -y
            sudo yum install -y scl-utils devtoolset-3-gcc-c++ python27 git19 \
                ruby perl git make gzip diffutils gperf bison flex \
                zlib-devel openssl-devel freetype-devel fontconfig-devel \
                libX11-devel libXrender-devel libXext-devel
            echo 'source scl_source enable devtoolset-3 python27 git19' > /etc/profile.d/scl_env.sh
        LINUX_PROVISION
    end

    config.vm.define "linux32" do |linux|
        linux.vm.box           = "bento/centos-6.7"
        linux.vm.synced_folder ".", "/source"
        linux.vm.provision    "shell", inline: <<-LINUX_PROVISION
            sudo cat <<REPO > /etc/yum.repos.d/slc6.repo
[slc6-scl]
name=Scientific Linux CERN (SLC6) - SCL addons
baseurl=http://linuxsoft.cern.ch/cern/scl/slc6X/i386/yum/scl/
gpgcheck=0
enabled=1
REPO
            sudo yum clean all && yum update -y
            sudo yum install -y scl-utils devtoolset-3-gcc-c++ python27 git19 \
                ruby perl git make gzip diffutils gperf bison flex \
                zlib-devel openssl-devel freetype-devel fontconfig-devel \
                libX11-devel libXrender-devel libXext-devel
            echo 'source scl_source enable devtoolset-3 python27 git19' > /etc/profile.d/scl_env.sh
        LINUX_PROVISION
    end

    config.vm.define "mingw" do |mingw|
        mingw.vm.box           = "ubuntu/yakkety64"
        mingw.vm.synced_folder ".", "/source"
        mingw.vm.provision    "shell", inline: <<-MINGW_PROVISION
            sudo apt-get update
            sudo apt-get install -y build-essential mingw-w64 nsis \
                python perl git ruby gperf bison flex
        MINGW_PROVISION
    end
end
