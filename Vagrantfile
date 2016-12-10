# -*- mode: ruby -*-

Vagrant.configure(2) do |config|
    config.vm.box_check_update = false
    config.vm.provider "virtualbox" do |p|
      p.memory = 2048
    end

    config.vm.define "centos6_64" do |linux|
        linux.vm.box       = "bento/centos-6.7"
        linux.vm.provision "shell", path: "vagrant/centos6.sh"
    end

    config.vm.define "centos6_32" do |linux|
        linux.vm.box       = "bento/centos-6.7-i386"
        linux.vm.provision "shell", path: "vagrant/centos6.sh"
    end

    config.vm.define "yaketty64_mingw" do |mingw|
        mingw.vm.box       = "ubuntu/yakkety64"
        mingw.vm.provision "shell", path: "vagrant/mingw.sh"
    end

    config.vm.define "osx_10.9" do |osx|
        osx.vm.box         = "AndrewDryga/vagrant-box-osx"
        osx.vm.box_version = "0.1.0"
        osx.vm.synced_folder ".", "/vagrant", type: "rsync", rsync__exclude: ".git/"
        osx.vm.provision     "shell", path: "vagrant/osx.sh"
    end

    config.vm.define "windows_10" do |windows|
        windows.vm.box           = "gusztavvargadr/windows10ee"
        windows.vm.communicator  = "winrm"
        windows.vm.guest         = :windows
        windows.vm.network :private_network, ip: "10.255.67.217", :netmask => "255.255.0.0", :mac => "080027AEC176"
        windows.vm.network       :forwarded_port, guest: 22,   host: 22222, id: "ssh",   auto_correct: true
        windows.vm.network       :forwarded_port, guest: 5985, host: 5985,  id: "winrm", auto_correct: true
        windows.vm.provision     "shell", path: "vagrant/windows.cmd"
    end

end
