This branch is for the Qt5 port of wkhtmltopdf/wkhtmltoimage.

# Release Plan

The approach is quite simple -- there are 5 steps which need to happen for the port to be complete.

1. get Qt/WebKit to build in static mode for all platforms
2. get Qt/WebKit running headless on all platforms using QPA
3. port patches to both Qt and WebKit
4. fixes/upgrades to build system to support Qt5 + dependencies
5. upstream changes or APIs introduced in #3 (not required, but ideally should happen)

Based on this, the release plan is as follows:

## 0.13 "alpha"

This is points #1 and #4. Reaching this milestone would mean that we are able to get the static builds without the wkhtmltopdf-specific patches working on all our supported platforms. After this release, we will not have to make any changes to the build process for further milestones.

## 0.13 "beta"

This is point #2. Reaching this milestone would mean that we are now having "headless" static builds and that the behavior of each platform is almost the same due to usage of a custom QPA platform, similar to what is being implemented for PhantomJS 2. We still won't have the wkhtmltopdf-specific patches for this milestone.

## 0.13 "rc"

This is point #3. Now we have the wkhtmltopdf-specific patches and we can start the release candidates and the normal release process can be followed to lead up to the final 0.13 release.

At each milestone release, the application can be tested for completeness and give a preview of the final release.

# Branching Conventions

We will initially have three repositories under the `wkhtmltopdf` organization:
* `wkhtmltopdf` (main repository, has submodule references to the other two)
* `qtbase` (QtBase 5.4.0)
* `qtwebkit` (QtWebKit 5.4.0)

The `wkhtmltopdf` repository will always have a single branch `0.13` which will eventually become the `master` branch (with the current `master` being renamed to `0.12`). There may be other branches created targetting the OS-specific branches of the submodules. Each Qt5 submodule will have the following branches:

Name              | Purpose/Description                                    |
------------------|--------------------------------------------------------|
`wk_master`       | master integration branch, never merged with any other branch -- all commits are cherry-picked to it from the respective branch after testing. No `git push -f` allowed unless absolutely necessary. |
`wk_backports`    | fixes already made in `stable` or `dev` of the official repository -- easy to track which patches are obsoleted if we upgrade to a later version of Qt5. All commits are cherry-picks and no `git push -f` allowed at all. |
`wk_custom`       | patches made to support wkhtmltopdf-specific functionality which may need to be merged/pushed upstream, but may require a lot of work. All commits are cherry-picks from the respective OS branch and no `git push -f` allowed unless absolutely necessary. |
`wk_windows`       | WIP for changes which have been made and tested on Windows -- frequent commits, rebases and `git push -f` are expected |
`wk_linux`         | WIP for changes which have been made and tested on Linux -- frequent commits, rebases and `git push -f` are expected |
`wk_osx`           | WIP for changes which have been made and tested on OS X -- frequent commits, rebases and `git push -f` are expected |

All changes should originate on the OS-specific branches, and after testing/verification should be cherry-picked to the branches for other OSes and tested there. Only after they have been verified on 2 OSes should it be pushed to wk_master (along with wk_custom or wk_backports, if the patch is of that nature).

Please note that the "fork" repositories above should have the remote name `origin` and the official repositories on Gitorious should be added with the name `official`.

# Structure of Build Script

The build script is written in Python and uses a few tricks to make changes easy. It has the following configuration sections at the start:

Section        | Purpose
---------------|-----------------------------------------------------
BUILDERS       | Defines the targets available for the build script and which method will actually build it (code can be shared across targets)
QT_CONFIG      | Defines the Qt4 configuration options to be used for each major build variant. *This may need to be split up into a QtBase configuration and QtWebKit configuration later on.*
FPM_SETUP      | For each supported Linux distribution and OS X, we create native packages using [fpm](https://github.com/jordansissel/fpm) -- this represents the command line options to be used for building the package. *This needs to be in sync with the schroot configuration mentioned below.*
CHROOT_SETUP   | A mini-DSL to configure how a chroot environment should be setup for a given Linux distribution. Normally, the `amd64` and `i386` variants are automatically set up and configured to use `schroot` to allow building as a normal user.
DEPENDENT_LIBS | For Windows (and in some cases, OS X) -- some dependent libraries cannot be obtained from the package manager because there is no standard for it. Hence, we download and configure the libraries from the internet so that they are available -- the exact steps for getting it to work can be different for each target.

Each target is implemented in two functions: `check_<target>` which will verify that the per-requisites for the target are satisfied and `build_<target>` which will actually perform the build steps. The build function can use any of the configurations mentioned above directly or via helper methods defined just below the configuration.

# Tasks for Linux (tentative?)

## 0.13 (alpha)

For the alpha release, our target is to just get a static build working with the necessary changes in the build script. We will pick a particular distribution (e.g. Ubuntu Trusty) and do the following for it, while referring to [building Qt5 from git](http://qt-project.org/wiki/Building_Qt_5_from_Git):

1. identify additional libraries which need to be installed and their package names
  * libxcb
  * libsqlite3
  * libicu
  * libthai (optional?)
  * libxml
  * libxslt
2. Identify the tools which need to be installed in the schroot and get them up and running (e.g. python, ruby, flex, [RedHat Developer Toolset 2.1](http://linux.web.cern.ch/linux/devtoolset/#install) on CentOS 5/6, Python 2.6 from EPEL on CentOS 5)
3. Change the schroot setup so that the above tools and libraries are installed and working in a clean environment. You may need to identify packages/tools which are going to be required always and those that will be obsoleted after the switch to headless build in beta (e.g. libxcb, libx11, libxrender).
4. split up config for Qt5Base and Qt5WebKit and have both built in a working condition. In case patches are needed, first search for them in the following locations:
  * `5.4.0` branch in official Qt5
  * `5.4` branch in official Qt5
  * PhantomJS fork of Qt5 from KDAB
  * PhantomJS fork of Qt5 from Vitallium

Once it works for one target, it would be relatively straightforward to get it working on other distributions.
