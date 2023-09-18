FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=non-interactive

ENV DISPLAY :0

# Initial update and basic tools
RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  clang \
  make \
  git \
  wget \
  unzip && \
  apt-get clean && rm -rf /var/lib/apt/lists/*

# Installing graphics and multimedia related packages
RUN apt-get update && apt-get install -y \
  libgtk2.0-dev \
  pkg-config \
  libavcodec-dev \
  libavformat-dev \
  libswscale-dev \
  libtbb2 \
  libtbb-dev \
  libjpeg-dev \
  libpng-dev \
  libtiff-dev \
  gfortran \
  libxine2-dev \
  libv4l-dev \
  libgstreamer1.0-dev \
  libgstreamer-plugins-base1.0-dev \
  libfaac-dev \
  libmp3lame-dev \
  libtheora-dev \
  libvorbis-dev \
  libxvidcore-dev \
  libopencore-amrnb-dev \
  libopencore-amrwb-dev \
  libdc1394-dev && \
  apt-get clean && rm -rf /var/lib/apt/lists/*

# X11 Forwarding
RUN apt-get update && apt-get install -y \
  libgtk-3-dev \
  x11-apps \
  x11-xserver-utils \
  libxext6 \
  libxrender1 && \
  apt-get clean && rm -rf /var/lib/apt/lists/*

# SSH setup that doesn't work
#   && mkdir /var/run/sshd \
#   && mkdir -p /root/.ssh \
#   && chmod 700 /root/.ssh \
#   && ssh-keygen -A \
#   && sed -i "s/^.*PasswordAuthentication.*$/PasswordAuthentication no/" /etc/ssh/sshd_config \
#   && sed -i "s/^.*X11Forwarding.*$/X11Forwarding yes/" /etc/ssh/sshd_config \
#   && sed -i "s/^.*X11UseLocalhost.*$/X11UseLocalhost no/" /etc/ssh/sshd_config \
#   && grep "^X11UseLocalhost" /etc/ssh/sshd_config || echo "X11UseLocalhost no" >> /etc/ssh/sshd_config
# RUN echo "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQCPupfbgN3GSqobNH5Acv71oIZe5uFvcIZGNvqV1ttjYS0HOuqVOxcYbn7vNH471v97P0eGZxJUK7jLQdU4PtZaj1jzY0mpjDC67+doa2nmE6JUm3+eavv8afo3Tw2UKfC2dT2ViJIuNji+azR3KimIWsCr5QXWZfgRey0SwK1O72Cef3zJ88tIiCUlqzGYYA2+xXztpUkMdJRYEi/bToP+eDWSOh157Qw8aCMGJ1gKrZxHoeE7bgmmwqLzhYkA91IdyCAckiawUP+4A5UT3iSe3J6k/LbQVwRpE0vl3v5aGCYKBSs/ea2O6oHh+0zO3LiuUVHNTM0/xPAWZ1PxLmsRg5Mt5H+XVFeR1n9OH/MxCBBTvfjmrZXmhEvcolcOkFx9ivZnGFT7sSES0TxibV/xAKx0jmqvH8UdpKpjmgRRZZfHS2bBoWdQ4qNmsfrMNKVc52okWELlDJmEP8I3p6QfiP90X9eyBONlIwvUY8/X2769AEMtC0TQNqVLNmCt/GvN/7WMKqUIazCKu/48nDgv3Gz7PTYtTAFFBa60tOqtyGWVoc+pfEN4ht+RL0MRouL7mNGGlnwGvh8eYZaAEqVaWfQt52zrBk1z5UegjN6SYIEUt0800D5t9bMn4VdimOyQt5kmBA6WKzatoi6sLTXDeTMr9xzdOr44dWsmrGSQJQ== kshreyas@NAVPC-Autonomos54" >> /root/.ssh/authorized_keys
# ENTRYPOINT ["sh", "-c", "/usr/sbin/sshd && tail -f /dev/null"]

# Packages below cause Docker issues currently
#   qtbase5-dev
#   libopencv-dev
#   openssh-server
# RUN add-apt-repository 'deb http://security.ubuntu.com/ubuntu xenial-security main' && \
#     apt-get update && apt-get install -y libjasper1 && \
#     apt-get clean && rm -rf /var/lib/apt/lists/*

# Downloading and extracting OpenCV and OpenCV Contrib
RUN wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip && \
    wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.x.zip && \
    unzip opencv.zip && \
    unzip opencv_contrib.zip && \
    mv opencv-4.x opencv && \
    mv opencv_contrib-4.x opencv_contrib && \
    mkdir opencv/build && cd opencv/build && \
    cmake -D CMAKE_BUILD_TYPE=RELEASE \
          -D CMAKE_INSTALL_PREFIX=/usr/local \
          -D INSTALL_C_EXAMPLES=OFF \
          -D INSTALL_PYTHON_EXAMPLES=OFF \
          -D ENABLE_FAST_MATH=ON \
          -D BUILD_opencv_java=OFF \
          -D BUILD_ZLIB=ON \
          -D BUILD_TIFF=ON \
          -D WITH_GTK=ON \
          -D WITH_FFMPEG=ON \
          -D WITH_1394=ON \
          -D OPENCV_GENERATE_PKGCONFIG=ON \
          -D OPENCV_PC_FILE_NAME=opencv4.pc \
          -D OPENCV_ENABLE_NONFREE=ON \
          -D WITH_GSTREAMER=ON \
          -D WITH_V4L=ON \
          -D WITH_QT=ON \
          -D WITH_OPENGL=ON \
          -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
          -D BUILD_EXAMPLES=ON .. && \
    make -j4$(nproc) && \
    make install && \
    ldconfig

# Cleanup OpenCV source
RUN rm -rf /opencv && rm -rf /opencv_contrib

# Copy project files and compile project
COPY / .

RUN clang++ test.cpp -o TEST `pkg-config --cflags --libs opencv4`

CMD ["./TEST"]


# Using the docker commands below to verify output -->
# docker build -t image_name .
# docker run image_name
# docker ps -a
# docker cp container_id:/container/path /local/path
# docker rm $(docker ps -aq) 