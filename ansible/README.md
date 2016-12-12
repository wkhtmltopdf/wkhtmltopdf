# Playbooks

Ansible playbooks to setup Vagrant boxes enviroment
to build [wkhtmltopdf][wkhtmltopdf].


## Requirements

- [ansible][ansible] >= 2.0
- [vagrant][vagrant]


## Role variables

- **setup_architecture**: instance architecture (`i386` or `amd64`).
- **setup_dir_build**: directory where repo is mounted (default: `/vagrant`).
  Be sure this matches what you have on the `Vagrantfile`.


## Usage

Before starting,
be sure you have installed all of the dependencies
above.

The `Vagrantfile` reads the `boxes.yml` file
to configure the virtual machines on Oracle Virtualbox.

On the `boxes.yml` you have the host names defined.

The full host name is `{{ box_name }}.vagrant.dev` at the end.

To start and provision a virtual machine (e.g. `centos6_64`):

    vagrant up centos6_64.vagrant.dev

If the virtual machine has already been started
you can execute the following:

    vagrant up --provision centos6_64.vagrant.dev
    # or
    vagrant provision centos6_64.vagrant.dev


[ansible]:  https://ansible.com/    "Ansible"
[vagrant]:  https://vagrantup.com/  "Vagrant"
[wkhtmltopdf]:  https://github.com/wkhtmltopdf/wkhtmltopdf  "wkhtmltopdf"
