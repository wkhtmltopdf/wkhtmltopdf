---
layout: default
---

## AppArmor

[AppArmor](https://gitlab.com/apparmor/apparmor/-/wikis/GettingStarted) is a Mandatory
Access Control like security system for Linux, similar to [SELinux](https://github.com/SELinuxProject/selinux).
Wkhtmltopdf is not recommended for use when rendering HTML you don't explicitly
trust, a Mandatory Access Control system is useful for limiting just how much
of your local filesystem an attacker can potentially grab if you are using
wkhtmltopdf on untrusted content. AppArmor is installed and will work more or
less out of the box on:

* [Ubuntu](https://wiki.ubuntu.com/AppArmor) and derivatives
* [SuSE/SLES](https://documentation.suse.com/de-de/sles/15-GA/html/SLES-all/part-apparmor.html) and derivatives
* [Debian](https://wiki.debian.org/AppArmor/HowToUse) and derivatives

RedHat systems do **not** use AppArmor and instead relies entirely on SELinux.

### AppArmor for using wkhtmltopdf on untrusted files
AppArmor provides a kernel-based method to limit what wkhtmltopdf is allowed
to do, and what files it can access. While the `--disable-local-file-access`
option will block access to the local filesystem, an attacker able to exploit
a CVE in the prebuilt binaries may be able to bypass this setting; which is
where AppArmor comes into play. AppArmor provides a secure backstop for
preventing local file access outside of approved locations, as well as preventing
it from executing commands should an attacker find a way to do that.

#### Enable AppArmor if it isn't already
To check if AppArmor is running on your Ubuntu, SuSE/SLES, or Debian system
you can run the command:

    systemctl status apparmor

This should show it both *loaded* and *active*.

If this command shows `Loaded: not-found`, then AppArmor isn't even installed.
RedHat and Fedora systems do not use AppArmor at all, and use SELinux instead,
so this procedure will not work at all with them.

If this shows `Loaded: inactive`, you can enable AppArmor on boot through:

    systemctl enable apparmor
    systemctl start apparmor

Running the status command again should show you both loaded and active.

### Installing the wkhtmltopdf AppArmor profile
The two step process for installing the profile is:

1. Put the profile file into `/etc/apparmor.d`
1. Reload the apparmor profiles through `systemctl reload apparmor`

The profile config-file needs to be customized for your specific usage. The
example below contains the base allows that wkhtmltopdf will need, plus two
example rules for an example application. Please add rules for your specific
use-case in the `# Working paths for your application:` section.

File: /etc/apparmor.d/usr.local.bin.wkhtmltopdf
```
#include <tunables/global>

/usr/local/bin/wkhtmltopdf {
  # Allow base system access.
  #include <abstractions/base>

  # Allows:
  #  - Basic network connectivity (TCP/UDP)
  #  - Nameservice access (the whole resolver chain)
  #  - Ability to interact with PAM
  #  - Ability to interact with name-services like nis, ldap, winbind, kerberos
  # Remove if you don't need network access.
  #include <abstractions/nameservice>
  #include <abstractions/openssl>

  # Allow access to font infrastructure
  #include <abstractions/fonts>
  
  deny capability sys_ptrace,

  # System paths wkhtmltopdf needs to operate
  /proc/*/maps r,
  /usr/local/bin/wkhtmltopdf mr,
  /var/cache/fontconfig/* r,
  /tmp/** rwlk,

  # Working paths for your application, please customize:
  /opt/example_worktree/** rwk,
  /opt/other_workspace/single_dir/* rwk,

}
```

If your usage of wkhtmltopdf doesn't require network access, remove the line
with `#include <abstractions/nameservice>`. Without this line present, any
attempt to make a network connection by wkhtmltopdf will get denied.

To verify your profile is loaded, the following command can be used to make sure
that the AppArmor system loaded your profile:

    sudo aa-status

Which will return a long list of the loaded profiles as well as any running command
that are AppArmor confined. Such as this one:

```
~$ sudo aa-status
apparmor module is loaded.
11 profiles are in enforce mode
   /sbin/dhclient
   /snap/core/9289/usr/lib/snapd/snap-confine
   /snap/core/9289/usr/lib/snapd/snap-confine//mount-namespace-capture-helper
   /usr/bin/lxc-start
   /usr/lib/NetworkManager/nm-dhcp-client.action
   /usr/lib/NetworkManager/nm-dhcp-helper
   /usr/lib/connman/scripts/dhclient-script
   /usr/lib/lxd/lxd-bridge-proxy
   /usr/lib/snapd/snap-confine
   /usr/lib/snapd/snap-confine//mount-namespace-capture-helper
   /usr/local/bin/wkhtmltopdf
```

#### Track AppArmor violations
AppArmor will log events to the system's audit-log. For Ubuntu systems 16.04 and
older this by default is handled in systemd. For SuSE/SLES system, this is found
in `/var/log/audit/audit.log`. After reloading your profiles you should see many
entries similar to this wkhtmltopdf one:

    Jan 12 17:01:57 ip-192-0-2-121 audit[19164]: AVC apparmor="STATUS" operation="profile_replace" profile="unconfined" name="/usr/local/bin/wkhtmltopdf" pid=19164 comm="apparmor_parser"

Or in newer AppArmor versions:

    Jan 12 17:01:57 ip-192-0-2-121 audit[19164]: AVC apparmor="STATUS" operation="profile_replace" info="same as current profile, skipping" operation="profile_replace" profile="unconfined" name="/usr/local/bin/wkhtmltopdf" pid=19164 comm="apparmor_parser"

Violations will show up the same log:

    Jan 12 17:42:03 ip-192-0-2-121 audit[19164]: AVC apparmor="DENIED" operation="open" profile="/usr/local/bin/wkhtmltopdf" name="/etc/passwd" pid=12112 comm="wkhtmltopdf" requested_mask="r" denied_mask="r" fsuid=1000 ouid=0

Which shows that AppArmor denied an attempt to read `/etc/passwd`.
