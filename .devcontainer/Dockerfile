# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.209.6/containers/debian/.devcontainer/base.Dockerfile
FROM mcr.microsoft.com/vscode/devcontainers/base:0-focal

RUN : \
    && apt-get update -q \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        curl \
        wget \
        software-properties-common \
    && apt-get update -q \
    && apt-get -y install --no-install-recommends \
        bash-completion \
        bear \
        bison \
        build-essential \
        ca-certificates \
        ccache \
        check \
        dfu-util \
        flex \
        gcovr \
        git \
        gperf \
        jq \
        libpython2.7 \
        libtool \
        libusb-1.0-0-dev \
        pkg-config \
        python3 \
        python3-pip \
        python-is-python3 \
        ripgrep \
        shellcheck \
        tzdata \
        udev \
        unzip \
        usbutils \
        xz-utils \
        zip \
    && python3 -m pip install --upgrade \
        autopep8 \
        pip \
        pyserial \
        pyusb \
        virtualenv \
    && :

# Install idf.py
ARG IDF_CLONE_URL=https://github.com/espressif/esp-idf.git
ARG IDF_CLONE_BRANCH_OR_TAG=v4.4.2
ARG IDF_CHECKOUT_REF=

ENV IDF_PATH=/opt/esp/idf
ENV IDF_TOOLS_PATH=/opt/esp
ENV IDF_CCACHE_ENABLE=1

RUN echo IDF_CHECKOUT_REF=$IDF_CHECKOUT_REF IDF_CLONE_BRANCH_OR_TAG=$IDF_CLONE_BRANCH_OR_TAG && \
    git clone --recursive \
      ${IDF_CLONE_BRANCH_OR_TAG:+-b $IDF_CLONE_BRANCH_OR_TAG} \
      $IDF_CLONE_URL $IDF_PATH && \
    if [ -n "$IDF_CHECKOUT_REF" ]; then \
      cd $IDF_PATH && \
      git checkout $IDF_CHECKOUT_REF && \
      git submodule update --init --recursive; \
    fi

RUN : \
    && update-ca-certificates --fresh \
    && $IDF_PATH/tools/idf_tools.py --non-interactive install required \
    && $IDF_PATH/tools/idf_tools.py --non-interactive install cmake \
    && $IDF_PATH/tools/idf_tools.py --non-interactive install-python-env \
    && rm -rf $IDF_TOOLS_PATH/dist \
    && :

# Give the user the required permissions for using USB devices
ARG USERNAME=vscode
RUN : \
    && usermod -aG dialout $USERNAME \
    && usermod -aG plugdev $USERNAME \
    && :

ARG SHFMT_VERSION=v3.4.2
RUN : \
    && curl -sSfL \
        -o /usr/local/bin/shfmt \
        https://github.com/mvdan/sh/releases/download/${SHFMT_VERSION}/shfmt_${SHFMT_VERSION}_linux_amd64 \
    && chmod +x /usr/local/bin/shfmt \
    && :

RUN : \
    && . /opt/esp/idf/export.sh \
    && pip install --upgrade pyclang \
    && idf_tools.py install xtensa-clang \
    && :

RUN : \
    && mkdir -p /home/$USERNAME/.vscode-server/extensions \
        /home/$USERNAME/.vscode-server-insiders/extensions \
    && chown -R $USERNAME \
        /home/$USERNAME/.vscode-server \
        /home/$USERNAME/.vscode-server-insiders \
    && :