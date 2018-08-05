#Prime Select

Prime select was designed to allow you to switch between your Intel video card and Nvidia video card, it facilitates the writing of all the config files and finding of the video cards.
It was designed and tested on Debian Sid (2018-08-05) Using the latest Nvidia-drivers.  I saw the need for this in Debian because Ubuntu already had a switching application, but I preferred to stick with Debian.

##Building
A Couple Of Applications are required:

1.  xrandr
2.  lspci
3.  gtk3.0+-dev
4.  All the Nvidia Drivers

```
./configure
make
sudo make install
```

I use lightdm, the real issue is the Window Manager actually allowing you to line up the screens when you are using the Nvidia card (I really struggled with this).  To fix this

```
nano /etc/lightdm/lightdm.com 

```
Find
```
[Seat:*]
```
Add The Line
```
display-setup-script=/usr/local/bin/prime_switch -s
```

This command sets up all the modesetting.

After you installed the nvidia packages you need to make sure that GLX is set back to mesa-diverted.

```
update-alternatives --config glx
```

I also make that that any intel stuff is gone from X11

```
rm /usr/share/X11/xorg.conf.d/*intel*
```

Alright now we are ready to use prime_select!!

Two modes in prime_select gui and cli to switch to nvidia

GUI:
```
sudo prime_select
```

CLI:
```
sudo prime_select -n
```

To Switch to Intel Only
GUI:
```
sudo prime_select
```

CLI:
```
sudo prime_select -i
```

Getting the current mode

GUI:
```
prime_select
```

CLI:
```
prime_select -m
```

That's it!  it's important to note that you MUST logout and log back in, if anyone knows a way around this let me know, it would be great not to have to logout.

Also, make sure you do the lightdm.conf edit, If you try to force xrandr modesetting with the intel driver only it won't go into X, prime_select -s fixes this issue by detecting the mode.

#Nvidia Packages
These are all the packages I have installed:

ii  glx-alternative-nvidia                0.8.6                                   amd64        allows the selection of NVIDIA as GLX provider
ii  libegl-nvidia0:amd64                  390.77-1                                amd64        NVIDIA binary EGL library
ii  libegl-nvidia0:i386                   390.77-1                                i386         NVIDIA binary EGL library
ii  libgl1-nvidia-glvnd-glx:amd64         390.77-1                                amd64        NVIDIA binary OpenGL/GLX library (GLVND variant)
ii  libgl1-nvidia-glvnd-glx:i386          390.77-1                                i386         NVIDIA binary OpenGL/GLX library (GLVND variant)
ii  libgles-nvidia2:amd64                 390.77-1                                amd64        NVIDIA binary OpenGL|ES 2.x library
ii  libgles-nvidia2:i386                  390.77-1                                i386         NVIDIA binary OpenGL|ES 2.x library
ii  libglx-nvidia0:amd64                  390.77-1                                amd64        NVIDIA binary GLX library
ii  libglx-nvidia0:i386                   390.77-1                                i386         NVIDIA binary GLX library
ii  libnvidia-cfg1:amd64                  390.77-1                                amd64        NVIDIA binary OpenGL/GLX configuration library
ii  libnvidia-cfg1:i386                   390.77-1                                i386         NVIDIA binary OpenGL/GLX configuration library
ii  libnvidia-egl-wayland1:amd64          390.77-1                                amd64        NVIDIA binary Wayland EGL external platform library
ii  libnvidia-egl-wayland1:i386           390.77-1                                i386         NVIDIA binary Wayland EGL external platform library
ii  libnvidia-eglcore:amd64               390.77-1                                amd64        NVIDIA binary EGL core libraries
ii  libnvidia-eglcore:i386                390.77-1                                i386         NVIDIA binary EGL core libraries
ii  libnvidia-glcore:amd64                390.77-1                                amd64        NVIDIA binary OpenGL/GLX core libraries
ii  libnvidia-glcore:i386                 390.77-1                                i386         NVIDIA binary OpenGL/GLX core libraries
ii  libnvidia-ml1:amd64                   390.77-1                                amd64        NVIDIA Management Library (NVML) runtime library
ii  nvidia-alternative                    390.77-1                                amd64        allows the selection of NVIDIA as GLX provider
ii  nvidia-driver                         390.77-1                                amd64        NVIDIA metapackage
ii  nvidia-driver-bin                     390.77-1                                amd64        NVIDIA driver support binaries
ii  nvidia-driver-libs:amd64              390.77-1                                amd64        NVIDIA metapackage (OpenGL/GLX/EGL/GLES libraries)
ii  nvidia-driver-libs:i386               390.77-1                                i386         NVIDIA metapackage (OpenGL/GLX/EGL/GLES libraries)
ii  nvidia-driver-libs-i386:i386          390.77-1                                i386         NVIDIA metapackage (OpenGL/GLX/EGL/GLES 32-bit libraries)
ii  nvidia-egl-common                     390.77-1                                amd64        NVIDIA binary EGL driver - common files
ii  nvidia-egl-icd:amd64                  390.77-1                                amd64        NVIDIA EGL installable client driver (ICD)
ii  nvidia-egl-icd:i386                   390.77-1                                i386         NVIDIA EGL installable client driver (ICD)
ii  nvidia-egl-wayland-common             390.77-1                                amd64        NVIDIA binary Wayland EGL external platform - common files
ii  nvidia-egl-wayland-icd:amd64          390.77-1                                amd64        NVIDIA Wayland EGL external platform library (ICD)
ii  nvidia-egl-wayland-icd:i386           390.77-1                                i386         NVIDIA Wayland EGL external platform library (ICD)
ii  nvidia-installer-cleanup              20151021+8                              amd64        cleanup after driver installation with the nvidia-installer
ii  nvidia-kernel-common                  20151021+8                              amd64        NVIDIA binary kernel module support files
ii  nvidia-kernel-dkms                    390.77-1                                amd64        NVIDIA binary kernel module DKMS source
ii  nvidia-kernel-support                 390.77-1                                amd64        NVIDIA binary kernel module support files
ii  nvidia-legacy-check                   390.77-1                                amd64        check for NVIDIA GPUs requiring a legacy driver
ii  nvidia-modprobe                       390.25-1                                amd64        utility to load NVIDIA kernel modules and create device nodes
ii  nvidia-persistenced                   390.25-1                                amd64        daemon to maintain persistent software state in the NVIDIA driver
ii  nvidia-settings                       390.67-1                                amd64        tool for configuring the NVIDIA graphics driver
ii  nvidia-support                        20151021+8                              amd64        NVIDIA binary graphics driver support files
ii  nvidia-vdpau-driver:amd64             390.77-1                                amd64        Video Decode and Presentation API for Unix - NVIDIA driver
ii  nvidia-vulkan-common                  390.77-1                                amd64        NVIDIA Vulkan driver - common files
ii  nvidia-vulkan-icd:amd64               390.77-1                                amd64        NVIDIA Vulkan installable client driver (ICD)
ii  nvidia-vulkan-icd:i386                390.77-1                                i386         NVIDIA Vulkan installable client driver (ICD)
ii  xserver-xorg-video-nvidia             390.77-1                                amd64        NVIDIA binary Xorg driver
